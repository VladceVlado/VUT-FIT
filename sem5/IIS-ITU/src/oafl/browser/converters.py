from .utils import get_season_from_url


class SeasonConverter:
    regex = r'[0-9]{4}\-[0-9]{4}'

    @staticmethod
    def to_python(value):
        return get_season_from_url(value)

    @staticmethod
    def to_url(value):
        return str(value)
