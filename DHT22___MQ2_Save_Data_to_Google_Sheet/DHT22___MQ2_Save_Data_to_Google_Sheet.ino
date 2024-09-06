#include <WiFiNINA.h>
#include <WiFiClient.h>
#include <ArduinoHttpClient.h>
#include <DHT.h>

// Replace with your network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Google Script Deployment URL
const char* server = "script.google.com"; // Do not include "https://"
const char* path = "/macros/s/YOUR_SCRIPT_ID/exec"; // Use your deployment ID here

// WiFi and HTTP Client objects
WiFiClient wifiClient;
HttpClient client = HttpClient(wifiClient, server, 443);

// DHT22 Sensor Configuration
#define DHTPIN 2          // Digital pin connected to the DHT22 sensor
#define DHTTYPE DHT22     // DHT22 sensor type
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

// MQ2 Sensor Configuration
const int mq2Pin = A0;    // Analog pin connected to the MQ2 sensor

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;  // Wait for Serial to be ready
  }

  // Initialize DHT sensor
  dht.begin();

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
  // Read temperature and humidity from DHT22
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read gas level from MQ2 sensor
  int gasLevel = analogRead(mq2Pin);

  // Check if readings are valid
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT22 sensor!");
    return;
  }

  // Create the request URL
  String url = String(path) + "?temperature=" + temperature + "&humidity=" + humidity + "&gasLevel=" + gasLevel;

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

  delay(60000);  // Delay before the next loop (60 seconds)
}
