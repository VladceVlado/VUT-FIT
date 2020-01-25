from . import utils

from django.db import models
from django.core.validators import MaxValueValidator, MinValueValidator
from django.contrib.auth.models import User


class Team(models.Model):
    name = models.CharField(max_length=50, unique=True)
    user = models.ForeignKey(null=True, on_delete=models.SET_NULL, to=User)

    def __str__(self):
        return self.name


class Season(models.Model):
    begin_date = models.DateField(unique=True, null=True)
    end_date = models.DateField(unique=True, null=True)
    teams = models.ManyToManyField(Team, through='SeasonTeam')

    def __str__(self):
        return '{}-{}'.format(self.begin_date.year, self.end_date.year)

    def is_current(self):
        return self == utils.get_current_season()


class SeasonTeam(models.Model):
    team = models.ForeignKey(Team, on_delete=models.CASCADE)
    season = models.ForeignKey(Season, on_delete=models.CASCADE)
    final_place = models.PositiveSmallIntegerField(null=True, blank=True)
    registered = models.BooleanField(default=False)

    class Meta:
        unique_together = ('team', 'season')
        verbose_name = 'Team'
        verbose_name_plural = 'Teams'

    def __str__(self):
        return '{} ({})'.format(self.team, self.season)


class Round(models.Model):
    GROUP_PHASE = 'GP'
    EIGHT_FINAL = 'EF'
    QUATERFINAL = 'QF'
    SEMIFINAL = 'SF'
    FINAL = 'F'
    TYPE_CHOICES = (
        (GROUP_PHASE, 'Group phase'),
        (EIGHT_FINAL, 'Eight-final'),
        (QUATERFINAL, 'Quaterfinal'),
        (SEMIFINAL, 'Semifinal'),
        (FINAL, 'Final')
    )

    number = models.PositiveSmallIntegerField(validators=[MinValueValidator(1), MaxValueValidator(999)], null=True)
    type = models.CharField(max_length=2, choices=TYPE_CHOICES, null=True)
    season = models.ForeignKey(Season, on_delete=models.CASCADE)

    class Meta:
        unique_together = ('number', 'season')

    def __str__(self):
        return '{}. round in {}'.format(self.number, self.season)


class Player(models.Model):
    GOALKEEPER = "G"
    DEFENDER = "D"
    ATTACKER = "A"
    POSITION_CHOICES = (
        (GOALKEEPER, 'Goalkeeper'),
        (DEFENDER, 'Defender'),
        (ATTACKER, 'Attacker'),
    )

    first_name = models.CharField(max_length=50)
    last_name = models.CharField(max_length=50)
    birth_date = models.DateField()
    position = models.CharField(max_length=1, choices=POSITION_CHOICES, null=True)
    dress_number = models.PositiveSmallIntegerField(validators=[MinValueValidator(1), MaxValueValidator(99)])
    team_season = models.ManyToManyField(SeasonTeam, related_name='teams')

    # ToDo:
    # class Meta:
    #     unique_together = ('dress_number', 'team_season')

    def __str__(self):
        return '{} {}'.format(self.last_name, self.first_name)


class Match(models.Model):
    number = models.PositiveSmallIntegerField(validators=[MinValueValidator(1), MaxValueValidator(9999)], null=True)
    date_time = models.DateTimeField(unique=True, null=True)
    played = models.BooleanField(default=False)
    home_team = models.ForeignKey(Team, on_delete=models.CASCADE, related_name='match_home_team')
    away_team = models.ForeignKey(Team, on_delete=models.CASCADE, related_name='match_away_team')
    round = models.ForeignKey(Round, on_delete=models.CASCADE)
    home_team_players = models.ManyToManyField(Player, related_name='home_team_players')
    away_team_players = models.ManyToManyField(Player, related_name='away_team_players')

    class Meta:
        verbose_name_plural = "Matches"

    def __str__(self):
        return '{}-{} | {} vs {}'.format(self.number, self.round.season, self.home_team, self.away_team)


class Goal(models.Model):
    time = models.TimeField()
    match = models.ForeignKey(Match, on_delete=models.CASCADE)
    assist_player = models.ForeignKey(Player, on_delete=models.CASCADE, null=True, blank=True,
                                      related_name='goal_assist_player')
    score_player = models.ForeignKey(Player, on_delete=models.CASCADE, related_name='goal_score_player')
    collect_team = models.ForeignKey(Team, on_delete=models.CASCADE, related_name='goal_collect_team')
    score_team = models.ForeignKey(Team, on_delete=models.CASCADE, related_name='goal_score_team')

    def __str__(self):
        return '{} ({}, {}) scores to {}'.format(self.score_team, self.score_player, self.assist_player,
                                                 self.collect_team)


class Penalty(models.Model):
    SHORT_PENALTY = 2
    LONG_PENALTY = 5
    LENGTH_CHOICES = (
        (SHORT_PENALTY, '2 minutes'),
        (LONG_PENALTY, '5 minutes'),
    )

    length = models.PositiveSmallIntegerField(choices=LENGTH_CHOICES, null=True)
    time = models.TimeField()
    match = models.ForeignKey(Match, on_delete=models.CASCADE)
    player = models.ForeignKey(Player, on_delete=models.CASCADE)

    class Meta:
        verbose_name_plural = "Penalties"

    def __str__(self):
        return '{} - {} minutes penalty in {}'.format(self.player, self.length, self.match)
