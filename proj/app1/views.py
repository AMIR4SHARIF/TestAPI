from django.shortcuts import render

from django.http import HttpResponse, JsonResponse
from django.views.decorators.csrf import csrf_exempt
from rest_framework.parsers import JSONParser

# Create your views here.

from .models import Device, Sensors, Relays
from .serializers import SensorsSerializer, RelaysSerializer

def index(request):
    return render(request, 'app1/index.html')
    # return HttpResponse('hello')

def test(request):
    return JsonResponse({'k0':'v0'}, status=200)


@csrf_exempt
def sensors_detail(request, token):
    
    try:
        d = Device.objects.get(token=token)
        s = d.sensors.first()
        
    except Device.DoesNotExist:
        return HttpResponse(status=404)
    except Sensors.DoesNotExist:
        return HttpResponse(status=404)

    if request.method == 'GET':
        serializer = SensorsSerializer(s)
        return JsonResponse(serializer.data)

    elif request.method == 'POST':
        print("*-"*10)
        data = JSONParser().parse(request)
        serializer = SensorsSerializer(s, data=data)
        if serializer.is_valid():
            serializer.save()
            return JsonResponse(serializer.data, status=200)
        return JsonResponse(serializer.errors, status=400)

    # elif request.method == 'PUT':
    #     data = JSONParser().parse(request)
    #     serializer = SnippetSerializer(snippet, data=data)
    #     if serializer.is_valid():
    #         serializer.save()
    #         return JsonResponse(serializer.data)
    #     return JsonResponse(serializer.errors, status=400)

    # elif request.method == 'DELETE':
    #     snippet.delete()
    #     return HttpResponse(status=204)

@csrf_exempt
def relays_detail(request, token):
    
    try:
        d = Device.objects.get(token=token)
        r = d.relays.first()
        
    except Device.DoesNotExist:
        return HttpResponse(status=404)
    except Relays.DoesNotExist:
        return HttpResponse(status=404)

    if request.method == 'GET':
        serializer = RelaysSerializer(r)
        return JsonResponse(serializer.data)

    elif request.method == 'POST':
        print('relays:post')
        data = JSONParser().parse(request)
        serializer = RelaysSerializer(r, data=data)
        if serializer.is_valid():
            serializer.save()
            print(serializer.data)
            return JsonResponse(serializer.data, status=200)
        return JsonResponse(serializer.errors, status=400)



