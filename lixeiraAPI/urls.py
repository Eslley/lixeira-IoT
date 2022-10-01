from django.urls import path
from lixeiraAPI.views import test

urlpatterns = [
    path('', test, name='test'),
]