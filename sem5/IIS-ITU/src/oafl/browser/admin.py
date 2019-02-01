from oafl.browser.models import Season, Player, Penalty, Round, Match, Goal, SeasonTeam

from django.contrib import admin


class TeamInline(admin.TabularInline):
    model = Season.teams.through
    verbose_name = 'Team'
    verbose_name_plural = 'Teams'


class SeasonTeamInline(admin.TabularInline):
    model = Player.team_season.through
    verbose_name = 'Season-Team'
    verbose_name_plural = 'Season-Team'


class PlayerInline(admin.TabularInline):
    model = Player.team_season.through
    verbose_name = 'Player'
    verbose_name_plural = 'Players'


class MatchInline(admin.TabularInline):
    model = Match


class GoalInline(admin.TabularInline):
    model = Goal


class PenaltyInline(admin.TabularInline):
    model = Penalty


@admin.register(Season)
class SeasonAdmin(admin.ModelAdmin):
    ordering = ('-begin_date',)
    inlines = (TeamInline,)


@admin.register(Round)
class RoundAdmin(admin.ModelAdmin):
    ordering = ('-season', '-number',)
    inlines = (MatchInline,)


@admin.register(Match)
class MatchAdmin(admin.ModelAdmin):
    ordering = ('-round', '-date_time',)
    date_hierarchy = 'date_time'
    inlines = (GoalInline, PenaltyInline,)


@admin.register(SeasonTeam)
class SeasonTeamAdmin(admin.ModelAdmin):
    ordering = ('-season', 'team__name')
    inlines = (PlayerInline,)


@admin.register(Player)
class PlayerAdmin(admin.ModelAdmin):
    ordering = ('last_name', 'first_name',)
    exclude = ('team_season',)
    inlines = (SeasonTeamInline,)
