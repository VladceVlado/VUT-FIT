from .models import Player, Team, SeasonTeam

from django import forms
from django.forms import ModelForm
from django.contrib.auth.forms import UserCreationForm
from django.contrib.auth.models import User


class DateInput(forms.DateInput):
    input_type = 'date'


class SignUpForm(UserCreationForm):
    username = forms.CharField(max_length=30, required=False, label='Uživatelské jméno')
    first_name = forms.CharField(max_length=30, required=False, help_text='Volitelné.', label='Jméno')
    last_name = forms.CharField(max_length=30, required=False, help_text='Volitelné.', label='Příjmení')
    email = forms.EmailField(max_length=254, help_text='Povinné.')

    class Meta:
        model = User
        fields = ('username', 'first_name', 'last_name', 'email', 'password1', 'password2', )


class PlayerForm(ModelForm):
    class Meta:
        model = Player
        fields = ('first_name', 'last_name', 'birth_date', 'position', 'dress_number')
        labels = {
            'first_name': 'Jméno',
            'last_name': 'Příjmení',
            'birth_date': 'Datum narození',
            'position': 'Pozice',
            'dress_number': 'Číslo dresu'
        }
        widgets = {
            'birth_date': DateInput()
        }


class TeamForm(ModelForm):
    class Meta:
        model = Team
        fields = ('name', )
        labels = {
            'name': 'Jméno týmu'
        }


class SeasonTeamForm(ModelForm):
    class Meta:
        model = SeasonTeam
        fields = ('season', 'team', )
