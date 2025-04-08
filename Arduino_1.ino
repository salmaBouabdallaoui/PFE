#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

#define PIR_PIN 2
#define LDR_PIN A5
#define SERVO_PIN 5

// Pins du registre à décalage
#define DATA_PIN 3   // DS
#define LATCH_PIN 4  // ST_CP
#define CLOCK_PIN 6  // SH_CP

Servo servo;  
bool authorized = false;

void setup() {
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();
    
    pinMode(PIR_PIN, INPUT);
    pinMode(LDR_PIN, INPUT);
    
    pinMode(DATA_PIN, OUTPUT);
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);

    servo.attach(SERVO_PIN);
    servo.write(0); // Fermer la porte par défaut

    Serial.println("Système prêt...");
}

// Fonction pour envoyer un octet au 74HC595
void shiftOutData(byte data) {
    digitalWrite(LATCH_PIN, LOW); 
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, data);
    digitalWrite(LATCH_PIN, HIGH);
}

void loop() {
    int ldrValue = analogRead(LDR_PIN);
    int pirState = digitalRead(PIR_PIN);

    Serial.print("Lumière: "); Serial.print(ldrValue);
    Serial.print(" | Mouvement: "); Serial.println(pirState);

    // LEDs avec registre à décalage
    if (pirState == HIGH && ldrValue <320) {
        shiftOutData(0xFF); // Allumer toutes les LEDs
        Serial.println("LEDs allumées !");
    } else {
        shiftOutData(0x00); // Éteindre toutes les LEDs
        Serial.println("LEDs éteintes.");
    }
                 
    Serial.println("Passez votre carte RFID...");

    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        Serial.print("Carte détectée, UID: ");
        String tagID = "";
        for (byte i = 0; i < mfrc522.uid.size; i++) {
            tagID += String(mfrc522.uid.uidByte[i], HEX);
        }
        Serial.println(tagID);

        if (tagID == "69279718") { // Remplace par ton UID
            authorized = !authorized;
            Serial.println(authorized ? "Accès autorisé" : "Accès refusé");
            if (authorized) {
                Serial.println("Ouverture du servo...");
                servo.write(90);
                delay(3000);
                servo.write(0);
                Serial.println("Fermeture du servo.");
            }
        }
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
    }

    delay(2000);
}
