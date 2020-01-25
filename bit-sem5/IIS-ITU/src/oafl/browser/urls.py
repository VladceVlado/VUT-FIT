from . import views
from .converters import SeasonConverter

from django.urls import path, register_converter
from django.contrib.auth.views import LoginView


register_converter(SeasonConverter, 'yyyy-xxxx')

urlpatterns = [
    path('', views.season_redirect, name='season_redirect'),
    path('<yyyy-xxxx:season>/', views.index, name='index'),
    path('<yyyy-xxxx:season>/rozpis/', views.schedule, name='schedule'),
    path('<yyyy-xxxx:season>/vysledky/', views.results, name='results'),
    path('<yyyy-xxxx:season>/tabulka/', views.table, name='table'),
    path('<yyyy-xxxx:season>/tymy/', views.teams, name='teams'),
    path('<yyyy-xxxx:season>/statistiky/', views.stats, name='stats'),
    path('<yyyy-xxxx:season>/play-off/', views.spider, name='play-off'),
    path('registrace-tymu/', views.team_administration, name='team_administration'),
    path('registrace-tymu/prihlaseni/', LoginView.as_view(), name='user_login'),
    path('registrace-tymu/registrace/', views.signup_user, name='user_signup'),
    path('registrace-tymu/odhlaseni/', views.logout_user, name='logout_user'),
    path('registrace-tymu/registrace-hrace/', views.player_registration, name='player_registration'),
    path('registrace-tymu/registrace-tymu/', views.team_registration, name='team_registration'),
    path('registrace-tymu/uprava-tymu/<int:team_id>/', views.team_modify, name='team_modify'),
    path('registrace-tymu/uprava-tymu/<int:team_id>/prejmenovat/', views.team_rename, name='team_rename'),
    path('registrace-tymu/uprava-tymu/<int:team_id>/registrovat_sezonu/', views.register_team_season, name='register_team_season'),
    path('registrace-tymu/uprava-tymu/<int:team_id>/pridat_hrace/', views.add_players_to_team, name='add_players_to_team'),
]
