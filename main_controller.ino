#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// PIN Definitions
#define SS_PIN    5
#define RST_PIN   4  
#define SERVO_PIN 12
#define BUZZER_PIN 13
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

MFRC522 rfid(SS_PIN, RST_PIN);
Servo storageLock;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Master Key UID for the blue keyfob
byte masterCard[] = {0x32, 0xE4, 0xA8, 0xAB};

// System state control
bool isUnlocked = false;

void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();
  
  storageLock.attach(SERVO_PIN);
  storageLock.write(0); // Initial position is locked
  
  pinMode(BUZZER_PIN, OUTPUT);
  
  // OLED Initialization
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Display connection failed!");
  } else {
    Serial.println("Display connected successfully!");
    showWaitingMessage();
  }
}

void loop() {
  // Check for new RFID cards
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  bool accessGranted = true;
  // Compare scanned UID with Master Key
  for (byte i = 0; i < 4; i++) {
    if (rfid.uid.uidByte[i] != masterCard[i]) {
      accessGranted = false;
      break;
    }
  }

  if (accessGranted) {
    isUnlocked = !isUnlocked; // Toggle the lock state

    if (isUnlocked) {
      // OPENING SEQUENCE
      playSuccessSound();
      displayMessage("HOSGELDIN ASKIMMMM", "SENI COK SEVIYORUMMMM");
      
      Serial.println("Box state: Unlocked");
      // Smooth servo open motion
      for (int pos = 0; pos <= 90; pos++) {
        storageLock.write(pos);
        delay(25); // Speed of opening (increase number to slow down)
      }
      
    } else {
      // CLOSING SEQUENCE
      playSuccessSound();
      displayMessage("BABAY", "ASKIMMMM");
      
      Serial.println("Box state: Locked");
      // Smooth servo close motion
      for (int pos = 90; pos >= 0; pos--) {
        storageLock.write(pos);
        delay(25); // Speed of closing (increase number to slow down)
      }
      
      delay(3000); // Show "LOCKED" message for 2 seconds
      showWaitingMessage();
    }
  } else {
    // Unauthorized access attempt
    playErrorSound();
  }
  
  rfid.PICC_HaltA();
  delay(2000); // Cooldown to prevent double scans
}

// Function to display messages on OLED
void displayMessage(String title, String sub) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(15, 15);
  display.println(title);
  display.setTextSize(1);
  display.setCursor(10, 45);
  display.println(sub);
  display.display();
}

// Default standby screen message
void showWaitingMessage() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(20, 25);
  display.println("BEKLIYOM ASKIMI");
  display.display();
}

// Double beep for a more noticeable success signal
void playSuccessSound() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(150); // First beep
  digitalWrite(BUZZER_PIN, LOW);
  delay(100); // Short silence
  digitalWrite(BUZZER_PIN, HIGH);
  delay(150); // Second beep
  digitalWrite(BUZZER_PIN, LOW);
}

// Triple beep for wrong card
void playErrorSound() {
  for(int i=0; i<3; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  }
}
