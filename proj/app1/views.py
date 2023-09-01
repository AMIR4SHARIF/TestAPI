from django.shortcuts import render

from django.http import HttpResponse, JsonResponse
from django.views.decorators.csrf import csrf_exempt
from rest_framework.parsers import JSONParser

# Create your views here.

from .models import Device#, Sensors, Relays
# from .serializers import SensorsSerializer, RelaysSerializer

from pathlib import Path
# $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
import json
from dataclasses import dataclass, field

from .my_data_classes import *
# SENSORS_FILE_NAME = 'sensors.json'


# 4$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

VALID_VERSIONS = ['1']
def is_valid_version(v):
    return v in VALID_VERSIONS

DATA_DIR = Path('media/')

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

        # print(DATA_DIR, DATA_DIR.resolve())
        user_dir = DATA_DIR / user.username
        # print(user_dir)
        user_dir.mkdir()

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
    qq = Response({'token':token.key, 'user':serializer.data['username']})
    return qq
    

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
            # print(d)
            # device_dir = DATA_DIR / request.user.username / device.uid
            device_dir = DATA_DIR / request.user.username / f"{device.version}-{device.uid}"
            sensors = Sensors(path=device_dir).read()
            # print('-------', device_dir, Relays(path=device_dir).read())
            relays = Relays(path=device_dir).read()
            
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
        
        version = data.get('version', None)
        if version == None:
            return Response({"devices": 'version key required'}, status=status.HTTP_400_BAD_REQUEST)
        # print(version)
        user = request.user
        if user.devices.filter(uid=uid, version=version).exists():
            return Response({"devices": 'uid already exists'})
        
        if not is_valid_version(version):
            return Response({"devices": 'version is not valid'})

        device = Device(user=request.user, uid=uid, version=version)
        device.save()
        
        device_dir = DATA_DIR / user.username / f"{version}-{uid}"
        device_dir.mkdir()
        Sensors(path=device_dir).save()
        Relays(path=device_dir).save()

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
        version = data.get('version', None)
        if version == None:
            return Response({"devices": 'version key required'}, status=status.HTTP_400_BAD_REQUEST)
        
        user = request.user
        device = user.devices.filter(uid=uid)
        if not device.exists():
            return Response({"devices": 'uid not exists'}, status=status.HTTP_404_NOT_FOUND)
        
        if not is_valid_version(version):
            return Response({"devices": 'version is not valid'})
        
        # print(device)
        device.delete()

        # device_dir = DATA_DIR / user.username / uid
        device_dir = DATA_DIR / user.username / f"{version}-{uid}"
        sensors_file = device_dir / SENSORS_FILE_NAME
        sensors_file.unlink()
        relays_file = device_dir / RELAYS_FILE_NAME
        relays_file.unlink()
        device_dir.rmdir()

        return Response({"ststus": 'ok'})
    
# ------------------------------------------------
# @csrf_exempt
@api_view(['GET', 'POST'])
@authentication_classes([TokenAuthentication, SessionAuthentication])
@permission_classes([IsAuthenticated])
def sensors_detail(request, version, uid):
    
    try:
        if uid == None:
            return Response({"devices": 'uid  required'}, status=status.HTTP_400_BAD_REQUEST)
        
        if not is_valid_version(version):
            return Response({"devices": 'version is not valid'}, status=status.HTTP_400_BAD_REQUEST)
        
        user = request.user
        print(user)
        device = user.devices.filter(uid=uid, version=version)
        if not device.exists():
            print('dev not exist')
            return Response({"devices": 'uid not exists.'}, status=status.HTTP_404_NOT_FOUND)
        d = device.first() # Device.objects.get(uid=123)
        # s = d.sensors.first()
        device_dir = DATA_DIR / user.username / f"{version}-{uid}"
        # s = Sensors(path=device_dir).read()
        
    except Device.DoesNotExist:
        return HttpResponse(status=404)
    

    if request.method == 'GET':
        if version == '1':
            s = Sensors(path=device_dir)
            data = s.read()
        return JsonResponse(data)

    elif request.method == 'POST':
        print("*-"*10)
        data = JSONParser().parse(request)
        if version == '1':
            try:
                s = Sensors(path=device_dir, data=data)
            except:
                return JsonResponse({'detail':'invalid keys'}, status=400)
        print(s)
        is_valid, err_resp = s.is_valid()
        print(is_valid, err_resp)
        if is_valid:
            s.save()
            return JsonResponse({"ststus": 'ok'}, status=200)
        return JsonResponse(err_resp, status=400)

@api_view(['GET', 'POST'])
@authentication_classes([TokenAuthentication, SessionAuthentication])
@permission_classes([IsAuthenticated])
def relays_detail(request, version, uid):
    
    try:
        if uid == None:
            return Response({"devices": 'uid  required'}, status=status.HTTP_400_BAD_REQUEST)
        
        if not is_valid_version(version):
            return Response({"devices": 'version is not valid'}, status=status.HTTP_400_BAD_REQUEST)
        
        user = request.user
        print(user)
        device = user.devices.filter(uid=uid, version=version)
        if not device.exists():
            print('dev not exist')
            return Response({"devices": 'uid not exists.'}, status=status.HTTP_404_NOT_FOUND)
        d = device.first() # Device.objects.get(uid=123)
        # s = d.sensors.first()
        device_dir = DATA_DIR / user.username / f"{version}-{uid}"
        # s = Sensors(path=device_dir).read()
        
    except Device.DoesNotExist:
        return HttpResponse(status=404)
    

    if request.method == 'GET':
        if version == '1':
            r = Relays(path=device_dir)
            print(r)
            data = r.read()
            # data = {1:2}
        return JsonResponse(data)

    elif request.method == 'POST':
        print("*-"*10)
        data = JSONParser().parse(request)
        if version == '1':
            try:
                r = Relays(path=device_dir, data=data)
            except:
                return JsonResponse({'detail':'invalid keys'}, status=400)
        print(r)
        is_valid, err_resp = r.is_valid()
        print(is_valid, err_resp)
        if is_valid:
            r.save()
            return JsonResponse({"ststus": 'ok'}, status=200)
        return JsonResponse(err_resp, status=400)
    
# @api_view(['GET', 'POST'])
# @authentication_classes([TokenAuthentication, SessionAuthentication])
# @permission_classes([IsAuthenticated])
# def relays_detail(request, uid):
    
#     try:
#         # d = Device.objects.get(token=token)
#         # r = d.relays.first()

#         if uid == None:
#             return Response({"devices": 'uid key required'}, status=status.HTTP_400_BAD_REQUEST)
        
#         user = request.user
#         # print(user)
#         device = user.devices.filter(uid=uid)
#         if not device.exists():
#             print('dev not exist')
#             return Response({"devices": 'uid not exists.'}, status=status.HTTP_404_NOT_FOUND)
#         d = device.first()
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
#             # return JsonResponse(serializer.data, status=200)
#             return JsonResponse({"ststus": 'ok'}, status=200)
#         return JsonResponse(serializer.errors, status=400)

