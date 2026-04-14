#include <SPI.h>
#include <MFRC522.h>

/* * Professional Tip: Using clear definitions for pin mapping * is standard practice in Embedded Systems.*/
#define SS_PIN 5
#define RST_PIN 4 

MFRC522 rfidReader(SS_PIN, RST_PIN); 

void setup() {
  Serial.begin(115200);
  SPI.begin();           // Init SPI bus
  rfidReader.PCD_Init(); // Init MFRC522 
  Serial.println("--- System Online: Waiting for RFID Scan ---");
}

void loop() {
  // Look for new cards
  if (!rfidReader.PICC_IsNewCardPresent()) return;
  
  // Select one of the cards
  if (!rfidReader.PICC_ReadCardSerial()) return;

  // Print UID in HEX format
  Serial.print("CARD DETECTED! UID: ");
  for (byte i = 0; i < rfidReader.uid.size; i++) {
    Serial.print(rfidReader.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfidReader.uid.uidByte[i], HEX);
  }
  Serial.println();
  
  // Stop reading to prevent multiple scans
  rfidReader.PICC_HaltA();
}
