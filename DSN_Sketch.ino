#include "thingProperties.h"
#include <Arduino_MKRIoTCarrier.h>
#include <EduIntro.h>

MKRIoTCarrier carrier;
DHT11 dht11(A6);
int PIR_PIN;

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  //Get Cloud Info/errors , 0 (only errors) up to 4
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  //Wait to get cloud connection to init the carrier
  while (ArduinoCloud.connected() != 1) {
    ArduinoCloud.update();
    delay(500);
  }

  delay(500);
  CARRIER_CASE = true;
  carrier.begin();
  delay(1500);
  PIR_PIN = carrier.getBoardRevision() == 1 ? A5 : A0;
  pinMode(PIR_PIN, INPUT);
}

void loop() {
  while (!carrier.Light.colorAvailable()) {
    delay(5);
  }
  int none;
  carrier.Light.readColor(none, none, none, illuminance);
  pIRMotion = digitalRead(PIR_PIN);
  
  
  tempInsideDevice = carrier.Env.readTemperature();
  humInsideDevice = carrier.Env.readHumidity();
  airPressure = carrier.Pressure.readPressure();
  pIRMotion = digitalRead(PIR_PIN);
  dht11.update();
  tempOutsideDevice = dht11.readCelsius();
  humOutsideDevice = dht11.readHumidity();
  
  ArduinoCloud.update();
  
  delay(1000);
}