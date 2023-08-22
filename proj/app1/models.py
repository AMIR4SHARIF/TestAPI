from django.db import models
from django.contrib.auth.models import User

class Device(models.Model):
    uid = models.CharField(max_length=100, blank=True, default='')
    token = models.CharField(max_length=100, blank=True, default='')
    # user  devices
    user = models.ForeignKey(User, default=None, on_delete=models.CASCADE, related_name='devices')

    def __str__(self) -> str:
        return self.uid

class Sensors(models.Model):
    time = models.CharField(max_length=50, blank=True, default='')

    temp = models.FloatField(default=0.0, null=True)
    humid = models.FloatField(default=0.0, null=True)
    lpg = models.FloatField(default=0.0, null=True)
    smk = models.FloatField(default=0.0, null=True)
    
    # val1 = models.CharField(max_length=50, blank=True, default='')
    # val2 = models.CharField(max_length=50, blank=True, default='')

    device = models.ForeignKey(Device, on_delete=models.CASCADE, related_name='sensors')

    def __str__(self) -> str:
        return f"device: {self.device}"


class Relays(models.Model):
    rel1 = models.BooleanField(null=True, default=False)
    rel2 = models.BooleanField(null=True, default=False)
    rel3 = models.BooleanField(null=True, default=False)
    rel4 = models.BooleanField(null=True, default=False)

    device = models.ForeignKey(Device, on_delete=models.CASCADE, related_name='relays')

