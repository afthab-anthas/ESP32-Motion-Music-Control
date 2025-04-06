#include <Arduino.h>

#define TRIG_PIN D8
#define ECHO_PIN D9

#define DISTANCE_THRESHOLD 10 // in cm
#define MOTION_THRESHOLD 5    // Minimum change in distance to detect motion (in cm)
#define MOTION_DELAY 100      // Time delay between measurements to check motion

long previousDistance = 0; // To store the previous distance reading

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
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

  // Check if the current distance is within the threshold
  if (currentDistance > 0 && currentDistance <= DISTANCE_THRESHOLD) {
    // Calculate the change in distance from the previous reading
    long distanceChange = abs(currentDistance - previousDistance);

    if (distanceChange >= MOTION_THRESHOLD) {
      // If the distance changes significantly, consider it as motion
      Serial.print("Motion Detected! Distance: ");
      Serial.print(currentDistance);
      Serial.println(" cm");
    } else {
      // If the distance doesn't change significantly, no motion detected
      Serial.println("No motion detected.");
    }

    // Store the current distance for the next loop
    previousDistance = currentDistance;

  } else {
    // If the reading is out of range or invalid
    Serial.println("Out of range or no echo.");
  }

  delay(MOTION_DELAY); // Delay to avoid frequent checks and false detection
}
