from dataclasses import field
from rest_framework import serializers

from lixeiraAPI.models import PessoasModel

class PessoasSerializer(serializers.ModelSerializer):

    class Meta:
        model = PessoasModel
        fields = ['id','nome']