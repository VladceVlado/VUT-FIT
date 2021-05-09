"""
VUT FIT MITAI
PDS 2021/2022
Project: Identification of Mobile Traffic using TLS Fingerprinting
Author: Vladimir Dusek
Login: xdusek27
"""

from enum import Enum
from logging import basicConfig, getLogger

# Logging
LOGGING_FORMAT = '%(levelname)s | %(message)s'  # noqa: WPS323
basicConfig(format=LOGGING_FORMAT, level='INFO')
logger = getLogger(__name__)

# File paths
PCAP_TRAIN_DIR = 'pcap_train'
PCAP_TEST_DIR = 'pcap_test'
DATABASE = 'features.json'
DATABASE_VERBOSE = 'features_verbose.json'
RESULTS = 'results.json'

# Forbidden values in TLS communication. We don't want to include them in the computation
# of the JA3 fingerprint because of their randomness.
_GREASE_VALUES = [0x0A0A, 0x1A1A, 0x2A2A, 0x3A3A, 0x4A4A, 0x5A5A, 0x6A6A, 0x7A7A, 0x8A8A, 0x9A9A, 0xAAAA, 0xBABA,
                  0xCACA, 0xDADA, 0xEAEA, 0xFAFA]
_RENEGOTIATION = 65281
_PADDING = 21
FORBIDDEN_VALUES = _GREASE_VALUES + [_RENEGOTIATION, _PADDING]

# TLS content type handshake
CONTENT_TYPE_HANDSHAKE = 22

# TLS client handshake
CLIENT_HELLO = 1

# TLS server handshake
SERVER_HELLO = 2


class App(Enum):
    """
    Enumeration of supported applications
    """

    REVOLUT = 'revolut'
    TWITTER = 'twitter'
    PHOENIX = 'phoenix'
    NETFLIX = 'netflix'
    FORZA = 'forza'
    YOUTUBE = 'youtube'
    YR = 'yr'
    FOREST = 'forest'
    GITHUB = 'github'
    SETTLEUP = 'settleup'


# White list of server names for supported applications
SNI_WHITELIST = {
    App.REVOLUT.value: ['revolut'],
    App.TWITTER.value: ['twitter', 'twimg'],
    App.PHOENIX.value: ['phoenix', 'acinq'],
    App.NETFLIX.value: ['netflix', 'nflxext'],
    App.FORZA.value: ['forza'],
    App.YOUTUBE.value: ['youtube', 'ytimg', 'redirector.googlevideo'],
    App.YR.value: ['yr'],
    App.FOREST.value: ['forest', 'seekrtech'],
    App.GITHUB.value: ['github'],
    App.SETTLEUP.value: ['settleup', 'settle-up'],
}
