#include <Wire.h>
#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#define DHTPIN 4        
#define DHTTYPE DHT11    
DHT dht(DHTPIN, DHTTYPE);

#define CO2_PIN 34  // Capteur de Co2
#define RELAY_PIN 5   // Ventilateur  
#define SOIL_SENSOR_1 32 //Capteur d'himidite de sol (Plante)
#define SOIL_SENSOR_2 33 //Capteur d'himidite de sol (Étable)
#define PUMP_1 25    //Pompe de l'eau 1
#define PUMP_2 26    //Pompe de l'eau 2  
#define WATER_LEVEL_1 27  //Capteur de niveau de l'eau 1
#define WATER_LEVEL_2 14 // Capteur de niveau de l'eau 2

const char* ssid = "AGJDIELEC";
const char* password = "xxxxxxxx.2020";
const char* mqtt_server = "test.mosquitto.org";

const char* twilio_api = "https://api.twilio.com/2010-04-01/Accounts/AC734a21a44eda1b96f0ed5861de477967/Messages.json";
const char* twilio_account_sid = "AC734a21a44eda1b9xxxxxxxxxxxxxxx";
const char* twilio_auth_token = "82e75924f36783fxxxxxxxxxxxxxxxxxx";
const char* twilio_whatsapp_number = "+177536xxxx"; 
const char* recipient_whatsapp = "+2126xxxxxxxx";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
    Serial.println("Connexion au WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connecté !");
}

void reconnect_mqtt() {
    while (!client.connected()) {
        Serial.println("Connecting to MQTT...");
        String clientId = "ESP32_esto_2025_" + String(WiFi.macAddress());
        if (client.connect(clientId.c_str())) {
            Serial.println("MQTT Connected!");
        } else {
            Serial.print("MQTT Connection Failed: ");
            Serial.println(client.state());
            delay(5000);
        }
    }
}

void send_whatsapp_alert(const char* message) {
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClientSecure client;
        client.setInsecure();
        HTTPClient http;

        http.begin(client, twilio_api);
        http.setAuthorization(twilio_account_sid, twilio_auth_token);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        String postData = "To=" + String(recipient_whatsapp) + "&From=" + String(twilio_whatsapp_number) + "&Body=" + String(message);
        int httpCode = http.POST(postData);
        Serial.println(httpCode == 201 ? "WhatsApp alert sent!" : "WhatsApp failed!");
        http.end();
    }
}

void setup() {
    Serial.begin(115200);
    dht.begin();
    setup_wifi();
    client.setServer(mqtt_server, 1883);

    pinMode(RELAY_PIN, OUTPUT);
    pinMode(PUMP_1, OUTPUT);
    pinMode(PUMP_2, OUTPUT);
    pinMode(SOIL_SENSOR_1, INPUT);
    pinMode(SOIL_SENSOR_2, INPUT);
    pinMode(WATER_LEVEL_1, INPUT);
    pinMode(WATER_LEVEL_2, INPUT);

    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(PUMP_1, LOW);
    digitalWrite(PUMP_2, LOW);
}

void loop() {
    if (!client.connected()) {
        reconnect_mqtt();
    }
    client.loop();

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    float voltage = analogRead(CO2_PIN) * (3.3 / 4095.0);
    float co2_ppm = (voltage - 0.4) / 0.02;
    int soil_1 = analogRead(SOIL_SENSOR_1);
    int soil_2 = analogRead(SOIL_SENSOR_2);
    bool water_level_1 = analogRead(WATER_LEVEL_1);
    bool water_level_2 = analogRead(WATER_LEVEL_2);

    char msg[80];
    snprintf(msg, 80, "temperature: %.1fC, humidity: %.1f%%", temperature, humidity);
    client.publish("sne32/dht11_1", msg);

    snprintf(msg, 80, "%.0f ppm", co2_ppm);
    client.publish("sne32/co2", msg);

    snprintf(msg, 80, "%d", soil_1);
    client.publish("sne32/soil1", msg);

    snprintf(msg, 80, "%d", soil_2);
    client.publish("sne32/soil2", msg);

    snprintf(msg, 80, "%d", water_level_1);
    client.publish("sne32/bottle1", msg);

    snprintf(msg, 80, "Water2: %d", water_level_2);
    client.publish("sne32/bottle2", msg);

    if (temperature > 30 || co2_ppm > 80) {
        digitalWrite(RELAY_PIN, HIGH);
        client.publish("sne32/fan", "ON");
        send_whatsapp_alert("Alerte! Température élevée ou CO2 excessif!");
    } else {
        digitalWrite(RELAY_PIN, LOW);
        client.publish("sne32/fan", "OFF");
    }

    if (soil_1 < 1000 && water_level_1) {
        digitalWrite(PUMP_1, HIGH);
        client.publish("sne32/pompe1", "ON");
    } else {
        digitalWrite(PUMP_1, LOW);
        client.publish("sne32/pompe1", "OFF");
    }

    if (soil_2 < 1000 && water_level_2) {
        digitalWrite(PUMP_2, HIGH);
        client.publish("sne32/pompe2", "ON");
    } else {
        digitalWrite(PUMP_2, LOW);
        client.publish("sne32/pompe2", "OFF");
    }

    delay(2000);
}
