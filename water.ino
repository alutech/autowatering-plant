#include <ESP8266WiFi.h>


#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define volumePin A0
#define MOS 14
#define LINK 12
#define SCAN 15
#define WIFI_link 13
#define PERIOD 259200
#define WORK 60
#define MQTT_NAME "pentalgin90"
#define MQTT_PASS "b50a674e8dff4d8baf69d6faaf062f9d"
#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define WIFI_SSID "MiA2"
#define WIFI_PASS "WlanPass"


WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);
Adafruit_MQTT_Subscribe onoff = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/flower");

uint32_t mainTimer, myTimer;
boolean state = false;
void setup()
{
  Serial.begin(9600);
  Serial.println("\n\nConnecting Wifi... ");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  Serial.println("OK!");
  mqtt.subscribe(&onoff);
  pinMode(MOS, OUTPUT);
  pinMode(SCAN, INPUT);
  pinMode(WIFI_link, OUTPUT);
  digitalWrite(WIFI_link, LOW);
  pinMode(LINK, OUTPUT);
  digitalWrite(LINK, LOW);


}

void loop()
{
  Serial.println(digitalRead(SCAN));
  if((digitalRead(SCAN)) == 0){
    digitalWrite(LINK, HIGH);
  }else{
    digitalWrite(LINK, LOW);
  }
  if(WiFi.status()== 3){
    digitalWrite(WIFI_link, HIGH);
  }else if(WiFi.status()== 1){
    digitalWrite(WIFI_link, HIGH);
    delay(100);
    digitalWrite(WIFI_link, LOW);
    delay(100);
    MQTT_connect();
  }else{
    digitalWrite(WIFI_link, LOW);
    MQTT_connect();
  }
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
Serial.println(digitalRead(MOS));
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