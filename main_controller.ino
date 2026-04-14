#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// PIN Configuration
#define SS_PIN    5
#define RST_PIN   4  
#define SERVO_PIN 12
#define BUZZER_PIN 13
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

MFRC522 rfid(SS_PIN, RST_PIN);
Servo storageLock;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Master Key UID 
byte masterCard[] = {0x32, 0xE4, 0xA8, 0xAB};
bool isUnlocked = false;

// Function: Update OLED with custom messages
void displayMessage(String title, String sub) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE); 
  display.setTextSize(2); 
  display.setCursor(10, 10);
  display.println(title);
  display.setTextSize(1); 
  display.setCursor(10, 40);
  display.println(sub);
  display.display();
}

// Function: Default standby screen
void showWaitingMessage() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(15, 25);
  display.println("ASKIMI BEKLIYOM...");
  display.display();
}

// Function: Confirmation sound
void playSuccessSound() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(150); 
  digitalWrite(BUZZER_PIN, LOW); 
  delay(100);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(150); 
  digitalWrite(BUZZER_PIN, LOW);
}

// Function: Error sound
void playErrorSound() {
  for(int i=0; i<3; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(80); 
    digitalWrite(BUZZER_PIN, LOW); 
    delay(80);
  }
}

void setup() {
  Serial.begin(115200);
  SPI.begin();           
  rfid.PCD_Init();       
  
  storageLock.attach(SERVO_PIN);
  storageLock.write(0);  
  
  pinMode(BUZZER_PIN, OUTPUT);
  
  // OLED Initialization
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Display connection failed!");
  } else {
    Serial.println("Display connected successfully!");
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    showWaitingMessage();
  }
}

void loop() {
  // Check for new RFID cards
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  bool accessGranted = true;
  // Compare scanned UID with Master UID
  for (byte i = 0; i < 4; i++) {
    if (rfid.uid.uidByte[i] != masterCard[i]) {
      accessGranted = false;
      break;
    }
  }

  if (accessGranted) {
    isUnlocked = !isUnlocked; 

    if (isUnlocked) {
      playSuccessSound();
      displayMessage("HOSGELDIN", "SENI COK SEVIYOMMMM");
      
      for (int pos = 0; pos <= 90; pos++) { 
        storageLock.write(pos); 
        delay(30); 
      }
    } else {
      playSuccessSound();
      displayMessage("BABAYYYY", "ASKIMMMMM");
      
      for (int pos = 90; pos >= 0; pos--) { 
        storageLock.write(pos); 
        delay(30); 
      }
      delay(2000); 
      showWaitingMessage();
    }
  } else {
    playErrorSound();
  }
  
  rfid.PICC_HaltA(); 
  delay(1000);       
}
