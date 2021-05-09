"""
VUT FIT MITAI
PDS 2021/2022
Project: Identification of Mobile Traffic using TLS Fingerprinting
Author: Vladimir Dusek
Login: xdusek27
"""

import json
from os import listdir, sep
from typing import Dict, List

import nest_asyncio
from pyshark import FileCapture

from connection import Connection, extract_connections
from constants import DATABASE, DATABASE_VERBOSE, PCAP_TRAIN_DIR, App, logger
from tls_packet import TlsPacket
from wrapped_packet import WrappedPacket

# Async
nest_asyncio.apply()


def process_training_file(app_name: str, file_path: str) -> List[Connection]:
    """
    Returns list of connections extracted from a training pcap file.

    Args:
        app_name: training app
        file_path: path to the training pcap file

    Returns:
        list of connections (TLS client hello + TLS server hello)
    """
    client_hellos = []
    server_hellos = []

    # Process all the packets from the file
    for packet in FileCapture(file_path):
        wpacket = WrappedPacket(packet)

        if wpacket.is_tls_client_hello() and wpacket.is_server_name_ok(app_name):
            tls_packet = TlsPacket(wpacket, app_name=app_name)
            client_hellos.append(tls_packet)

        if wpacket.is_tls_server_hello():
            server_hellos.append(TlsPacket(wpacket, app_name=app_name))

    return extract_connections(client_hellos, server_hellos)


def process_training_dir(app_name: str, dir_path: str) -> List[Connection]:
    """
    Process all pcap files in the training directory.

    Args:
        app_name: training app
        dir_path: path to the training directory

    Returns:
        list of connections (TLS client hello + TLS server hello)
    """
    logger.info('Processing training data for app "{}"'.format(app_name))
    connections = []

    # Process all the files in the directory
    for file_name in listdir(dir_path):
        file_path = '{}{}{}'.format(dir_path, sep, file_name)
        logger.info('Processing file "{}"'.format(file_path))
        connections += process_training_file(app_name, file_path)

    return connections


def dump_database(database: Dict[str, List[Connection]]) -> None:
    """
    Dump database to the file.

    Args:
        database: database as a dictionary
    """
    logger.info('Dumping database to "{}"'.format(DATABASE))
    output = {}

    for app_name in database.keys():
        conns = {conn.to_tuple() for conn in database[app_name]}
        output[app_name] = [{'client_ja3': conn[0], 'server_ja3':conn[1], 'sni': conn[2]} for conn in conns]

    with open(DATABASE, 'w') as fp:
        json.dump(output, fp, indent=2, sort_keys=True)


def dump_database_verbose(database: Dict[str, List[Connection]]) -> None:
    """
    Dump verbose database to the file.

    Args:
        database: database as a dictionary
    """
    logger.info('Dumping database verbose to "{}"'.format(DATABASE_VERBOSE))
    output = {}

    for app_name in database.keys():
        output[app_name] = [connection.to_output_verbose() for connection in database[app_name]]

    with open(DATABASE_VERBOSE, 'w') as fp:
        json.dump(output, fp, indent=2, sort_keys=True)


if __name__ == '__main__':
    database = {}

    # Process training data
    for app_name in listdir(PCAP_TRAIN_DIR):
        if app_name not in {app.value for app in App}:
            raise ValueError('Uknown app name')

        subdir_path = '{}{}{}'.format(PCAP_TRAIN_DIR, sep, app_name)
        database[app_name] = process_training_dir(app_name, subdir_path)

    # Store the extracted data to the csv
    dump_database(database)
    dump_database_verbose(database)
