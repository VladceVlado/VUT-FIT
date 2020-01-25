from django.core.management.base import BaseCommand
from django.core.management import call_command


class Command(BaseCommand):
    help = 'Fill database tables with some data'

    def handle(self, *args, **options):
        call_command('clean_db')
        for json_data in ['contenttype', 'session', 'permission', 'user', 'seasons',
                          'teams', 'players', 'rounds', 'matches', 'goals', 'penalties']:
            call_command('loaddata', json_data)
