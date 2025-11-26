#include <WiFi.h>
#include <WebSocketsClient.h>

#define BUZZER_PIN 12

WebSocketsClient webSocket;

const char* ssid = "Ukrtelecom_784C";
const char* password = "10946220";

void onWebSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_CONNECTED:
      Serial.println("Connected to Django WebSocket!");
      webSocket.sendTXT("{\"msg\":\"hello from ESP32\"}");
      break;

    case WStype_TEXT:
      {
        String msg = String((char*)payload); 
        Serial.println("Received: " + msg);

        if (msg.indexOf("buzzer_on") >= 0) {
          Serial.println("Done");
          digitalWrite(BUZZER_PIN, HIGH);
        } else if (msg.indexOf("buzzer_off") >= 0) {

          digitalWrite(BUZZER_PIN, LOW);
        }
      }
      break;

    case WStype_DISCONNECTED:
      Serial.println("Disconnected!");
      break;

    case WStype_ERROR:
      Serial.println("WebSocket Error!");
      break;

    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  webSocket.begin("192.168.1.106", 8000, "/ws/esp/");
  webSocket.onEvent(onWebSocketEvent);
  webSocket.setReconnectInterval(5000);
}

void loop() {
  webSocket.loop(); 
  
}
