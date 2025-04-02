#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

// WiFi Credentials
const char* ssid = "Redmi Sinan";
const char* password = "hifive123";

// Spotify API Credentials
String accessToken = "BQC4Na5fnfVEPmhLkLY8n-mN30zwwhO7QXH1ValpTMke6EE6jiInyH7dAAe2UDy-YnzzA1WhACvtA9OOMD1892ZMzuE23GcBaPQV39cuJm0JLPsHCsC5UJ0X-qsB4GYP-xyul0e-UaHb7_oDnCo1qn4bZByE7zWCSnauvfAEA3kJW5zwvMr1IgW8uGHViReKZMXHpbUakHdNGh3-0lDBp7jJnjFpcHDSiMGVX2QJJC8IhBdYtMQwr3hK-2PJ-7ud9yqxnvJRRMm7Vz3QKePwqx0tRBj98zOcNmGLT3mxCszcrTUDiOMdKwjLEYTDbA";
String refreshToken = "AQCUt-xFWMJ08vLM44mfH29PaJLV-681_DusySeJLMqxNKqQ_XRg1X25jVGmkHgYee6x0FBzJLG8Jk5zB2DkkRGk0FvCssXxw0MmuXrAr8oQPLQUNDF3q7-lJpSCib46qLM";
const char* clientID = "03b3f909f3e24c019738b0459fb28ac4";
const char* clientSecret = "bee960a16fb44b3b9f76afd54bed6cc7";

// Spotify API URLs
const char* playURL = "https://api.spotify.com/v1/me/player/play";
const char* pauseURL = "https://api.spotify.com/v1/me/player/pause";
const char* refreshURL = "https://accounts.spotify.com/api/token";
const char* deviceURL = "https://api.spotify.com/v1/me/player/devices";

// WiFiClientSecure for HTTPS requests
WiFiClientSecure client;
HTTPClient http;

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");

    client.setInsecure();  // Ignore SSL certificate
}

void sendSpotifyCommand(const char* url) {
    WiFiClientSecure client;
    HTTPClient http;

    client.setInsecure();  // Ignore SSL certificate (not recommended for production)
    http.begin(client, url);
    http.addHeader("Authorization", String("Bearer ") + accessToken);
    http.addHeader("Content-Type", "application/json");

    // Send an EMPTY JSON object '{}' to satisfy Content-Length requirement
    int httpResponseCode = http.PUT("{}");  

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    http.end();
}


// 🔄 Refresh Access Token When Expired
void refreshAccessToken() {
    http.begin(client, refreshURL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String body = "grant_type=refresh_token&refresh_token=" + refreshToken +
                  "&client_id=" + String(clientID) +
                  "&client_secret=" + String(clientSecret);

    int httpResponseCode = http.POST(body);

    if (httpResponseCode == 200) {
        String response = http.getString();
        int tokenStart = response.indexOf("\"access_token\":\"") + 16;
        int tokenEnd = response.indexOf("\"", tokenStart);
        accessToken = response.substring(tokenStart, tokenEnd);
        Serial.println("New Access Token: " + accessToken);
    } else {
        Serial.print("Failed to refresh token. HTTP Response: ");
        Serial.println(httpResponseCode);
    }

    http.end();
}

// 🔍 Get Active Device ID
String getActiveDeviceID() {
    http.begin(client, deviceURL);
    http.addHeader("Authorization", "Bearer " + accessToken);

    int httpResponseCode = http.GET();
    String deviceID = "";

    if (httpResponseCode == 200) {
        String response = http.getString();
        int idStart = response.indexOf("\"id\":\"") + 6;
        int idEnd = response.indexOf("\"", idStart);
        deviceID = response.substring(idStart, idEnd);
        Serial.println("Active Device ID: " + deviceID);
    } else {
        Serial.println("Failed to get active device ID. HTTP Response: " + String(httpResponseCode));
    }

    http.end();
    return deviceID;
}

void loop() {
    Serial.println("Checking for active device...");
    String deviceID = getActiveDeviceID();

    if (deviceID != "") {
        Serial.println("Playing Spotify...");
        sendSpotifyCommand(playURL);
        delay(5000);  // Wait 5 seconds

        Serial.println("Pausing Spotify...");
        sendSpotifyCommand(pauseURL);
        delay(5000);  // Wait 5 seconds
    } else {
        Serial.println("No active Spotify device found! Please open Spotify and play something.");
        delay(5000);
    }
}