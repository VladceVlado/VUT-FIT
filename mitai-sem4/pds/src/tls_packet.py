"""
VUT FIT MITAI
PDS 2021/2022
Project: Identification of Mobile Traffic using TLS Fingerprinting
Author: Vladimir Dusek
Login: xdusek27
"""

from hashlib import md5
from typing import Dict

from constants import CLIENT_HELLO, FORBIDDEN_VALUES, SERVER_HELLO
from wrapped_packet import WrappedPacket


class TlsPacket:
    """
    TLS Handshake Packet
    """

    def __init__(self, wpacket: WrappedPacket, app_name: str = 'unknown') -> None:
        # Extract basic information from the packet
        self.ip_src = wpacket.packet.ip.src
        self.ip_dst = wpacket.packet.ip.dst
        self.port_src = wpacket.packet.tcp.srcport
        self.port_dst = wpacket.packet.tcp.dstport
        self.handshake_type = int(wpacket.packet.ssl.handshake_type)
        self.handshake_version = int(wpacket.packet.ssl.handshake_version, 16)
        self._extract_ciphersuites(wpacket)
        self._extract_extensions(wpacket)
        self._extract_server_name(wpacket)
        self._extract_supported_groups(wpacket)
        self._extract_ec_point_format(wpacket)

        # Extract JA3 digest and fingerprint and store the app name
        self._extract_ja3()
        self._extract_fingerprint()
        self.app_name = app_name

    def __str__(self) -> str:
        return '{}\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n{}'.format(
            'app_name: {}'.format(self.app_name),
            'fingerprint: {}'.format(self.fingerprint.hexdigest()),
            'ja3: {}'.format(self.ja3.hexdigest()),
            '----------------------------------------',
            'ip_src: {}'.format(self.ip_src),
            'ip_dst: {}'.format(self.ip_dst),
            'port_src: {}'.format(self.port_src),
            'port_dst: {}'.format(self.port_dst),
            'handshake_type: {}'.format(self.handshake_type),
            'handshake_version: {}'.format(self.handshake_version),
            'ciphersuites: {}'.format(self.ciphersuites),
            'extensions: {}'.format(self.extensions),
            'server_name: {}'.format(self.server_name),
            'supported_groups: {}'.format(self.supported_groups),
            'ec_point_format: {}'.format(self.ec_point_format),
        )

    def is_client_hello(self) -> bool:
        return self.handshake_type == CLIENT_HELLO

    def is_server_hello(self) -> bool:
        return self.handshake_type == SERVER_HELLO

    def to_dict(self) -> Dict[str, str]:
        """
        Returns TLS packet as a dictionary.

        Returns:
            dicts
        """
        return {
            'packet_fingerprint': self.fingerprint.hexdigest(),
            'ja3': self.ja3.hexdigest(),
            'ip_src': self.ip_src,
            'ip_dst': self.ip_dst,
            'port_src': self.port_src,
            'port_dst': self.port_dst,
            'handshake_type': self.handshake_type,
            'handshake_version': self.handshake_version,
            'ciphersuites': self.ciphersuites,
            'extensions': self.extensions,
            'server_name': self.server_name,
            'supported_groups': self.supported_groups,
            'ec_point_format': self.ec_point_format,
        }

    def _extract_ciphersuites(self, wpacket: WrappedPacket) -> None:
        if wpacket.has_ciphersuites():
            ciphersuites = []
            for elem in wpacket.packet.ssl.handshake_ciphersuite.all_fields:
                if int(elem.show) not in FORBIDDEN_VALUES:
                    ciphersuites.append(elem.show)
            self.ciphersuites = '-'.join(ciphersuites)
        else:
            self.ciphersuites = ''

    def _extract_extensions(self, wpacket: WrappedPacket) -> None:
        if wpacket.has_extentions():
            extensions = []
            for elem in wpacket.packet.ssl.handshake_extension_type.all_fields:
                if int(elem.show) not in FORBIDDEN_VALUES:
                    extensions.append(elem.show)
            self.extensions = '-'.join(extensions)
        else:
            self.ciphersuites = ''

    def _extract_server_name(self, wpacket: WrappedPacket) -> None:
        if wpacket.has_server_name():
            self.server_name = wpacket.packet.ssl.handshake_extensions_server_name
        else:
            self.server_name = ''

    def _extract_supported_groups(self, wpacket: WrappedPacket) -> None:
        if wpacket.has_supported_groups():
            supported_groups = []
            for elem in wpacket.packet.ssl.handshake_extensions_supported_group.all_fields:
                if int(str(elem.show), 16) not in FORBIDDEN_VALUES:
                    supported_groups.append(str(int(str(elem.show), 16)))
            self.supported_groups = '-'.join(supported_groups)
        else:
            self.supported_groups = ''

    def _extract_ec_point_format(self, wpacket: WrappedPacket) -> None:
        if wpacket.has_ec_point_format():
            self.ec_point_format = wpacket.packet.ssl.handshake_extensions_ec_point_format
        else:
            self.ec_point_format = ''

    def _extract_ja3(self) -> None:
        x = '{};{};{};{};{}'.format(
            self.handshake_version,
            self.ciphersuites,
            self.extensions,
            self.supported_groups,
            self.ec_point_format,
        )
        self.ja3 = md5(bytes(x, 'utf-8'))

    def _extract_fingerprint(self) -> None:
        x = '{};{};{};{};{};{};{};{};{};{};{}'.format(
            self.ip_src,
            self.ip_dst,
            self.port_src,
            self.port_dst,
            self.handshake_type,
            self.handshake_version,
            self.ciphersuites,
            self.extensions,
            self.server_name,
            self.supported_groups,
            self.ec_point_format,
        )
        self.fingerprint = md5(bytes(x, 'utf-8'))
