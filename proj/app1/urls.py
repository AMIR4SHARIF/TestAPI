from django.urls import path
from . import views

urlpatterns = [
    # path('', views.index, name='index'),
    # path('', views.index, name='index'),
    path('test/', views.test, name='test_json'),

    # path('api/<str:token>/sensors/', views.sensors_detail, name='sensors'),
    path('api/<str:version>/<str:uid>/sensors/', views.sensors_detail, name='sensors'),
    # path('api/<str:token>/relays/', views.relays_detail),
    path('api/<str:version>/<str:uid>/relays/', views.relays_detail),

    path('signup/', views.signup),
    path('login/', views.login),
    path('test_token/', views.test_token),

    path('devices/', views.devices),
]
