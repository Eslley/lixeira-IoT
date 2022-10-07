from django.urls import path
from lixeiraAPI.views import *

urlpatterns = [
    path('pessoas/create/', pessoaCreate, name='pessoa-create'),
    path('pessoas/', pessoasList, name='pessoas-list'),
    path('pessoas/details/<str:pk>/', pessoaDetail, name='pessoas-detail'),
    path('pessoas/delete/<str:pk>/', pessoaDelete, name='pessoas-delete'),
    path('lixeiras/update/', lixeiraUpdate, name='lixeiras-update'),
    path('lixeiras/', lixeirasList, name='lixeiras-list'),
]