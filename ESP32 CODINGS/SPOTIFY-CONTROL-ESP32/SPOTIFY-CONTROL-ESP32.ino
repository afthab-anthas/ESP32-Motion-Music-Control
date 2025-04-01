#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

const char* ssid = "Anthas Home";
const char* password = "althaf1109";
const char* accessToken = "BQCiNL1huEF9WfFBdm2905YjGf88OpYCC_dTNr98YJzKkYDO7I9fsxl1YHf58Eg7SfCERvgyYrOw2utzBvEooyQ6DGgL8BC-DLCXKT3MpuH_Byn1LUPISCBqDWwQTsM3nAhwHYR5gMkZNIp76Ga8k6_IsOdyErKSBCwo4JjOmCKx6bDePRdhkJqPe6vCMaqU7a0uIk5aATE62qsGy_D9YhWjGy9snrFsh2U3XAesfZ2FDSnEsO4";  // Paste your access token here

const char* playURL = "https://api.spotify.com/v1/me/player/play";
const char* pauseURL = "https://api.spotify.com/v1/me/player/pause";

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");
}

void sendSpotifyCommand(const char* url) {
    WiFiClientSecure client;
    HTTPClient http;

    client.setInsecure();  // Ignore SSL certificate (not recommended for production)
    http.begin(client, url);
    http.addHeader("Authorization", String("Bearer ") + accessToken);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Content-Length", "0");  // Fix for HTTP 411 error

    int httpResponseCode = http.PUT("");  // PUT request for play/pause

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    http.end();
}


void loop() {
    Serial.println("Playing Spotify...");
    sendSpotifyCommand(playURL);
    delay(5000);  // Wait 5 seconds

    Serial.println("Pausing Spotify...");
    sendSpotifyCommand(pauseURL);
    delay(5000);  // Wait 5 seconds
}
