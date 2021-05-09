"""
VUT FIT MITAI
KRY 2021/2022
Project: Elliptic-curve cryptography
Author: Vladimir Dusek
Login: xdusek27
"""

from logging import basicConfig, getLogger

# Logger
LOGGING_FORMAT = '%(levelname)s | %(message)s'  # noqa: WPS323
basicConfig(format=LOGGING_FORMAT, level='WARNING')
logger = getLogger(__name__)


class EllipticCurveException(Exception):
    """
    Elliptic Curve errors
    """
