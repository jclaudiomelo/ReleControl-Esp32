#include <U8g2lib.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

#define OLED_SDA 14   // GPIO14 (D5)
#define OLED_SCL 12   // GPIO12 (D6)
#define RELAY1_PIN 5  // GPIO5 (D1)
#define RELAY2_PIN 4  // GPIO4 (D2)
#define RELAY3_PIN 0  // GPIO0 (D3)
#define RELAY4_PIN 2  // GPIO2 (D4)
#define RELAY5_PIN 15 // GPIO15 (D8)
#define RELAY6_PIN 13 // GPIO13 (D7)
#define RELAY7_PIN 12 // GPIO12 (D6)
#define RELAY8_PIN 14 // GPIO14 (D5)

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE, OLED_SCL, OLED_SDA);

const char *ssid = "Rede_Oculta-srv";
const char *password = "000000000";

ESP8266WebServer server(80);

static const unsigned char image_wifi_not_connected_bits[] U8X8_PROGMEM = { 0x84, 0x0f, 0x00, 0x68, 0x30, 0x00, 0x10, 0xc0, 0x00, 0xa4, 0x0f, 0x01, 0x42, 0x30, 0x02, 0x91, 0x40, 0x04, 0x08, 0x85, 0x00, 0xc4, 0x1a, 0x01, 0x20, 0x24, 0x00, 0x10, 0x4a, 0x00, 0x80, 0x15, 0x00, 0x40, 0x20, 0x00, 0x00, 0x42, 0x00, 0x00, 0x85, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00 };

static const unsigned char image_wifi_5_bars_bits[] U8X8_PROGMEM = { 0x80, 0x0f, 0x00, 0xe0, 0x3d, 0x00, 0x78, 0xf0, 0x00, 0x8c, 0x8f, 0x01, 0xee, 0xbd, 0x03, 0x73, 0x70, 0x06, 0x1a, 0xc7, 0x02, 0xcc, 0x9f, 0x01, 0xe8, 0xb8, 0x00, 0x30, 0x62, 0x00, 0xa0, 0x2f, 0x00, 0xc0, 0x1d, 0x00, 0x80, 0x0a, 0x00, 0x00, 0x07, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00 };

static const unsigned char image_weather_humidity_white_bits[] U8X8_PROGMEM = { 0x20, 0x00, 0x20, 0x00, 0x30, 0x00, 0x50, 0x00, 0x48, 0x00, 0x88, 0x00, 0x04, 0x01, 0x04, 0x01, 0x82, 0x02, 0x02, 0x03, 0x01, 0x05, 0x01, 0x04, 0x02, 0x02, 0x02, 0x02, 0x0c, 0x01, 0xf0, 0x00 };

static const unsigned char image_weather_temperature_bits[] U8X8_PROGMEM = { 0x38, 0x00, 0x44, 0x40, 0xd4, 0xa0, 0x54, 0x40, 0xd4, 0x1c, 0x54, 0x06, 0xd4, 0x02, 0x54, 0x02, 0x54, 0x06, 0x92, 0x1c, 0x39, 0x01, 0x75, 0x01, 0x7d, 0x01, 0x39, 0x01, 0x82, 0x00, 0x7c, 0x00 };

static const unsigned char image_lamp_off_bits[] U8X8_PROGMEM =  { 0x40, 0x00, 0xe0, 0x00, 0xa0, 0x00, 0x10, 0x01, 0x10, 0x01, 0x10, 0x01, 0x0c, 0x06, 0x02, 0x08, 0x01, 0x10, 0xfe, 0x0f, 0x10, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static const unsigned char image_lamp_on_bits[] U8X8_PROGMEM = { 0x40, 0x00, 0xe0, 0x00, 0xa0, 0x00, 0x10, 0x01, 0x10, 0x01, 0x10, 0x01, 0x0c, 0x06, 0x02, 0x08, 0x01, 0x10, 0xfe, 0x0f, 0x10, 0x01, 0xe3, 0x18, 0x00, 0x00, 0x08, 0x02, 0x44, 0x04, 0x40, 0x00 };

void setup() {
  Serial.begin(115200);

  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  pinMode(RELAY4_PIN, OUTPUT);
  pinMode(RELAY5_PIN, OUTPUT);
  pinMode(RELAY6_PIN, OUTPUT);
  pinMode(RELAY7_PIN, OUTPUT);
  pinMode(RELAY8_PIN, OUTPUT);

  digitalWrite(RELAY1_PIN, LOW);
  digitalWrite(RELAY2_PIN, LOW);
  digitalWrite(RELAY3_PIN, LOW);
  digitalWrite(RELAY4_PIN, LOW);
  digitalWrite(RELAY5_PIN, LOW);
  digitalWrite(RELAY6_PIN, LOW);
  digitalWrite(RELAY7_PIN, LOW);
  digitalWrite(RELAY8_PIN, LOW);

  Wire.begin(OLED_SDA, OLED_SCL);
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.sendBuffer();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    u8g2.clearBuffer();
    u8g2.setFontMode(1);
    u8g2.setBitmapMode(1);
    u8g2.drawXBMP(1, 1, 19, 16, image_wifi_not_connected_bits);
    u8g2.sendBuffer();
  }
  Serial.println("Connected!");
  show_wifi_connected();

  server.on("/", handleRoot);
  server.on("/relay", handleRelay);
  server.on("/status", handleStatus);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void show_wifi_connected() {
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setBitmapMode(1);
  u8g2.drawXBMP(1, 1, 19, 16, image_wifi_5_bars_bits);
  u8g2.drawXBMP(80, 0, 11, 16, image_weather_humidity_white_bits);
  u8g2.drawXBMP(108, 0, 16, 16, image_weather_temperature_bits);
  updateRelayImages();
  u8g2.sendBuffer();
}

void handleRoot() {
  server.send(200, "text/plain", "Relay Control");
}

void handleRelay() {
  String relayParam = server.arg("id");
  String action = server.arg("action");

  int relayPin;
  if (relayParam == "1") relayPin = RELAY1_PIN;
  else if (relayParam == "2") relayPin = RELAY2_PIN;
  else if (relayParam == "3") relayPin = RELAY3_PIN;
  else if (relayParam == "4") relayPin = RELAY4_PIN;
  else if (relayParam == "5") relayPin = RELAY5_PIN;
  else if (relayParam == "6") relayPin = RELAY6_PIN;
  else if (relayParam == "7") relayPin = RELAY7_PIN;
  else if (relayParam == "8") relayPin = RELAY8_PIN;
  else {
    server.send(400, "text/plain", "Rele Invalid");
    return;
  }

  if (action == "on") {
    digitalWrite(relayPin, HIGH);
    server.send(200, "text/plain", "Relay " + relayParam + " ON");
    Serial.println("Relay " + relayParam + " ON");
  } else if (action == "off") {
    digitalWrite(relayPin, LOW);
    server.send(200, "text/plain", "Relay " + relayParam + " OFF");
    Serial.println("Relay " + relayParam + " OFF");
  } else {
    server.send(400, "text/plain", "Invalid action");
    return;
  }

  updateDisplay();
}

void handleStatus() {
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["relay1"] = digitalRead(RELAY1_PIN) == HIGH ? "ON" : "OFF";
  jsonDoc["relay2"] = digitalRead(RELAY2_PIN) == HIGH ? "ON" : "OFF";
  jsonDoc["relay3"] = digitalRead(RELAY3_PIN) == HIGH ? "ON" : "OFF";
  jsonDoc["relay4"] = digitalRead(RELAY4_PIN) == HIGH ? "ON" : "OFF";
  jsonDoc["relay5"] = digitalRead(RELAY5_PIN) == HIGH ? "ON" : "OFF";
  jsonDoc["relay6"] = digitalRead(RELAY6_PIN) == HIGH ? "ON" : "OFF";
  jsonDoc["relay7"] = digitalRead(RELAY7_PIN) == HIGH ? "ON" : "OFF";
  jsonDoc["relay8"] = digitalRead(RELAY8_PIN) == HIGH ? "ON" : "OFF";
  String jsonString;
  serializeJson(jsonDoc, jsonString);
  server.send(200, "application/json", jsonString);
}

void updateRelayImages() {
  if (digitalRead(RELAY1_PIN) == HIGH) {
    u8g2.drawXBMP(3, 23, 13, 16, image_lamp_on_bits);
  } else {
    u8g2.drawXBMP(3, 23, 13, 16, image_lamp_off_bits);
  }
  if (digitalRead(RELAY2_PIN) == HIGH) {
    u8g2.drawXBMP(3, 47, 13, 16, image_lamp_on_bits);
  } else {
    u8g2.drawXBMP(3, 47, 13, 16, image_lamp_off_bits);
  }
  if (digitalRead(RELAY3_PIN) == HIGH) {
    u8g2.drawXBMP(19, 23, 13, 16, image_lamp_on_bits);
  } else {
    u8g2.drawXBMP(19, 23, 13, 16, image_lamp_off_bits);
  }
  if (digitalRead(RELAY4_PIN) == HIGH) {
    u8g2.drawXBMP(19, 47, 13, 16, image_lamp_on_bits);
  } else {
    u8g2.drawXBMP(19, 47, 13, 16, image_lamp_off_bits);
  }
  if (digitalRead(RELAY5_PIN) == HIGH) {
    u8g2.drawXBMP(35, 23, 13, 16, image_lamp_on_bits);
  } else {
    u8g2.drawXBMP(35, 23, 13, 16, image_lamp_off_bits);
  }
  if (digitalRead(RELAY6_PIN) == HIGH) {
    u8g2.drawXBMP(35, 47, 13, 16, image_lamp_on_bits);
  } else {
    u8g2.drawXBMP(35, 47, 13, 16, image_lamp_off_bits);
  }
  if (digitalRead(RELAY7_PIN) == HIGH) {
    u8g2.drawXBMP(51, 23, 13, 16, image_lamp_on_bits);
  } else {
    u8g2.drawXBMP(51, 23, 13, 16, image_lamp_off_bits);
  }
  if (digitalRead(RELAY8_PIN) == HIGH) {
    u8g2.drawXBMP(51, 47, 13, 16, image_lamp_on_bits);
  } else {
    u8g2.drawXBMP(51, 47, 13, 16, image_lamp_off_bits);
  }

  
}

void updateDisplay() {
  u8g2.clearBuffer();
  show_wifi_connected();
  u8g2.sendBuffer();
}
