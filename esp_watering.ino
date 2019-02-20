#include <ETH.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiSTA.h>
#include <WiFiType.h>
#include <WiFiUdp.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WET 400
#define DRY 900
#define volumePin A0
#define MOS 5
#define PERIOD 259200
#define WORK 60
#define MQTT_NAME ""
#define MQTT_PASS ""
#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define WIFI_SSID ""
#define WIFI_PASS ""
 
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);
Adafruit_MQTT_Subscribe onoff = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/flower");

uint32_t mainTimer, myTimer;
boolean state = false;
void setup()
{
  Serial.begin(115200);
  Serial.print("\n\nConnecting Wifi... ");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  Serial.println("OK!");
  mqtt.subscribe(&onoff);
}

void loop()
{
  MQTT_connect();
  
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    if (subscription == &onoff)
    {
      Serial.print("onoff: ");
      Serial.println((char*) onoff.lastread);
      if (!strcmp((char*) onoff.lastread, "ON"))
      {
digitalWrite(MOS, HIGH);
delay(5000);
digitalWrite(MOS, LOW);
      }
      else
      {
Serial.print("OFFFFF: ");
      }
    }
  }
  if (!mqtt.ping())
  {
    mqtt.disconnect();
  }
}

void MQTT_connect() 
{
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) 
  {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  { 
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) 
       {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
