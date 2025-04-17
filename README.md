# ESP32-Motion-Music-Control

Control Spotify playback using simple hand gestures! This project uses an STM32 microcontroller to detect motion, which then signals an ESP32 to send commands to Spotify via its API — all done wirelessly over Wi-Fi.

---

## 🧠 Concept

The STM32 uses an ultrasonic sensor to detect motion. When motion is detected within 10 cm, it sends a GPIO signal to the ESP32. The ESP32 is connected to Wi-Fi and the Spotify API. Based on this signal, it sends a **play** or **pause** command to Spotify. It's a hands-free, gesture-based way to control your music — ideal for a desk or room setup.

---

## 📦 Technologies Used

### STM32 Side:
- STM32 (e.g., STM32F103 or Nucleo)
- Ultrasonic Sensor (HC-SR04)
- GPIO for signal output
- LED indicator

### ESP32 Side:
- ESP32 Dev Board
- Wi-Fi & Secure HTTPS Communication
- Spotify API integration
- Token Refresh (OAuth 2.0)
- GPIO for signal input
- LED indicator

---

## ⚙️ Features

- 👋 Motion-controlled playback: wave your hand to play/pause music  
- 🔐 Secure token-based Spotify control  
- 📶 Wi-Fi-enabled, real-time API communication  
- 🔁 Automatic device detection and playback targeting  
- 🔋 Low-power, efficient sensor control  
- 💡 LED indicators for visual feedback  

---

## 💻 Setup Instructions

### Hardware:
- Connect the ultrasonic sensor to STM32.
- Use GPIO output from STM32 to trigger input on ESP32.
- Wire LEDs for feedback on both boards.
- Ensure ESP32 has internet access via Wi-Fi.

### Software:
1. Flash the STM32 with motion detection code (Arduino or PlatformIO).
2. Flash the ESP32 with the Spotify control code:
   - Insert your `refreshToken`, `clientID`, `clientSecret`, and Wi-Fi credentials.
   - Install libraries: `WiFi`, `WiFiClientSecure`, `HTTPClient`, `ArduinoJson`.
3. Open Serial Monitor on ESP32 to debug and monitor playback status.

---

## 🧪 What I Learned

- Spotify API and OAuth 2.0 token flows
- Secure HTTPS communication from microcontrollers
- Sensor input/output synchronization between STM32 and ESP32
- Real-time gesture-based hardware interfaces
- Wi-Fi and JSON-based RESTful API interaction
- Debugging low-level timing and logic in embedded systems

---

## 🚀 Future Improvements

- Gesture-controlled volume or song skipping
- Web or mobile app for live configuration
- Replace hardcoded credentials with OTA or QR-based setup
- OLED or LCD display for song info and playback status
- AI-powered smart playlist control using motion patterns

---

## 🤝 Credits

Built with ❤️ by **Afthab Anthas**, **Bhumika Singh** and **Sinan Luckman**   
GitHub: [@afthab-anthas](https://github.com/afthab-anthas)  
Date: April 2025

---


## 📂 Repository Structure
Motion-Controlled-Spotify/ ├── ESP32/ │ └── esp32_spotify.ino ├── STM32/ │ └── motion_sensor_stm32.ino └── README.md

---

## 📬 Want to Collaborate?

Got ideas to expand this project? Want to add voice commands, sensors, or integrate with smart home devices? Fork this repo or reach out — let’s build cool stuff together!



