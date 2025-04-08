from django.shortcuts import render,redirect
import plotly.express as px
from django.http import HttpResponse
import pandas as pd
from .sensor_data import SENSOR_DATA
from .mqtt_client import client
from django.http import JsonResponse
import json
from datetime import datetime
# Create your views here.

def get_sensor_data(request):
    return JsonResponse(SENSOR_DATA)

#FOR DASHBOARD 1 (PLANTE)
def dashboard1(request):
    humidite_sol_1 = SENSOR_DATA.get('humidite_sol_1', [])

    if not humidite_sol_1:
        humidite_sol_1 = []

    # Convertir les données en dictionnaire (heure: valeur)
    minute_data = {data["time"]: data["value"] for data in humidite_sol_1}

    # Obtenir la dernière valeur (ou 0 si aucune donnée)
    latest_value = humidite_sol_1[-1]["value"] if humidite_sol_1 else 0

    context = {
        "times": json.dumps(list(minute_data.keys())),
        "values": json.dumps(list(minute_data.values())),
        "humidite_sol_1": latest_value  # Ajouter la dernière valeur
    }

    print("Times:", context["times"])  
    print("Values:", context["values"])
    print("Latest Humidity:", context["humidite_sol_1"])

    return render(request, 'dashboard1.html', context)

def get_humidite_sol_1(request):
    humidite_sol_1 = SENSOR_DATA.get('humidite_sol_1', [])
    times = [data["time"] for data in humidite_sol_1]
    values = [data["value"] for data in humidite_sol_1]
    return JsonResponse({"times": times, "values": values})

def control_pump1(request):
    if request.method == 'POST':
        action = request.POST.get('action')  # expect values: 'activate' or 'deactivate'
        # Publish an MQTT message to control pump 1.
        client.publish('control/pump1', action)
        SENSOR_DATA['pompe_eau_1'] = action  # update locally
    return redirect('dashboard1')

#FOR DASHBOARD 2 (SERRRE)
def dashboard2(request):
    co2_data = SENSOR_DATA.get('co2', [])
    temp_hum_data = SENSOR_DATA.get('dht11_1', [])
    humidite_sol_2_data = SENSOR_DATA.get('humidite_sol_2', [])

    def extract_latest_value(data, key, default=0):
        return data[-1] if data else default

    # Préparer les données pour les graphiques
    context = {
        "times_co2": json.dumps([d["time"] for d in co2_data]),
        "values_co2": json.dumps([d["value"] for d in co2_data]),

        "times_temp_hum": json.dumps([d["time"] for d in temp_hum_data]),
        "values_temp": json.dumps([d["temperature"] for d in temp_hum_data]),
        "values_hum": json.dumps([d["humidity"] for d in temp_hum_data]),

        "times_humidite_sol_2": json.dumps([d["time"] for d in humidite_sol_2_data]),
        "values_humidite_sol_2": json.dumps([d["value"] for d in humidite_sol_2_data]),

        "co2_latest": extract_latest_value(co2_data, "value"),
        "temp_latest": extract_latest_value(temp_hum_data, "temperature"),
        "hum_latest": extract_latest_value(temp_hum_data, "humidity"),
        "humidite_sol_2_latest": extract_latest_value(humidite_sol_2_data, "value"),
    }

    return render(request, 'dashboard2.html', context)


def get_dashboard2_data(request):
    latest_temp_hum = SENSOR_DATA.get("temperature_humidity", [])

    return JsonResponse({
        "co2_latest": SENSOR_DATA["co2"][-1]["value"] if SENSOR_DATA["co2"] else 0,
        "temp_latest": latest_temp_hum[-1]["temperature"] if latest_temp_hum else 0,
        "hum_latest": latest_temp_hum[-1]["humidity"] if latest_temp_hum else 0,
        "humidite_sol_2_latest": SENSOR_DATA["humidite_sol_2"][-1]["value"] if SENSOR_DATA["humidite_sol_2"] else 0
    })

def control_pump2(request):
    if request.method == 'POST':
        action = request.POST.get('action')  # 'activate' or 'deactivate'
        client.publish('control/pump2', action)
        SENSOR_DATA['pompe_eau_2'] = action
    return redirect('dashboard2')

#FOR DASHBOARD 3 (ETABLE)
def dashboard3(request):
    # Get DHT11_2 data
    temp_hum_data = SENSOR_DATA.get('dht11_2', [])

    
    # Get current date and time
    current_date = datetime.now().strftime("%d %b %Y")
    
    # Get time of day
    hour = datetime.now().hour
    if hour >= 5 and hour < 12:
        time_of_day = "Morning"
    elif hour >= 12 and hour < 18:
        time_of_day = "Afternoon"
    else:
        time_of_day = "Evening"
    
    # Get latest flame detection status
    flame_status = SENSOR_DATA.get('flamme', 'Non Detecte')
    
    context = {
        'current_date': current_date,
        'time_of_day': time_of_day,
        'flame_status': flame_status,
        "times_temp_hum": json.dumps([d["time"] for d in temp_hum_data]),
        "values_temp": json.dumps([d["temperature"] for d in temp_hum_data]),
        "values_hum": json.dumps([d["humidity"] for d in temp_hum_data]),

        "temp_latest": temp_hum_data[-1]["temperature"] if temp_hum_data else 0,
        "hum_latest": temp_hum_data[-1]["humidity"] if temp_hum_data else 0,
    }
    
    return render(request, 'dashboard3.html', context)

def get_dashboard3_data(request):
    """API endpoint to provide data for dashboard3 charts and values"""
    temp_hum_data = SENSOR_DATA.get('dht11_2', [])

    
    data = {
        'temp_hum_data': {
            'times': [d["time"] for d in temp_hum_data],
            'temperature': [d["temperature"] for d in temp_hum_data],
            'humidity': [d["humidity"] for d in temp_hum_data]
        },
        
        'temp_latest': temp_hum_data[-1]["temperature"] if temp_hum_data else 0,
        'hum_latest': temp_hum_data[-1]["humidity"] if temp_hum_data else 0,
        'flame_status': SENSOR_DATA.get('flamme', 'Non Detecte')
    }
    
    return JsonResponse(data)



def dashboard4(request):
    current_date = datetime.now().strftime("%d %b %Y")
    
    # Get time of day
    hour = datetime.now().hour
    if hour >= 5 and hour < 12:
        time_of_day = "Morning"
    elif hour >= 12 and hour < 18:
        time_of_day = "Afternoon"
    else:
        time_of_day = "Evening"

    # Get ultrasonic sensor value and determine status
    ultrason_value = SENSOR_DATA.get('ultrason', 0)
    # If distance is >= 6cm, water level is insufficient
    water_status = "suffisant" if ultrason_value == 0 else "insuffisant"
    context = {
        'water_status': water_status,
        'current_date': current_date,
        'time_of_day': time_of_day,
        'ultrason_value': ultrason_value
    }
    return render(request, './dashboard4.html', context)
def dashboard5(request):
    current_date = datetime.now().strftime("%d %b %Y")
    
    # Get time of day
    hour = datetime.now().hour
    if hour >= 5 and hour < 12:
        time_of_day = "Morning"
    elif hour >= 12 and hour < 18:
        time_of_day = "Afternoon"
    else:
        time_of_day = "Evening"

    # Get bottle water status and convert to 'plein' or 'vide'
    bottle1_value = SENSOR_DATA.get('bottle_water_1', 0)
    bottle2_value = SENSOR_DATA.get('bottle_water_2', 0)
    
    bottle1_status = 'plein' if bottle1_value == 1 else 'vide'
    bottle2_status = 'plein' if bottle2_value == 1 else 'vide'

    context = {
        'current_date': current_date,
        'time_of_day': time_of_day,
        'bottle1_status': bottle1_status,
        'bottle2_status': bottle2_status
    }
    return render(request, 'dashboard5.html', context)


