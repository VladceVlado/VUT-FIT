from .models import Season

from django.urls import reverse, NoReverseMatch

import json


def seasons_processor(request):
    seasons = Season.objects.all()
    try:
        season_urls = {
            str(season): reverse(request.resolver_match.url_name, kwargs={'season': season})
            for season in seasons
        }
    except NoReverseMatch:
        season_urls = None

    return {
        'G_seasons': seasons,
        'G_season_urls': json.dumps(season_urls)
    }
