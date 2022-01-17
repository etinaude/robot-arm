#include "WiFi.h"
#include "ESPAsyncWebServer.h"
 
const char* ssid = "¢={=====>";
const char* password =  "sudsnotdubs";
 
AsyncWebServer server(80);

#define RXD2 16
#define TXD2 17
 
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println(WiFi.localIP());
 
  server.on(
    "/post",
    HTTP_POST,
    [](AsyncWebServerRequest * request){},
    NULL,
    [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
 
      for (size_t i = 0; i < len; i++) {
        Serial.write(data[i]);
        Serial2.write(data[i]);
      }
 
      Serial.println();
      Serial2.println();
 
      request->send(200);
  });
 
  server.begin();
}
 
void loop() {}
