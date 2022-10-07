from django.db import models

class PessoasModel(models.Model):

    class Meta:
        db_table = 'pessoas'

    nome = models.CharField(max_length=255)
    foiUltimo = models.BooleanField(default=False)

    def __str__(self):
        return self.nome

class LixeirasModel(models.Model):

    class Meta:
        db_table = 'lixeiras'

    estaAberta = models.BooleanField()
    nivel = models.IntegerField()

    def __str__(self):
        return self.id