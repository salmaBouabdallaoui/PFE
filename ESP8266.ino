#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <Servo.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "AGJDIELEC";
const char* password = "xxxxxxxxxx2020";
const char* mqtt_server = "test.mosquitto.org";  // Use broker IP if needed

// Twilio WhatsApp settings
const char* twilio_api = "https://api.twilio.com/2010-04-01/Accounts/AC734a21a44eda1b96f0ed5861de477967/Messages.json";
const char* twilio_account_sid = "AC734a21a44eda1b9xxxxxxxxxxxxxxx";
const char* twilio_auth_token = "82e75924f36783fxxxxxxxxxxxxxxxxxx";
const char* twilio_whatsapp_number = "+177xxxxxxx"; 
const char* recipient_whatsapp = "+2126xxxxxxxx";

// Sensor pins
#define DHT_PIN 14
#define DHT_TYPE DHT11
#define FLAME_PIN 16
#define LIGHT_PIN A0
#define RAIN_PIN 5
#define TRIG_PIN 12
#define ECHO_PIN 13
#define SERVO_TOIT_PIN 0

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHT_PIN, DHT_TYPE);
Servo servoToit;

float temperature, humidity;
int flame, light, rain;
long duration, distance;

void setup_wifi() {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi Connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nWiFi Connection Failed!");
    }
}

void reconnect_mqtt() {
    while (!client.connected()) {
        Serial.println("Connecting to MQTT...");
        String clientId = "ESP8266_" + String(random(0, 9999));
        
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
        client.setInsecure();  // Disable SSL certificate validation
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

void send_mqtt_data() {
    if (!client.connected()) return;
    
    client.publish("sne82/dht11_2", String(temperature).c_str());
    client.publish("sne82/flamme", String(flame).c_str());
    client.publish("sne82/lumiere", String(light).c_str());
    client.publish("sne82/pluie", String(rain).c_str());
    client.publish("sne82/ultrason", String(distance).c_str());

    Serial.println("MQTT Data Sent");
}

void handle_roof() {
    if (light < 800 && rain == 1) {
        servoToit.write(0);
    } else if (light < 800 && rain == 0) {
        servoToit.write(180);

    } else {
        servoToit.write(0);
    }
}

void read_sensors() {
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    flame = digitalRead(FLAME_PIN) == LOW ? 1 : 0;
    light = analogRead(LIGHT_PIN);
    rain = digitalRead(RAIN_PIN) == LOW ? 1 : 0;

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration * 0.034 / 2; 
}

void handle_alerts() {
    if (distance >= 6) {
        send_whatsapp_alert("Obstacle detected! Immediate action required.");
    }
    if (flame) {
        send_whatsapp_alert("Fire detected! Immediate intervention needed.");
    }
    if (rain) {
        send_whatsapp_alert("Rain detected. Checking roof status.");
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);  
    Serial.println("Starting ESP8266...");

    setup_wifi();
    client.setServer(mqtt_server, 1883);

    dht.begin();
    pinMode(RAIN_PIN, INPUT);
    pinMode(FLAME_PIN, INPUT);
    pinMode(LIGHT_PIN, INPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    servoToit.attach(SERVO_TOIT_PIN);
    servoToit.write(0);
}

void loop() {
    if (!client.connected()) {
        reconnect_mqtt();
    }
    client.loop();  

    read_sensors();
    handle_alerts();
    handle_roof();
    send_mqtt_data();

    delay(2000);
}
