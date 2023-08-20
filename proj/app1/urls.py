from django.urls import path
from . import views

urlpatterns = [
    # path('', views.index, name='index'),
    path('test/', views.test, name='test_json'),

    path('api/<str:token>/sensors/', views.sensors_detail, name='sensors'),
    path('api/<str:token>/relays/', views.relays_detail),
]
