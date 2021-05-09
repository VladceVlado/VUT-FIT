#!/usr/bin/env python3

"""
VUT FIT MITAI
KRY 2021/2022
Project: Elliptic-curve cryptography
Author: Vladimir Dusek
Login: xdusek27
"""

from sys import argv

from elliptic_curve import EllipticCurve
from point import Point
from utils import EllipticCurveException, logger

# Elliptic curve parameters
A = -0x3
B = 0x5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B
FP = 0xFFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF

# Starting point (generator)
S = Point(
    x=0x6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296,
    y=0x4FE342E2FE1A7F9B8EE7EB4A7C0F9E162BCE33576B315ECECBB6406837BF51F5,
)

if __name__ == '__main__':
    # Get eliptic curve EC
    ec = EllipticCurve(a=A, b=B, fp=FP)
    logger.info('Elliptic curve: EC = {}'.format(ec))
    logger.info('Staring point: S = {}'.format(S))

    if len(argv) != 2:
        raise EllipticCurveException('Invalid input')

    # Get public key PK
    PK = Point.from_string(argv[1])
    logger.info('Public key: PK = {}'.format(PK))

    # Check whether starting point S is on the elliptic curve EC
    if ec.is_on_curve(S):
        logger.info('S is on the Elliptic curve')
    else:
        raise EllipticCurveException('S is not on the Elliptic curve')

    # Check whether public key PK is on the elliptic curve EC
    if ec.is_on_curve(PK):
        logger.info('PK is on the Elliptic curve')
    else:
        raise EllipticCurveException('PK is not on the Elliptic curve')

    # Let's look for the private key
    point = S
    SK = 1

    while point != PK:
        SK += 1
        logger.info(80 * '-')
        logger.info('[{}] gonna get next point'.format(SK))
        logger.info('S = {}'.format(S))
        logger.info('point = {}'.format(point))

        point = ec.addition(S, point)
        logger.info('next point = {}'.format(point))

        if ec.is_on_curve(point):
            logger.info('New point is on the Elliptic curve')
        else:
            raise EllipticCurveException('New point is not the Elliptic curve')

    logger.info('secret key: SK = {}'.format(SK))
    print(SK)
