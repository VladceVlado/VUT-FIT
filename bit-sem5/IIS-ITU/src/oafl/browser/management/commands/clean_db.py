from django.core.management.base import BaseCommand
from django.contrib.auth.models import User

from oafl.browser.models import Season, Player, Penalty, Round, Match, Goal, Team, SeasonTeam


class Command(BaseCommand):
    help = 'Delete data in tables'

    def handle(self, *args, **options):
        for _class in [Season, Player, Penalty, Round, Match, Goal, Team, SeasonTeam]:
            _class.objects.all().delete()
        User.objects.filter(is_superuser=False).delete()
