"""
URL configuration for myFarm project.

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/4.2/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path
from farm.views import dashboard1,dashboard2,dashboard3,dashboard4,dashboard5,control_pump1,control_pump2 ,get_sensor_data,get_humidite_sol_1,get_dashboard2_data,get_dashboard3_data 

urlpatterns = [
    path('admin/', admin.site.urls),
    path('dashboard1/', dashboard1,name='dashboard1'),
    path('get_sensor_data/', get_sensor_data, name='get_sensor_data'),
    path('get_humidite_sol_1/', get_humidite_sol_1, name='get_humidite_sol_1'),
    path('control-pump1/', control_pump1, name='control_pump1'),
    path('dashboard2/', dashboard2,name='dashboard2'),
    path('get_dashboard2_data/', get_dashboard2_data, name='get_dashboard2_data'),
    path('control-pump2/', control_pump2, name='control_pump2'),
    path('dashboard3/', dashboard3,name='dashboard3'),
    path('get_dashboard3_data/', get_dashboard3_data, name='get_dashboard3_data'),
    path('dashboard4/', dashboard4,name='dashboard4'),
    path('dashboard5/', dashboard5,name='dashboard5'),
   
]
