from django.shortcuts import render

def index(request):
    return render(request, 'tela_inicial.html')