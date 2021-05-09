"""
VUT FIT MITAI
PDS 2021/2022
Project: Identification of Mobile Traffic using TLS Fingerprinting
Author: Vladimir Dusek
Login: xdusek27
"""

import json
from collections import Counter
from os import listdir, sep
from typing import Any, Dict, List

import nest_asyncio
from pyshark import FileCapture

from connection import Connection, extract_connections
from constants import DATABASE, PCAP_TEST_DIR, RESULTS, logger
from tls_packet import TlsPacket
from wrapped_packet import WrappedPacket

# Async
nest_asyncio.apply()


def get_connections(file_path: str) -> List[Connection]:
    """
    Returns list of connections extracted from a pcap file.

    Args:
        file_path: path to the pcap file

    Returns:
        list of connections (TLS client hello + TLS server hello)
    """
    client_hellos = []
    server_hellos = []

    # Process all the packets from the file
    for packet in FileCapture(file_path):
        wpacket = WrappedPacket(packet)

        if wpacket.is_tls_client_hello():
            client_hellos.append(TlsPacket(wpacket))

        elif wpacket.is_tls_server_hello():
            server_hellos.append(TlsPacket(wpacket))

    return extract_connections(client_hellos, server_hellos)


def most_frequent(lst: List[Any]) -> Any:
    """
    Returns most frequent item from the list.

    Args:
        lst: list of items of any types

    Returns:
        Any: most frequent item
    """
    if not lst:
        return ''
    occurence_count = Counter(lst)
    return occurence_count.most_common(1)[0][0]


def process_testing_file(database: Dict[str, Any], file_path: str) -> List[str]:
    """
    Process testing pcap file and recognize the communicating app.

    Args:
        database: database of known apps and their ja3 hashes
        file_path: path to the testing pcap file

    Returns:
        list of recognized apps
    """
    recognize_apps = []

    for connection in get_connections(file_path):
        for app, fingerprints in database.items():
            for fingerprint in fingerprints:
                if connection.compare(fingerprint):
                    recognize_apps.append(app)

    logger.info('recognize_apps = {}'.format(recognize_apps))
    return most_frequent(recognize_apps)


def load_database() -> Dict[str, Any]:
    """
    Load database from a file.

    Returns:
        database as a dictionary
    """
    logger.info('Loading database from "{}"'.format(DATABASE))

    with open(DATABASE) as fp:
        return json.load(fp)


def dump_results(results: Dict[str, str]) -> None:
    """
    Dump results to the file.

    Args:
        results: results as a dictionary
    """
    logger.info('Dumping results to "{}"'.format(RESULTS))

    with open(RESULTS, 'w') as fp:
        json.dump(results, fp, indent=2, sort_keys=True)


if __name__ == '__main__':
    results = {}

    # Load the database of the known applications
    database = load_database()

    # Process testing data
    for file_name in listdir(PCAP_TEST_DIR):
        file_path = '{}{}{}'.format(PCAP_TEST_DIR, sep, file_name)
        logger.info('Process testing pcap file "{}"'.format(file_path))
        results[file_name] = process_testing_file(database, file_path)

    logger.info('results = {}'.format(results))
    dump_results(results)
