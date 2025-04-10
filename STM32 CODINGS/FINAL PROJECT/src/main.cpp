#include <Arduino.h>

#define TRIG_PIN D8
#define ECHO_PIN D9
#define LED_PIN D7               // Pin connected to the LED
#define ESP_PIN D2               // Pin connected to the ESP32
#define DISTANCE_THRESHOLD 10    // in cm
#define MOTION_THRESHOLD 1     // Minimum distance change to detect motion (in cm)
#define MOTION_DELAY 100         // Delay between readings to prevent rapid false detection

unsigned long lastMotionTime = 0;

long previousDistance = 0;      // To store the previous distance reading

void setup() {
    Serial.begin(115200);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);   // Set the LED pin as output
}

long getDistanceCM() {
    // Clear trigger
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    // Trigger pulse
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Read echo duration
    long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout

    // Calculate distance in cm
    long distance = duration * 0.0343 / 2;
    return distance;
}

void loop() {
    long currentDistance = getDistanceCM();

    // Check if the current distance is within the threshold of 10 cm
// Your existing code (unchanged)
if (currentDistance > 0 && currentDistance <= DISTANCE_THRESHOLD) {
  // Calculate the change in distance from the previous reading
  long distanceChange = abs(currentDistance - previousDistance);

  if (distanceChange >= MOTION_THRESHOLD) {
      // If the distance changes even slightly (greater than the threshold), consider it as motion
      Serial.println("Motion Detected!");
      digitalWrite(LED_PIN, HIGH);  // Turn ON LED when motion is detected
      digitalWrite(ESP_PIN, HIGH);  // Turn ON LED when motion is detected
      lastMotionTime = millis();    // 🔥 Track when motion was last seen
  } else {
      // If the distance doesn't change significantly, no motion detected
      if (millis() - lastMotionTime >= 5000) {  // ⏱ Only print if no motion for 10 sec
          Serial.println("No motion detected");
          digitalWrite(LED_PIN, LOW);   // Turn OFF LED when no motion
          digitalWrite(ESP_PIN, LOW);   // Turn OFF LED when no motion
      }
  }

  // Store the current distance for next comparison
  previousDistance = currentDistance;

} else {
  // If the distance is greater than the threshold, reset the previous distance
  if (millis() - lastMotionTime >= 10000) {  // ⏱ Only print if no motion for 10 sec
      Serial.println("No motion detected");
      digitalWrite(LED_PIN, LOW);   // Turn OFF LED if no object is detected within the threshold
      digitalWrite(ESP_PIN, LOW);   // Turn OFF LED if no object is detected within the threshold
  }
}

    delay(MOTION_DELAY);  // Delay to avoid rapid false detection
}
