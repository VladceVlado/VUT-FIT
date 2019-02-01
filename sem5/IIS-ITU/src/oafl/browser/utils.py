from . import models

from django.http import Http404, HttpResponseBadRequest

from datetime import datetime
import re


def get_season_from_url(matched_argument):
    result = re.search(r'([0-9]{4})-([0-9]{4})', matched_argument).string
    begin_year, end_year = result.split('-')
    # TODO handle Season.DoesNotExist exception
    try:
        season = models.Season.objects.get(begin_date__year=begin_year, end_date__year=end_year)
    except models.Season.DoesNotExist:
        season = None
    return season


def set_current_season(season, request):
    if season is None:
        raise Http404
    request.session['selected_season'] = str(season)


def get_current_season():
    now = datetime.now()
    try:
        return models.Season.objects.exclude(begin_date__lte=now).order_by('begin_date').first()
    except models.Season.DoesNotExist:
        return None


def team_modify_authenticate(request, team_id):
    try:
        team = models.Team.objects.get(id=team_id)
    except models.Team.DoesNotExist:
        raise Http404

    if not (request.user.is_superuser or team.user == request.user):
        raise Http404  # no permission

    return team


def add_players_to_team_util(player_ids, team, remove=False):
    season_team, c = models.SeasonTeam.objects.get_or_create(season=get_current_season(), team=team)

    for id in player_ids:
        player = models.Player.objects.get(id=id)
        if remove:
            player.team_season.remove(season_team)
        else:
            if player.team_season.filter(season=season_team.season).count() > 0:
                # this should not happen unless someone is playing with JS
                raise HttpResponseBadRequest
            player.team_season.add(season_team)
        player.save()
