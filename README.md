# rfid-smart-gift-box

📂 RFID Smart Gift Box (ESP32)

An interactive, secure gift box project that uses RFID authentication to control a servo-locking mechanism. This project demonstrates the integration of multiple embedded sensors and actuators using the ESP32 microcontroller.

🚀 Overview

This project was designed as a "Proof of Concept" for a secure access system. It identifies a specific Master Key (RFID) to unlock the box with a smooth servo motor motion and provides visual/audio feedback to the user.

🛠️ Hardware Components

Component	Function
ESP32 Dev Kit	System Microcontroller (The Brain)
RC522 RFID Module	13.56 MHz Identity Authentication
SSD1306 OLED (I2C)	User Interface & Status Display
MG90S Servo Motor	Mechanical Locking Actuator
Passive Buzzer	Audio Feedback (Status Tones)

🧠 Technical Features

    Secure Auth: Only the authorized Blue Keyfob UID (32 E4 A8 AB) can toggle the lock state.

    Smooth Motion Algorithm: Instead of a harsh 0-to-90 snap, the servo moves in 1-degree increments with a 30ms delay to protect the cardboard structure.

    I2C Communication: Optimized display refresh rates for real-time interaction.

    Toggle Logic: Scans the card once to open, and once more to lock—providing a stress-free user experience.

💻 Software Stack

    Language: C++ (Embedded)

    Environment: Arduino IDE (Transitioning to PlatformIO/VS Code)

    Framework: Arduino Core for ESP32
