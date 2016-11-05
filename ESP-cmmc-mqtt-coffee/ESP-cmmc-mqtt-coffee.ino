#include <Arduino.h>
#include <CMMC_Manager.h>
#include "CMMC_Interval.hpp"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <MqttConnector.h>
#include "init_mqtt.h"
#include <Servo.h> 
Servo myservo;

#define SERVO 14

boolean stateCoffee = false;
#include "_publish.h"
#include "_receive.h"
const char* MQTT_HOST        = "mqtt.espert.io";
const char* MQTT_USERNAME    = "";
const char* MQTT_PASSWORD    = "";
const char* MQTT_CLIENT_ID   = "";
const char* MQTT_PREFIX      = "/CMMC";
const int MQTT_PORT           = 1883;
const int PUBLISH_EVERY       = 6000;

/* DEVICE DATA & FREQUENCY */
const char *DEVICE_NAME = "CMMC-ROOM-COFFEE";

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#define DHTPIN 12     // what digital pin we're connected to

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

CMMC_Interval timer001;
MqttConnector *mqtt;


float t_dht = 0;
float h_dht = 0;

static void read_dht() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
   Serial.println("Failed to read from DHT sensor!");
   return;
  }
  else {
    t_dht = t;
    h_dht = h;
    Serial.print("Temp: ");
    Serial.println(t_dht);
    Serial.print("Humid: ");
    Serial.println(h_dht);
  }
}

void init_hardware()
{
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println("Starting...");
  myservo.attach(SERVO);
  myservo.write(50);
  pinMode(LED_BUILTIN, OUTPUT);


  delay(200);
  dht.begin();
}

void setup()
{
  init_hardware();
  CMMC_Manager manager(0, LED_BUILTIN);
  manager.start();
  init_mqtt();
}

void loop()
{
  mqtt->loop();
//  timer001.every_ms(2000, read_dht);
}
