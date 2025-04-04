#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10   // SDA du RC522
#define RST_PIN 9   // RST du RC522
#define SERVO_PIN 6 // Servo sur D6

MFRC522 rfid(SS_PIN, RST_PIN);
Servo porteServo;

// ID de la carte autorisée (Remplace par ton UID)
const byte UID_AUTORISE[] = {0x43, 0x0D, 0x1D, 0x28}; 

void setup() {
    Serial.begin(115200);
    SPI.begin();
    rfid.PCD_Init();
    porteServo.attach(SERVO_PIN);
    porteServo.write(90);  // Position fermée au départ

    Serial.println("Système prêt. Passez une carte RFID...");
}

void loop() {
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
        return;  // Aucune carte détectée
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
        Serial.println("Accès autorisé ! Ouverture...");
        ouvrirPorte();
    } else {
        Serial.println("Accès refusé !");
    }

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
}

bool carteAutorisee() {
    if (rfid.uid.size != sizeof(UID_AUTORISE)) return false;
    for (byte i = 0; i < rfid.uid.size; i++) {
        if (rfid.uid.uidByte[i] != UID_AUTORISE[i]) {
            return false;
        }
    }
    return true;
}

void ouvrirPorte() {
    porteServo.write(0);  // Tourner à 90° (ouvrir)
    delay(3000);           // Attente 3 secondes
    porteServo.write(90);   // Retour à 0° (fermer)
}



