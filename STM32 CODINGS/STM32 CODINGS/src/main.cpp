#include <Arduino.h>

#define PIR_PIN D2  // Pin connected to PIR sensor output

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Set PIR_PIN as input
  pinMode(PIR_PIN, INPUT);
}

void loop() {
  // Read the PIR sensor output
  int motionDetected = digitalRead(PIR_PIN);
  
  if (motionDetected == HIGH) {
    Serial.println("Motion Detected!");
  } else {
    Serial.println("No Motion");
  }

  delay(500);  // Wait for half a second before checking again
}
