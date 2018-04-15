#include <Arduino.h>
#include "SSD1306.h"
#include "SSD1306Brzo.h"
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESPHue.h>

#include "settings.h"

SSD1306Brzo display(0x3c, D4, D5);

OneWire oneWire(D3);
DallasTemperature sensors(&oneWire);

ESPHue h = ESPHue(HUE_IP, HUE_USERNAME);


void setup() {
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 0, "Hi Tristan");
  display.display();

  WiFi.mode(WIFI_STA);

  // Initiate connection with SSID and PSK
  WiFi.hostname("bathmonitor");
  WiFi.begin(WIFI_SSID, WIFI_PSK);

  while (WiFi.status() != WL_CONNECTED) {
    display.clear();
    display.drawString(0, 0, "Connecting...");
    display.display();
  }
  sensors.begin();

  h.off();
  h.set(LAMP);
}

void loop() {
  float temp;
  int colour;
  char buff[10];
  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);
  display.clear();
  snprintf(buff, sizeof(buff), "%.3f c", temp);
  display.drawString(15, 0, buff);
  colour = (temp-20)*1632*3; // todo - play with this range
  snprintf(buff, sizeof(buff), "%d hue", colour);
  display.drawString(15, 23, buff);
  display.display();
  h.HSB(colour, 255, 255); //configure hsb
  h.on(); //configure on
  h.set(LAMP); //apply set hsb to lamp 2
}
