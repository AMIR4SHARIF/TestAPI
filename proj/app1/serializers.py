from rest_framework import serializers
# from .models import Snippet, LANGUAGE_CHOICES, STYLE_CHOICES
from .models import Sensors, Relays

    
class SensorsSerializer(serializers.Serializer):
    id = serializers.IntegerField(read_only=True)

    time = serializers.CharField(required=False, allow_blank=True, max_length=50)

    temp = serializers.FloatField()# default=None
    humid = serializers.FloatField()
    lpg = serializers.FloatField()
    smk = serializers.FloatField()
 
    # title = serializers.CharField(required=False, allow_blank=True, max_length=100)
    # code = serializers.CharField(style={'base_template': 'textarea.html'})
    # linenos = serializers.BooleanField(required=False)
    # language = serializers.ChoiceField(choices=LANGUAGE_CHOICES, default='python')
    # style = serializers.ChoiceField(choices=STYLE_CHOICES, default='friendly')

    def create(self, validated_data):
        """
        Create and return a new `Snippet` instance, given the validated data.
        """
        return Sensors.objects.create(**validated_data)

    def update(self, instance, validated_data):
        """
        Update and return an existing `Snippet` instance, given the validated data.
        """
        instance.time = validated_data.get('time' , 'None')
        instance.temp = validated_data.get('temp', -1.1)
        instance.humid = validated_data.get('humid', -1.1)
        instance.lpg = validated_data.get('lpg', -1.1)
        instance.smk = validated_data.get('smk', -1.1)
        instance.save()
        return instance
    
class RelaysSerializer(serializers.Serializer):
    id = serializers.IntegerField(read_only=True)

    rel1 = serializers.BooleanField()
    rel2 = serializers.BooleanField()
    rel3 = serializers.BooleanField()
    rel4 = serializers.BooleanField()
    
    def create(self, validated_data):
        return Relays.objects.create(**validated_data)

    def update(self, instance, validated_data):
        
        instance.rel1 = validated_data.get('rel1', 'False')
        instance.rel2 = validated_data.get('rel2', 'False')
        instance.rel3 = validated_data.get('rel3', 'False')
        instance.rel4 = validated_data.get('rel4', 'False')

        instance.save()
        return instance