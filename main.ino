#include <WiFi.h>
#include <DHT.h>

// Default Wi-Fi credentials for Wokwi virtual simulator environment
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ThingSpeak target cloud infrastructure server address
const char* server = "api.thingspeak.com";

// Authorized unique Write API Key configured for the cloud channel
String apiKey = "OX46QX4FOXYGYRCM"; 

#define DHTPIN 15                     // Microcontroller digital pin mapping: DHT22 connected to GPIO 15
#define DHTTYPE DHT22                 // Initializing the specific physical sensor type module variant

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void setup() {
  Serial.begin(115200);               // Initializing hardware UART Serial communication at 115200 Baud Rate
  dht.begin();                        // Activating and stabilizing the digital temperature sensor hardware
  
  // Executing dynamic Wi-Fi network layer authentication and connectivity logic
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected Successfully!"); // Gateway connection established successfully
}

void loop() {
  // Reading environmental ambient temperature data streams from the sensor array
  float temp = dht.readTemperature();
  
  // Data integrity check: Terminate loop routine if data is corrupted or null
  if (isnan(temp)) {
    Serial.println("Failed to read from sensor!");
    return;
  }
  
  Serial.print("Current Temperature: ");
  Serial.print(temp);
  Serial.println(" *C");
  
  // Initiating remote client socket connection framework to server on standard HTTP Port 80
  if (client.connect(server, 80)) {
    // Constructing the encoded payload string targeting database schema configuration Field 1
    String postStr = apiKey + "&field1=" + String(temp) + "\r\n\r\n";
    
    // Formatting and dispatching the legal standard HTTP POST network requests pipeline
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: " + String(postStr.length()) + "\n\n");
    client.print(postStr);             // Pushing the final telemetry data block payload onto the cloud
    
    Serial.println("Data sent to ThingSpeak Server!");
  }
  client.stop();                      // Gracefully closing the network TCP connection socket channel
  
  // Enforcing mandatory 15-second delay to comply with ingestion rate limit constraints
  delay(15000); 
}
