from rest_framework.test import APITestCase
from django.urls import reverse
from rest_framework import status

from lixeiraAPI.models import PessoasModel

class TestView(APITestCase):
    
    def test_adicionar_pessoa(self):
        url = reverse('pessoa-create')

        data = {"nome": "nome teste"}

        response = self.client.post(url, data, format='json')

        self.assertEqual(response.status_code, status.HTTP_201_CREATED)


    def test_recuperar_pessoa(self):
        pessoa = PessoasModel.objects.create(nome="Nome Teste")

        ultimo = PessoasModel.objects.last()
        
        self.assertEqual(pessoa.id, ultimo.id)
        self.assertEqual(pessoa.nome, ultimo.nome)