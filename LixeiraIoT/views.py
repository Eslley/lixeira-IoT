from django.shortcuts import render

def index(request):
    return render(request, 'tela_inicial.html')

def dashboard(request):
    return render(request, 'dashboard.html')

def lixeiras(request):
    return render(request,'lixeiras.html')