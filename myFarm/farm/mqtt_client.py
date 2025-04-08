import paho.mqtt.client as mqtt
from django.conf import settings
from threading import Thread
from .sensor_data import SENSOR_DATA
from datetime import datetime
import json


# Callback on connecting: subscribe to all sensor topics.
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected successfully to MQTT broker")
        
        # Use a unique prefix for your topics to avoid conflicts
        # For example, using your username/project name as a prefix
        client.subscribe("sne32/#")  # Subscribe to ESP32 topics
        client.subscribe("sne82/#")  # Subscribe to ESP82 topics
        
        print("Subscribed to topics: sne32/# and sne82/#")
    else:
        print("Failed to connect. Code:", rc)


# Callback on receiving a message:
def on_message(client, userdata, msg):
    topic = msg.topic
    payload = msg.payload.decode().strip()  # Ensure text data is clean

    print(f"MQTT message from topic: {topic} -> {payload}")

    if not payload:  # Check if payload is empty
        print(f"Received empty payload on topic: {topic}")
        return

    try:
        # Try parsing as JSON
        data = json.loads(payload)
        
        # Check if it's a DHT11 sensor message with temperature and humidity
        if isinstance(data, dict) and "temperature" in data and "humidity" in data:
            temperature = float(data["temperature"])
            humidity = float(data["humidity"])
            
            # Store DHT11 data differently based on the topic
            if "dht11_1" in topic:
                SENSOR_DATA['dht11_1'].append({
                    "time": datetime.now().strftime("%H:%M"),
                    "temperature": temperature,
                    "humidity": humidity
                })
                print(f"Stored DHT11_1 values: temp={temperature}, humidity={humidity}")
                return
            elif "dht11_2" in topic:
                SENSOR_DATA['dht11_2'].append({
                    "time": datetime.now().strftime("%H:%M"),
                    "temperature": temperature,
                    "humidity": humidity
                })
                print(f"Stored DHT11_2 values: temp={temperature}, humidity={humidity}")
                return
        
        # For other sensor data
        if isinstance(data, dict) and "value" in data:
            value = float(data["value"])  # Extract value field
        elif isinstance(data, dict) and "msg" in data:
            value = float(data["msg"])  # Handle msg field as value
        else:
            raise ValueError("Unexpected JSON format")
    except (json.JSONDecodeError, KeyError, ValueError):
        try:
            # If it's not JSON, try converting directly to float
            value = float(payload)
        except ValueError:
            print(f"Invalid payload format: {payload}")
            return

    # Store sensor data based on topic keywords rather than exact matches
    if "co2" in topic:
        SENSOR_DATA['co2'].append({
            "time": datetime.now().strftime("%H:%M"),
            "value": value
        })
    elif "soil1" in topic:
        SENSOR_DATA['humidite_sol_1'].append({
            "time": datetime.now().strftime("%H:%M"),
            "value": value
        })
    elif "soil2" in topic:
        SENSOR_DATA['humidite_sol_2'].append({
            "time": datetime.now().strftime("%H:%M"),
            "value": value
        })
    elif "bottle1" in topic:
        SENSOR_DATA['bottle_water_1'] = value
    elif "bottle2" in topic:
        SENSOR_DATA['bottle_water_2'] = value
    elif "pompe1" in topic:
        # Check if payload is a string (ON/OFF) or a number (0/1)
        if isinstance(payload, str) and payload.lower() in ['on', 'off']:
            SENSOR_DATA['pompe_eau_1'] = 'active' if payload.lower() == 'on' else 'inactive'
        else:
            SENSOR_DATA['pompe_eau_1'] = 'active' if value == 1 else 'inactive'
    elif "pompe2" in topic:
        # Check if payload is a string (ON/OFF) or a number (0/1)
        if isinstance(payload, str) and payload.lower() in ['on', 'off']:
            SENSOR_DATA['pompe_eau_2'] = 'active' if payload.lower() == 'on' else 'inactive'
        else:
            SENSOR_DATA['pompe_eau_2'] = 'active' if value == 1 else 'inactive'
    elif "temp3" in topic:
        SENSOR_DATA['temperature_3'].append({
            "time": datetime.now().strftime("%H:%M"),
            "value": value
        })
    elif "flamme" in topic:
        SENSOR_DATA['flamme'] = 'détecté' if value == 1 else 'non détecté'
    elif "pluie" in topic:
        SENSOR_DATA['pluie'].append({
            "time": datetime.now().strftime("%H:%M"),
            "value": value
        })
    elif "ultrason" in topic:
        SENSOR_DATA['ultrason'] = 'suffisant' if value == 1 else 'insuffisant'
        print(f"Stored ultrasonic status: {SENSOR_DATA['ultrason']}")
    else:
        print(f"Unknown topic: {topic}")



# Add debug callback for disconnections
def on_disconnect(client, userdata, rc):
    if rc != 0:
        print(f"Unexpected disconnection. Code: {rc}")
    else:
        print("Disconnected successfully")


# Initialize MQTT client with a unique client ID
client_id = f"django-dashboard-{datetime.now().timestamp()}"
client = mqtt.Client(client_id=client_id)
client.on_connect = on_connect
client.on_message = on_message
client.on_disconnect = on_disconnect

# Connect to the MQTT broker
try:
    print(f"Connecting to MQTT broker at {settings.MQTT_SERVER}:{settings.MQTT_PORT}")
    client.username_pw_set(settings.MQTT_USER, settings.MQTT_PASSWORD)
    client.connect(settings.MQTT_SERVER, settings.MQTT_PORT, settings.MQTT_KEEPALIVE)
    client.loop_start()
    print(f"MQTT client loop started with client ID: {client_id}")
except Exception as e:
    print(f"Error connecting to MQTT broker: {e}")