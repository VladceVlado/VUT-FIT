"""
VUT FIT MITAI
KRY 2021/2022
Project: Elliptic-curve cryptography
Author: Vladimir Dusek
Login: xdusek27
"""

from ast import literal_eval


class MetaPoint:

    def __repr__(self):
        return str(self)

    def __eq__(self, another: object) -> bool:
        if isinstance(self, Point) and isinstance(another, Point):
            return self.x == another.x and self.y == another.y
        elif isinstance(self, PointAtInf) and isinstance(another, PointAtInf):
            return True
        return False

    def is_inf(self) -> bool:
        """
        Returns:
            True if point is point at infinity, False otherwise.
        """
        return isinstance(self, PointAtInf)


class Point(MetaPoint):

    def __init__(self, x: int, y: int) -> None:
        self.x = x
        self.y = y

    def __str__(self) -> str:
        return '({}, {})'.format(self.x, self.y)

    @classmethod
    def from_string(cls, p: str) -> object:
        """
        Instantiate Point from the string definition.

        Args:
            p: point as a string

        Returns:
            point as a Point
        """
        x, y = literal_eval(p)
        return cls(x, y)


class PointAtInf(MetaPoint):

    def __str__(self) -> str:
        return '(inf, inf)'
