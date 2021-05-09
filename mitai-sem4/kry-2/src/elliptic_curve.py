"""
VUT FIT MITAI
KRY 2021/2022
Project: Elliptic-curve cryptography
Author: Vladimir Dusek
Login: xdusek27
"""

from point import MetaPoint, Point, PointAtInf
from utils import EllipticCurveException


class EllipticCurve:

    def __init__(self, a: int, b: int, fp: int) -> None:
        self.a = a
        self.b = b
        self.fp = fp

    def __str__(self) -> str:
        return 'y**2 % FP = (x**3 + A * x + B) % FP\n  A={a}\n  B={b}\n  FP={fp}'.format(
            a=self.a,
            b=self.b,
            fp=self.fp,
        )

    def addition(self, p1: MetaPoint, p2: MetaPoint) -> MetaPoint:
        """
        Addition of two points on the Elliptic Curve.

        It is a closed binary operation (identity, inversion, associativity, commutativity).

        Args:
            p1: first point
            p2: second point

        Raises:
            EllipticCurveException: when addition cannot be computed

        Returns:
            result of addition
        """
        if p1.is_inf() and p2.is_inf():
            raise EllipticCurveException('Error: cannot get next point')

        elif p1.is_inf():
            return p2

        elif p2.is_inf():
            return p1

        elif self._eq_mod(p1.x, p2.x) and self._eq_mod(p1.y, (-1) * p2.y):
            return PointAtInf()

        elif self._eq_mod(p1.x, p2.x) and self._eq_mod(p1.y, p2.y):
            u = self._mod((3 * p1.x**2 + self.a) * self._inverse_mod(2 * p1.y))

        else:
            u = self._mod((p1.y - p2.y) * self._inverse_mod(p1.x - p2.x))

        v = self._mod(p1.y - u * p1.x)
        x3 = self._mod(u**2 - p1.x - p2.x)
        y3 = self._mod((-1) * u * x3 - v)

        return Point(x=x3, y=y3)

    def is_on_curve(self, p: MetaPoint) -> bool:
        """
        Find out if point lies on the elliptic curve.

        Args:
            p: point

        Returns:
            True if does, False otherwise
        """
        if p.is_inf():
            return True
        return self._eq_mod(p.y**2, p.x**3 + self.a * p.x + self.b)

    def _eq_mod(self, i1: int, i2: int) -> bool:
        return self._mod(i1 - i2) == 0

    def _mod(self, i: int) -> int:
        return i % self.fp

    def _inverse_mod(self, i: int) -> int:
        if self._mod(i) == 0:
            raise EllipticCurveException('Invalid inverse mod call')
        return pow(i, self.fp - 2, self.fp)
