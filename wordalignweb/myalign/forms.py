from django import forms

class AlignForm(forms.Form):
    a = forms.IntegerField()
    b = forms.IntegerField()
    