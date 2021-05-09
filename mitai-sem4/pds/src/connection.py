"""
VUT FIT MITAI
PDS 2021/2022
Project: Identification of Mobile Traffic using TLS Fingerprinting
Author: Vladimir Dusek
Login: xdusek27
"""

from typing import Dict, List, Tuple

from tls_packet import TlsPacket


class Connection:
    """
    Representation of a TLS connection (client hello packet + server hello packet).
    """

    def __init__(self, client_hello: TlsPacket, server_hello: TlsPacket) -> None:
        self.client_hello = client_hello
        self.server_hello = server_hello

    def __str__(self):
        return 'Client Hello\n{}\n\nServer Hello\n{}\n'.format(self.client_hello, self.server_hello)

    def compare(self, another: Dict[str, str]) -> bool:
        """
        Compare connection with another connection (in the dict form).

        Args:
            another: connection as a dictionary with keys client_ja3, server_ja3 and sni

        Returns:
            comparision
        """
        return self.client_hello.ja3.hexdigest() == another['client_ja3'] and \
            self.server_hello.ja3.hexdigest() == another['server_ja3'] and \
            self.client_hello.server_name == another['sni']

    def to_output_verbose(self) -> Dict[str, Dict[str, str]]:
        """
        Get a verbose representation of a connection as a dictionary.

        Returns:
            verbose output representation
        """
        return {
            'client_hello': self.client_hello.to_dict(),
            'server_hello': self.server_hello.to_dict(),
        }

    def to_output(self) -> Dict[str, str]:
        """
        Get a representation of a connection as a dictionary.

        Returns:
            output representation
        """
        return {
            'client_ja3': self.client_hello.ja3.hexdigest(),
            'server_ja3': self.server_hello.ja3.hexdigest(),
        }

    def to_tuple(self) -> Tuple[str, str]:
        """
        Returns connection as a tuple.

        Returns:
            tuple
        """
        return (
            self.client_hello.ja3.hexdigest(),
            self.server_hello.ja3.hexdigest(),
            self.client_hello.server_name,
        )


def extract_connections(client_hellos: List[TlsPacket], server_hellos: List[TlsPacket]) -> List[Connection]:
    """
    Extract connections from a list of TLS client hello packets and TLS server hello packets.

    Args:
        client_hellos: list of client hello packets
        server_hellos: list of server hello packets

    Returns:
        list of connections
    """
    result = []

    for client_hello in client_hellos:
        for server_hello in server_hellos:
            if _is_connection(client_hello, server_hello):
                result.append(Connection(client_hello=client_hello, server_hello=server_hello))

    return result


def _is_connection(client_packet: TlsPacket, server_packet: TlsPacket) -> bool:
    """
    Do client and server TLS hello packets form a connection.

    Args:
        client_packet: client hello packet
        server_packet: server hello packet

    Returns:
        bool
    """
    if client_packet.ip_dst == server_packet.ip_src and \
       client_packet.ip_src == server_packet.ip_dst and \
       client_packet.port_src == server_packet.port_dst and \
       client_packet.port_dst == server_packet.port_src:
        return True
    else:
        return False
