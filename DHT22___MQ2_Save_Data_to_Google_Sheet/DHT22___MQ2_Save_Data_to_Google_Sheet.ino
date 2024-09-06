#include <WiFiNINA.h>
#include <WiFiClient.h>
#include <ArduinoHttpClient.h>

// Replace with your network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Google Script Deployment URL
const char* server = "script.google.com"; // Do not include "https://"
const char* path = "/macros/s/YOUR_SCRIPT_ID/exec"; // Use your deployment ID here

// WiFi and HTTP Client objects
WiFiClient wifiClient;
HttpClient client = HttpClient(wifiClient, server, 443);

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;  // Wait for Serial to be ready
  }

  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
}

void loop() {
  // Simulated sensor data (replace this with actual sensor data code)
  float temperature = 25.0 + random(-500, 500) / 100.0;
  float humidity = 50.0 + random(-200, 200) / 100.0;
  
  // Create the request URL
  String url = String(path) + "?value1=" + temperature + "&value2=" + humidity;

  // Send data to Google Sheets
  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.beginRequest();
  client.get(url);
  client.endRequest();

  // Read the response from the server
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
  
  Serial.print("Response Status Code: ");
  Serial.println(statusCode);
  Serial.print("Response Body: ");
  Serial.println(response);

  delay(60000);  // Delay before next loop (60 seconds)
}
