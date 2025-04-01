#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

// WiFi Credentials
const char* ssid = "Anthas Home";
const char* password = "althaf1109";

// Spotify API Credentials
String accessToken = "BQD1pJzsFVhVZ_9G5JV5Tp8CTJsExY0f6SVI67xWj5wwwZzwPuWCIZd65HtdzrmU_nw-iO_rCTkQO_nMTochZw3Rq4j_udgID_nD3-yxIUs75iBokUzSnUfWF8hdgALoJcBr9LcNOFAYkXgRAwtMRSfAYn9w9598NAgtn3k78gGa7Loz9KwDMSkLJLsE4ScsphUqYPDYDFsVVFbiIhVumBz92BxvHro5Mhlf6AsIc1M_I2KC2_M";
String refreshToken = "AQAT8Lh1uowI4ivbIxbqY3ccM2qwFXNjA8c7SSjn21xqLJ3-YpWkez4eDwpov_tg5qgJlxrmsJtT2apSFMQg3uq-FsjX_yFsZbXlxRD8nGCNq6GjXYsdYPMz-A7KchUXE9Q";
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
