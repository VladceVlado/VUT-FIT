from . import models
from .utils import set_current_season, get_current_season, team_modify_authenticate, add_players_to_team_util
from . import forms

from django.shortcuts import render, redirect
from django.core.paginator import Paginator, EmptyPage, PageNotAnInteger
from django.db.models import Sum
from django.contrib.auth import login, authenticate, logout
from django.http import Http404

from collections import defaultdict
from datetime import date


def season_redirect(request):
    latest_season = models.Season.objects.latest('begin_date')
    selected_season = request.session.get('selected_season', str(latest_season))
    request.session['selected_season'] = str(selected_season)
    return redirect('index', season=str(selected_season))


def index(request, season):
    set_current_season(season, request)
    return render(request, 'browser/index.html', {'current_season': season})


def schedule(request, season):
    set_current_season(season, request)
    data = []
    rounds = models.Round.objects.filter(season__exact=season)

    for rnd in rounds:
        inner_data = []
        matches = models.Match.objects.filter(round__exact=rnd)
        if matches.count() == 0:
            continue

        for match in matches:
            inner_data.append({'number': match.number, 'date_time': match.date_time, 'played': match.played,
                               'home_team': match.home_team, 'away_team': match.away_team})

        inner_data = sorted(inner_data, key=lambda m: m['number'])
        data.append({'number': rnd.number, 'type': rnd.type, 'matches': inner_data})

    data = sorted(data, key=lambda r: r['number'])

    page = request.GET.get('page', 1)
    paginator = Paginator(data, 1)

    try:
        data = paginator.page(page)
    except PageNotAnInteger:
        data = paginator.page(1)
    except EmptyPage:
        data = paginator.page(paginator.num_pages)

    return render(request, 'browser/schedule.html', {'data': data})


def results(request, season):
    set_current_season(season, request)
    data = []
    rounds = models.Round.objects.filter(season__exact=season)

    for rnd in rounds:
        inner_data = []
        matches = models.Match.objects.filter(round__exact=rnd, played__exact=True)
        if matches.count() == 0:
            continue

        for match in matches:
            goals = models.Goal.objects.filter(match__exact=match)
            penalties = models.Penalty.objects.filter(match__exact=match)

            home_team_events = []
            away_team_events = []
            home_team_goals = 0
            away_team_goals = 0

            for goal in goals:
                if goal.score_team_id == match.home_team_id:
                    home_team_events.append({'time': goal.time, 'type': 'goal', 'score_player': goal.score_player,
                                             'assist_player': goal.assist_player})
                    home_team_goals += 1
                elif goal.score_team_id == match.away_team_id:
                    away_team_events.append({'time': goal.time, 'type': 'goal', 'score_player': goal.score_player,
                                             'assist_player': goal.assist_player})
                    away_team_goals += 1

            for penalty in penalties:
                if penalty.player_id in match.home_team_players.all().values('id').values_list('id', flat=True):
                    home_team_events.append({'time': penalty.time, 'type': 'penalty', 'length': penalty.length,
                                             'player': penalty.player})
                elif penalty.player_id in match.away_team_players.all().values('id').values_list('id', flat=True):
                    away_team_events.append({'time': penalty.time, 'type': 'penalty', 'length': penalty.length,
                                             'player': penalty.player})

            home_team_events = sorted(home_team_events, key=lambda e: e['time'])
            away_team_events = sorted(away_team_events, key=lambda e: e['time'])

            inner_data.append({'number': match.number, 'date_time': match.date_time, 'played': match.played,
                               'home_team': match.home_team, 'away_team': match.away_team,
                               'home_team_goals': home_team_goals, 'away_team_goals': away_team_goals,
                               'home_team_events': home_team_events, 'away_team_events': away_team_events})

        inner_data = sorted(inner_data, key=lambda m: m['number'])
        data.append({'number': rnd.number, 'type': rnd.type, 'matches': inner_data})

    data = sorted(data, key=lambda r: r['number'])

    page = request.GET.get('page', 1)
    paginator = Paginator(data, 1)

    try:
        data = paginator.page(page)
    except PageNotAnInteger:
        data = paginator.page(1)
    except EmptyPage:
        data = paginator.page(paginator.num_pages)

    return render(request, 'browser/results.html', {'data': data})


def table(request, season):
    set_current_season(season, request)
    data = []
    matches = models.Match.objects.filter(round__season__exact=season, played__exact=True)
    teams = season.teams.all()

    for team in teams:
        matches_cnt = 0
        wins_cnt = 0
        draws_cnt = 0
        loses_cnt = 0
        scored_goals_cnt = 0
        collect_goals_cnt = 0
        points_cnt = 0

        for match in matches:
            if team.id == match.home_team_id or team.id == match.away_team_id:
                matches_cnt += 1
                goals = models.Goal.objects.filter(match__exact=match)
                scored_goals_this_match_cnt = 0
                collect_goals_this_match_cnt = 0

                for goal in goals:
                    if goal.score_team_id == team.id:
                        scored_goals_this_match_cnt += 1
                    elif goal.collect_team_id == team.id:
                        collect_goals_this_match_cnt += 1

                if scored_goals_this_match_cnt > collect_goals_this_match_cnt:
                    wins_cnt += 1
                    points_cnt += 3
                elif scored_goals_this_match_cnt < collect_goals_this_match_cnt:
                    loses_cnt += 1
                else:
                    draws_cnt += 1
                    points_cnt += 1

                scored_goals_cnt += scored_goals_this_match_cnt
                collect_goals_cnt += collect_goals_this_match_cnt

        data.append({'team_name': team.name, 'matches': matches_cnt, 'wins': wins_cnt, 'draws': draws_cnt,
                     'loses': loses_cnt, 'scored_goals': scored_goals_cnt, 'collect_goals': collect_goals_cnt,
                     'diff_goals': scored_goals_cnt - collect_goals_cnt, 'points': points_cnt})

    data = sorted(data, key=lambda t: (t['points'], t['diff_goals'], t['scored_goals']), reverse=True)

    return render(request, 'browser/table.html', {'data': data})


def teams(request, season):
    set_current_season(season, request)
    data = {"teams": []}

    for team in season.teams.all():
        seasons_played = models.Season.objects.filter(teams=team, begin_date__lt=season.begin_date).count()
        if date.today() > season.end_date:
            seasons_played += 1
        seasons_team = models.SeasonTeam.objects.filter(team=team) \
                                                .exclude(final_place=0) \
                                                .filter(season__end_date__lte=date.today()) \
                                                .order_by('final_place')
        best_place = seasons_team.first().final_place\
            if seasons_team.count() > 0 and seasons_team.first().final_place is not None\
            else "N/A"
        season_team = models.SeasonTeam.objects.get(season=season, team=team)
        players = models.Player.objects.filter(team_season=season_team)

        if players.count() == 0:
            # ignore empty teams
            continue

        data["teams"].append({
            "name": team.name,
            "players": players,
            "seasons_played": seasons_played,
            "best_place": best_place
        })

    return render(request, 'browser/teams.html', data)


def stats(request, season):
    set_current_season(season, request)
    data = {"players": []}
    for player in models.Player.objects.all():
        if player.team_season.filter(season=season).count() == 0:
            continue  # skip players that did not play selected season
        goals_count = models.Goal.objects.filter(score_player=player, match__round__season=season).count()
        assists = models.Goal.objects.filter(assist_player=player ,match__round__season=season).count()
        penalty_minutes = models.Penalty.objects.filter(player=player, match__round__season=season).aggregate(Sum("length"))["length__sum"]
        played_matches = models.Match.objects.filter(played=True, round__season=season)
        data["players"].append({
            "name": player,
            "team": player.team_season.get(season=season).team,
            "goals": goals_count,
            "assists": assists,
            "matches": played_matches.filter(home_team_players=player).count() +
                       played_matches.filter(away_team_players=player).count(),
            "penalty_minutes": penalty_minutes if penalty_minutes is not None else 0,
            "points": goals_count + assists
        })

    data["players"].sort(key=lambda x: x["points"], reverse=True)
    for i, player in enumerate(data["players"], 1):
        player["rank"] = i

    page = request.GET.get('page', 1)
    paginator = Paginator(data["players"], 20)

    try:
        data["players"] = paginator.page(page)
    except PageNotAnInteger:
        data["players"] = paginator.page(1)
    except EmptyPage:
        data["players"] = paginator.page(paginator.num_pages)

    return render(request, 'browser/stats.html', data)


def spider(request, season):
    set_current_season(season, request)

    qf_matches = models.Match.objects.filter(round__season=season, round__type=models.Round.QUATERFINAL)
    sf_matches = models.Match.objects.filter(round__season=season, round__type=models.Round.SEMIFINAL)
    f_matches = models.Match.objects.filter(round__season=season, round__type=models.Round.FINAL)

    def create_round_dict(matches):
        ret_dict = defaultdict(lambda: {
            "away_team_wins": 0,
            "home_team_wins": 0,
        })
        for match in matches:
            score_home_team = models.Goal.objects.filter(match=match, score_team=match.home_team).count()
            score_away_team = models.Goal.objects.filter(match=match, score_team=match.away_team).count()
            dict_key = tuple(sorted((str(match.home_team), str(match.away_team))))

            if score_home_team == score_away_team:
                pass
            elif score_home_team > score_away_team:
                ret_dict[dict_key]["home_team_wins"] += 1
            else:
                ret_dict[dict_key]["away_team_wins"] += 1
        return ret_dict

    qf_dict = create_round_dict(qf_matches)
    sf_dict = create_round_dict(sf_matches)
    f_dict = create_round_dict(f_matches)

    def create_round_data(round_dict):
        ret_list = []
        for key in round_dict:
            ret_list.append({
                "home_team": {
                    "name": key[0],
                    "score": round_dict[key]["home_team_wins"]
                },
                "away_team": {
                    "name": key[1],
                    "score": round_dict[key]["away_team_wins"]
                }
            })
        return ret_list

    data = {
        "QF": create_round_data(qf_dict),
        "SF": create_round_data(sf_dict),
        "final": create_round_data(f_dict),
    }

    return render(request, 'browser/spider.html', data)


def team_administration(request):
    if not request.user.is_authenticated:
        return redirect('user_login')

    data = {
        "player_creation_success": "",
        "create_player_form": forms.PlayerForm(),
        "team_form": forms.TeamForm(),
        "registered_teams": models.Team.objects.filter(user=request.user)
    }

    return render(request, 'browser/team_registration.html', data)


def team_registration(request):
    if not request.user.is_authenticated:
        return redirect('user_login')

    player_creation_success = ""

    if request.method == 'POST':
        team_form = forms.TeamForm(request.POST)
        if team_form.is_valid():
            team = team_form.save(commit=False)
            team.user = request.user
            team.save()
            team_form = forms.TeamForm()
    else:
        raise Http404

    data = {
        "player_creation_success": player_creation_success,
        "create_player_form": forms.PlayerForm(),
        "team_form": team_form,
        "registered_teams": models.Team.objects.filter(user=request.user)
    }

    return render(request, 'browser/team_registration.html', data)


def player_registration(request):
    if not request.user.is_authenticated:
        return redirect('user_login')

    player_creation_success = ""

    if request.method == 'POST':
        player_form = forms.PlayerForm(request.POST)
        if player_form.is_valid():
            player_form.save()
            player_creation_success = "Registrace hráče '{} {}' byla úspěšná".format(
                player_form.cleaned_data['first_name'],
                player_form.cleaned_data['last_name']
            )
            player_form = forms.PlayerForm()
    else:
        raise Http404

    data = {
        "player_creation_success": player_creation_success,
        "create_player_form": player_form,
        "team_form": forms.TeamForm(),
        "registered_teams": models.Team.objects.filter(user=request.user)
    }

    return render(request, 'browser/team_registration.html', data)


def team_modify(request, team_id):
    if not request.user.is_authenticated:
        return redirect('user_login')

    team = team_modify_authenticate(request, team_id)

    current_season = None
    seasons_data = []
    for season in models.Season.objects.all():
        team_season, c = models.SeasonTeam.objects.get_or_create(team=team, season=season)
        if not team_season.registered and not season.is_current():
            continue

        season_data = {
            "players": models.Player.objects.filter(team_season=team_season).order_by("first_name"),
            "title": "Sezóna {}".format(season),
            "played": team in season.teams.all(),
            "id": season.id,
            "current": season.is_current(),
            "registered": team_season.registered
        }

        seasons_data.append(season_data)

        if season_data["current"]:
            current_season = season_data

    seasons_data.sort(key=lambda x: x["current"], reverse=True)

    data = {
        "team": team,
        "team_form": forms.TeamForm(instance=team),
        "seasons_data": seasons_data,
    }

    if current_season is not None:
        data.update({
            "season_team_form": forms.SeasonTeamForm(),
            "players_to_add": models.Player.objects.exclude(team_season__season=get_current_season()),
            "current_season": current_season,
        })

    return render(request, 'browser/team_modify.html', data)


def team_rename(request, team_id):
    if not request.user.is_authenticated or request.method != 'POST':
        return redirect('user_login')

    team = team_modify_authenticate(request, team_id)

    team_form = forms.TeamForm(request.POST, instance=team)
    if team_form.is_valid():
        team_form.save()

    return redirect("team_modify", team_id=team_id)


def team_register(request, team_id):
    if not request.user.is_authenticated or request.method != 'POST':
        return redirect('user_login')

    team = team_modify_authenticate(request, team_id)

    team_form = forms.TeamForm(request.POST, instance=team)
    if team_form.is_valid():
        team_form.save()

    return redirect("team_modify", team_id=team_id)


def register_team_season(request, team_id):
    if not request.user.is_authenticated or request.method != 'POST':
        return redirect('user_login')

    team = team_modify_authenticate(request, team_id)

    try:
        season_team = models.SeasonTeam.objects.get(team=team, season=get_current_season())
    except models.SeasonTeam.DoesNotExist:
        season_team = None

    if season_team:
        season_team.registered = True
        season_team.save()

    return redirect("team_modify", team_id=team_id)


def add_players_to_team(request, team_id):
    if not request.user.is_authenticated or request.method != 'POST':
        return redirect('user_login')

    team = team_modify_authenticate(request, team_id)

    if "players[]" in request.POST:
        add_players_to_team_util(request.POST.getlist("players[]"), team, remove="remove" in request.POST)

    return redirect("team_modify", team_id=team_id)


def signup_user(request):
    if request.method == 'POST':
        form = forms.SignUpForm(request.POST)
        if form.is_valid():
            form.save()
            username = form.cleaned_data.get('username')
            raw_password = form.cleaned_data.get('password1')
            user = authenticate(username=username, password=raw_password)
            login(request, user)
            return redirect('team_administration')
    else:
        form = forms.SignUpForm()

    return render(request, 'registration/signup.html', {'form': form})


def logout_user(request):
    logout(request)
    return redirect('season_redirect')
