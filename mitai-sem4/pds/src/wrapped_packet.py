"""
VUT FIT MITAI
PDS 2021/2022
Project: Identification of Mobile Traffic using TLS Fingerprinting
Author: Vladimir Dusek
Login: xdusek27
"""

from pyshark.packet.packet import Packet

from constants import CLIENT_HELLO, CONTENT_TYPE_HANDSHAKE, SERVER_HELLO, SNI_WHITELIST


class WrappedPacket:
    """
    Wrapped PyShark Packet.

    Provide additional methods for recognizing TLS handshake hello packet.
    """

    def __init__(self, packet: Packet) -> None:
        self.packet = packet

    def is_tls_client_hello(self) -> bool:
        if self.is_ip() and \
           self.is_tcp() and \
           self.is_ssl() and \
           self.has_ips() and \
           self.has_ports() and \
           self.is_handshake() and \
           self.is_client_hello() and \
           self.has_handshake_version() and \
           self.has_server_name():

            return True
        else:
            return False

    def is_tls_server_hello(self) -> bool:
        if self.is_ip() and \
           self.is_tcp() and \
           self.is_ssl() and \
           self.has_ips() and \
           self.has_ports() and \
           self.is_handshake() and \
           self.is_server_hello() and \
           self.has_handshake_version():
            return True
        else:
            return False

    def is_ip(self) -> bool:
        return hasattr(self.packet, 'ip')

    def is_tcp(self) -> bool:
        return hasattr(self.packet, 'tcp')

    def has_ips(self) -> bool:
        return hasattr(self.packet.ip, 'src') and hasattr(self.packet.ip, 'dst')

    def has_ports(self) -> bool:
        return hasattr(self.packet.tcp, 'srcport') and hasattr(self.packet.tcp, 'dstport')

    def is_ssl(self) -> bool:
        return hasattr(self.packet, 'ssl')

    def is_handshake(self) -> bool:
        try:
            return int(self.packet.ssl.record_content_type) == CONTENT_TYPE_HANDSHAKE
        except AttributeError:
            return False

    def is_client_hello(self) -> bool:
        try:
            return int(self.packet.ssl.handshake_type) == CLIENT_HELLO
        except AttributeError:
            return False

    def is_server_hello(self) -> bool:
        try:
            return int(self.packet.ssl.handshake_type) == SERVER_HELLO
        except AttributeError:
            return False

    def has_handshake_version(self) -> bool:
        return hasattr(self.packet.ssl, 'handshake_version')

    def has_ciphersuites(self) -> bool:
        return hasattr(self.packet.ssl, 'handshake_ciphersuite')

    def has_extentions(self) -> bool:
        return hasattr(self.packet.ssl, 'handshake_extension_type')

    def has_server_name(self) -> bool:
        return hasattr(self.packet.ssl, 'handshake_extensions_server_name')

    def is_server_name_ok(self, app_name: str) -> bool:
        server_name = self.packet.ssl.handshake_extensions_server_name
        for candidate in SNI_WHITELIST[app_name]:
            if candidate in server_name:
                return True
        return False

    def has_supported_groups(self) -> bool:
        return hasattr(self.packet.ssl, 'handshake_extensions_supported_group')

    def has_ec_point_format(self) -> bool:
        return hasattr(self.packet.ssl, 'handshake_extensions_ec_point_format')
