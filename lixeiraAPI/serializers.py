from rest_framework import serializers

from lixeiraAPI.models import PessoasModel

class PessoasSerializer(serializers.ModelSerializer):

    class Meta:
        model = PessoasModel
        fields = ['id','nome']


class LixeirasSerializer(serializers.Serializer):

    id = serializers.IntegerField()
    estaAberta = serializers.BooleanField()
    nivel = serializers.IntegerField()

    def validate(self, data):

        if 'id' not in data:
            raise serializers.ValidationError('Informar campo id')

        if 'estaAberta' not in data:
            raise serializers.ValidationError('Informar campo estaAberta')

        if 'nivel' not in data:
            raise serializers.ValidationError('Informar campo nível')

        return data