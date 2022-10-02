from rest_framework.response import Response
from rest_framework import status
from rest_framework.decorators import api_view
from lixeiraAPI.models import PessoasModel

from lixeiraAPI.serializers import PessoasSerializer

@api_view(['GET'])
def pessoasList(request):
    pessoas = PessoasModel.objects.all()
    serializer = PessoasSerializer(pessoas, many=True)
    
    return Response(serializer.data, status=status.HTTP_200_OK)

@api_view(['GET'])
def pessoaDetail(request, pk):
    try:
        pessoa = PessoasModel.objects.get(id=pk)
        serializer = PessoasSerializer(pessoa)

        return Response(serializer.data, status=status.HTTP_200_OK)
    except PessoasModel.DoesNotExist:
        return Response({'message': 'Pessoa não encontrada'}, status=status.HTTP_404_NOT_FOUND)

@api_view(['POST'])
def pessoaCreate(request):
    serializer = PessoasSerializer(data=request.data)

    if serializer.is_valid():
        serializer.save()

        return Response({'message': 'Pessoa cadastrada com sucesso'}, status=status.HTTP_201_CREATED)

    return Response({'message': 'Erro ao cadastrar', 'errors': serializer.errors}, status=status.HTTP_400_BAD_REQUEST)

@api_view(['DELETE'])
def pessoaDelete(request, pk):
    try:
        pessoa = PessoasModel.objects.get(id=pk)
        pessoa.delete()

        return Response({'message': 'Pessoa deletada com sucesso'}, status=status.HTTP_200_OK)
    except PessoasModel.DoesNotExist:
        return Response({'message': 'Pessoa não encontrada'}, status=status.HTTP_404_NOT_FOUND)