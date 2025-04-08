#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10      // SDA du RC522
#define RST_PIN 9      // RST du RC522
#define RFID_SERVO_PIN 3 // Servo pour RFID
#define BUTTON_SERVO_PIN 2 // Servo pour le bouton
#define BUTTON_PIN 4    // Bouton

MFRC522 rfid(SS_PIN, RST_PIN);
Servo rfidServo;
Servo buttonServo;

const byte UID_AUTORISE[] = {0x43, 0x0D, 0x1D, 0x28}; // Remplace avec ton UID RFID

void setup() {
    Serial.begin(115200);
    SPI.begin();
    rfid.PCD_Init();

    pinMode(BUTTON_PIN, INPUT_PULLUP);

    rfidServo.attach(RFID_SERVO_PIN);
    buttonServo.attach(BUTTON_SERVO_PIN);

    rfidServo.write(100);  // Position fermée
    buttonServo.write(100); // Position fermée

    Serial.println("Système prêt. Passez une carte RFID");
}

void loop() {
    // Vérification du bouton pour le premier servo
    if (digitalRead(BUTTON_PIN) == LOW) {
        buttonServo.write(20);
        Serial.println("Bouton appuyé : Servo 1 OUVERT !");
        delay(1000);
        buttonServo.write(100);
        Serial.println("Servo 1 FERMÉ !");
    }

    // Vérification du RFID pour le deuxième servo
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
        return;
    }

    Serial.print("Carte détectée : ");
    String cardUID = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
        Serial.print(rfid.uid.uidByte[i], HEX);
        Serial.print(" ");
        cardUID += String(rfid.uid.uidByte[i], HEX);
    }
    Serial.println();

    if (carteAutorisee()) {
        Serial.println("Accès autorisé ! Ouverture Servo 2...");
        ouvrirPorteRFID();
    } else {
        Serial.println("Accès refusé !");
    }

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
}

// Vérifie si la carte RFID est autorisée
bool carteAutorisee() {
    if (rfid.uid.size != sizeof(UID_AUTORISE)) return false;
    for (byte i = 0; i < rfid.uid.size; i++) {
        if (rfid.uid.uidByte[i] != UID_AUTORISE[i]) {
            return false;
        }
    }
    return true;
}

// Ouvre le deuxième servo avec RFID
void ouvrirPorteRFID() {
    rfidServo.write(20);
    Serial.println("Servo 2 OUVERT !");
    delay(3000);
    rfidServo.write(100);
    Serial.println("Servo 2 FERMÉ !");
}