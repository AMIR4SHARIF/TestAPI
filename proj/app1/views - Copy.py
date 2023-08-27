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
    return JsonResponse({'k0':'v01'}, status=200)

## -------------------------------------------------
from rest_framework.decorators import api_view
from rest_framework.response import Response

from .serializers import UserSerializer
from rest_framework import status
from rest_framework.authtoken.models import Token
from django.contrib.auth.models import User
from django.shortcuts import get_object_or_404

@api_view(['POST'])
def signup(request):
    serializer = UserSerializer(data=request.data)
    if serializer.is_valid():
        serializer.save()
        print(serializer.data)
        user = User.objects.get(username=request.data['username'])
        user.set_password(request.data['password'])
        user.save()

        token = Token.objects.create(user=user)
        print(token, token.key)
        return Response({'token':token.key, 'user':serializer.data})
    
    print(serializer.errors)
    return Response(serializer.errors, status.HTTP_400_BAD_REQUEST)

@api_view(['POST'])
def login(request):
    user = get_object_or_404(User, username=request.data['username'])
    if not user.check_password(request.data['password']):
        return Response({"detail": "Not found."}, status=status.HTTP_404_NOT_FOUND)
    
    token, _ = Token.objects.get_or_create(user=user)
    print(user, token)
    serializer = UserSerializer(instance=user)
    # print(serializer.data)
    return Response({'token':token.key, 'user':serializer.data['username']})
    

from rest_framework.decorators import authentication_classes, permission_classes
from rest_framework.authentication import TokenAuthentication, SessionAuthentication
from rest_framework.permissions import IsAuthenticated

@api_view(['GET'])
@authentication_classes([TokenAuthentication, SessionAuthentication])
@permission_classes([IsAuthenticated])
def test_token(request):
    return Response({"status": "ok"})



@api_view(['GET', 'PUT', 'DELETE'])
@authentication_classes([TokenAuthentication, SessionAuthentication])
@permission_classes([IsAuthenticated])
def devices(request):
    if request.method == 'GET':
        d = request.user.devices.all()
        # print(d)
        if d.count() == 0:
            return Response({"devices": []})
        res = []
        for device in d:
            sensors = SensorsSerializer(device.sensors.first()).data
            relays = RelaysSerializer(device.relays.first()).data
            
            res.append({"uid":device.uid, "sensors":sensors, "relays":relays})
        return Response({"devices": res})
    
    elif request.method == 'PUT':
        try:
            data = JSONParser().parse(request)
        except:
            return Response({"devices": ''}, status=status.HTTP_400_BAD_REQUEST)
        
        uid = data.get('uid', None)
        if uid == None:
            return Response({"devices": 'uid key required'}, status=status.HTTP_400_BAD_REQUEST)
        
        user = request.user
        if user.devices.filter(uid=uid).exists():
            return Response({"devices": 'uid already exists'})

        device = Device(user=request.user, uid=uid)
        device.save()
        Sensors(device=device).save()
        Relays(device=device).save()
        
        return Response({"ststus": 'ok'})
    
    elif request.method == 'DELETE':
        try:
            data = JSONParser().parse(request)
        except:
            return Response({"devices": ''}, status=status.HTTP_400_BAD_REQUEST)
        
        uid = data.get('uid', None)
        # print(uid)
        if uid == None:
            return Response({"devices": 'uid key required'}, status=status.HTTP_400_BAD_REQUEST)
        
        user = request.user
        device = user.devices.filter(uid=uid)
        if not device.exists():
            return Response({"devices": 'uid not exists'}, status=status.HTTP_404_NOT_FOUND)
        # print(device)
        device.delete()

        return Response({"ststus": 'ok'})
    
# ------------------------------------------------
# @csrf_exempt
@api_view(['GET', 'POST'])
@authentication_classes([TokenAuthentication, SessionAuthentication])
@permission_classes([IsAuthenticated])
def sensors_detail(request, uid):
    
    try:
        # print('*** try')
        # # d = Device.objects.get(token=token)
        # # data = JSONParser().parse(request)
        # try:
        #     data = JSONParser().parse(request)
        # except:
        #     return Response({"devices": ''}, status=status.HTTP_400_BAD_REQUEST)
        # print('-'*30)
        # uid = data.get('uid', None)
        # print(f"{uid = }", uid == None)
        if uid == None:
            return Response({"devices": 'uid key required'}, status=status.HTTP_400_BAD_REQUEST)
        
        user = request.user
        print(user)
        device = user.devices.filter(uid=uid)
        if not device.exists():
            print('dev not exist')
            return Response({"devices": 'uid not exists.'}, status=status.HTTP_404_NOT_FOUND)
        d = device.first() # Device.objects.get(uid=123)
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
            # return JsonResponse(serializer.data, status=200)
            return JsonResponse({"ststus": 'ok'}, status=200)
        return JsonResponse(serializer.errors, status=400)

@api_view(['GET', 'POST'])
@authentication_classes([TokenAuthentication, SessionAuthentication])
@permission_classes([IsAuthenticated])
def relays_detail(request, uid):
    
    try:
        # d = Device.objects.get(token=token)
        # r = d.relays.first()

        if uid == None:
            return Response({"devices": 'uid key required'}, status=status.HTTP_400_BAD_REQUEST)
        
        user = request.user
        # print(user)
        device = user.devices.filter(uid=uid)
        if not device.exists():
            print('dev not exist')
            return Response({"devices": 'uid not exists.'}, status=status.HTTP_404_NOT_FOUND)
        d = device.first()
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
            # return JsonResponse(serializer.data, status=200)
            return JsonResponse({"ststus": 'ok'}, status=200)
        return JsonResponse(serializer.errors, status=400)

## -------------------------------------------------

# @csrf_exempt
# def sensors_detail(request, token):
    
#     try:
#         d = Device.objects.get(token=token)
#         s = d.sensors.first()
        
#     except Device.DoesNotExist:
#         return HttpResponse(status=404)
#     except Sensors.DoesNotExist:
#         return HttpResponse(status=404)

#     if request.method == 'GET':
#         serializer = SensorsSerializer(s)
#         return JsonResponse(serializer.data)

#     elif request.method == 'POST':
#         print("*-"*10)
#         data = JSONParser().parse(request)
#         serializer = SensorsSerializer(s, data=data)
#         if serializer.is_valid():
#             serializer.save()
#             return JsonResponse(serializer.data, status=200)
#         return JsonResponse(serializer.errors, status=400)

#     # elif request.method == 'PUT':
#     #     data = JSONParser().parse(request)
#     #     serializer = SnippetSerializer(snippet, data=data)
#     #     if serializer.is_valid():
#     #         serializer.save()
#     #         return JsonResponse(serializer.data)
#     #     return JsonResponse(serializer.errors, status=400)

#     # elif request.method == 'DELETE':
#     #     snippet.delete()
#     #     return HttpResponse(status=204)

# @csrf_exempt
# def relays_detail(request, token):
    
#     try:
#         d = Device.objects.get(token=token)
#         r = d.relays.first()
        
#     except Device.DoesNotExist:
#         return HttpResponse(status=404)
#     except Relays.DoesNotExist:
#         return HttpResponse(status=404)

#     if request.method == 'GET':
#         serializer = RelaysSerializer(r)
#         return JsonResponse(serializer.data)

#     elif request.method == 'POST':
#         print('relays:post')
#         data = JSONParser().parse(request)
#         serializer = RelaysSerializer(r, data=data)
#         if serializer.is_valid():
#             serializer.save()
#             print(serializer.data)
#             return JsonResponse(serializer.data, status=200)
#         return JsonResponse(serializer.errors, status=400)



