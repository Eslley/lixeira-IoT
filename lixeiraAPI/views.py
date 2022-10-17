from rest_framework.response import Response
from rest_framework import status
from rest_framework.decorators import api_view
from lixeiraAPI.models import LixeirasModel, PessoasModel
import re
from lixeiraAPI.serializers import LixeirasSerializer, PessoasSerializer
symbols_dict = {'>':'__gt','>e':'__gte','<':'__lt','<e':'__lte'}
params_regex = re.compile(r'\[(.*)\]')


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

@api_view(['GET'])
def lixeirasList(request):
    query = dict()
    for k,v in request.query_params.items():
        condition = re.search(params_regex,k)
        if condition:
            condition = condition.groups(0)[0]
            query[re.sub(params_regex,'',k)+symbols_dict[condition]] = request.query_params.get(k)
        else:
            query[k]=request.query_params.get(k)
    lixeiras = LixeirasModel.objects.filter(**query)
    serializer = LixeirasSerializer(lixeiras, many=True)

    return Response(serializer.data, status=status.HTTP_200_OK)

@api_view(['PUT'])
def lixeiraUpdate(request):
    try:
        
        serializer = LixeirasSerializer(data=request.data)

        if serializer.is_valid():
            lixeira = LixeirasModel.objects.filter(id=serializer.data['id'])

            if lixeira.count():
                lixeira = lixeira.get(id=serializer.data['id'])
                lixeira.estaAberta = serializer.data['estaAberta']

                lixeira.nivel = serializer.data['nivel']

                lixeira.save(update_fields=['estaAberta', 'nivel'])

                return Response({'message': 'Dados da lixeira foram atualizados com sucesso'}, status=status.HTTP_200_OK)
            else:
                lixeira = LixeirasModel(estaAberta=serializer.data['estaAberta'], nivel=serializer.data['nivel'])

                lixeira.save()

                return Response({'message': 'Lixeira adicionada com sucesso', 'id': lixeira.id}, status=status.HTTP_200_OK)

        return Response({'message': 'Erro ao atualizar', 'errors': serializer.errors}, status=status.HTTP_400_BAD_REQUEST)
        
    except Exception as e:
        return Response({'message': 'Erro interno', 'error': e}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


@api_view(['DELETE'])
def lixeiraDelete(request, pk):
    try:
        lixeira = LixeirasModel.objects.get(id=pk)
        lixeira.delete()

        return Response({'message': 'Lixeira deletada com sucesso'}, status=status.HTTP_200_OK)
    except LixeirasModel.DoesNotExist:
        return Response({'message': 'Lixeira não encontrada'}, status=status.HTTP_404_NOT_FOUND)