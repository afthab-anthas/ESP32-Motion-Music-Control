#define SIGNAL_PIN 5 // GPIO5 from STM32
#define LED_PIN 4 

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>  // JSON parsing library

// Spotify API Credentials
String accessToken = "";
String refreshToken = "AQCUt-xFWMJ08vLM44mfH29PaJLV-681_DusySeJLMqxNKqQ_XRg1X25jVGmkHgYee6x0FBzJLG8Jk5zB2DkkRGk0FvCssXxw0MmuXrAr8oQPLQUNDF3q7-lJpSCib46qLM";
const char* clientID = "03b3f909f3e24c019738b0459fb28ac4";
const char* clientSecret = "bee960a16fb44b3b9f76afd54bed6cc7";

// Spotify API URLs
const char* playURL = "https://api.spotify.com/v1/me/player/play";
const char* pauseURL = "https://api.spotify.com/v1/me/player/pause";
const char* refreshURL = "https://accounts.spotify.com/api/token";
const char* deviceURL = "https://api.spotify.com/v1/me/player/devices";

WiFiClientSecure client;

char ssid[32];        // Array to store SSID
char password[64];    // Array to store WiFi password

void setup() {
    Serial.begin(9600);
    pinMode(SIGNAL_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);

    // Use predefined credentials
    strcpy(ssid, "Redmi Sinan");
    strcpy(password, "hifive123");
    Serial.println("Using hardcoded Wi-Fi credentials.");


    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");
    client.setInsecure();  // Ignore SSL certificate (not recommended for production)
    refreshAccessToken();  // Get fresh token on start
}

void sendSpotifyCommand(const char* url) {
    HTTPClient http;
    http.begin(client, url);
    http.addHeader("Authorization", "Bearer " + accessToken);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.PUT("{}" );
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    if (httpResponseCode == 401) {  // Token expired
        refreshAccessToken();
    }
    http.end();
}

void refreshAccessToken() {
    HTTPClient http;
    http.begin(client, refreshURL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String body = "grant_type=refresh_token&refresh_token=" + refreshToken +
                  "&client_id=" + String(clientID) +
                  "&client_secret=" + String(clientSecret);
    int httpResponseCode = http.POST(body);
    
    if (httpResponseCode == 200) {
        String response = http.getString();
        Serial.println("Token Refreshed Successfully!");

        // Parse JSON Response
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, response);
        accessToken = doc["access_token"].as<String>();
        Serial.println("New Access Token: " + accessToken);
    } else {
        Serial.println("Failed to refresh token. HTTP Response: " + String(httpResponseCode));
    }
    http.end();
}

String getActiveDeviceID() {
    HTTPClient http;
    http.begin(client, deviceURL);
    http.addHeader("Authorization", "Bearer " + accessToken);
    int httpResponseCode = http.GET();

    String deviceID = "";
    if (httpResponseCode == 200) {
        String response = http.getString();
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, response);
        if (doc["devices"].size() > 0) {
            deviceID = doc["devices"][0]["id"].as<String>();
            Serial.println("Active Device ID: " + deviceID);
        } else {
            Serial.println("No active Spotify devices found!");
        }
    } else {
        Serial.println("Failed to get active device ID. HTTP Response: " + String(httpResponseCode));
    }
    http.end();
    return deviceID;
}

void loop() {
    int signal = digitalRead(SIGNAL_PIN);
    Serial.println("Checking for active device...");
    String deviceID = getActiveDeviceID();

    if (deviceID != "") {
        if(signal == HIGH){
          Serial.println("Playing Spotify...");
          sendSpotifyCommand(playURL);
          digitalWrite(LED_PIN, HIGH);
        } else{
          Serial.println("Pausing Spotify...");
          sendSpotifyCommand(pauseURL);
          digitalWrite(LED_PIN, LOW);
        }
    } else {
        Serial.println("No active Spotify device found. Retrying in 5 seconds...");
        delay(5000);
    }
}
