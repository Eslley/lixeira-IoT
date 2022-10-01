from rest_framework.response import Response
from rest_framework import status
from rest_framework.decorators import api_view

@api_view(['GET'])
def test(request):
    return Response({'message': 'Está Funcionando!'}, status=status.HTTP_200_OK)