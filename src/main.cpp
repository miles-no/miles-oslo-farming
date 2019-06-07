
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Moisture
#define MOISTPIN 0

// DHT
#define DHTPIN 2    
#define DHTTYPE    DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);

//Blynk
#define BLYNK_PRINT Serial
char auth[] = "c2e366785e3b45d98c5daf0caaf93b73";
BlynkTimer timer;

//WIFI
char ssid[] = "<ssid>";
char pass[] = "<password>";

void sendSensorData() {
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);

  float temperature = 0;
  float humidity = 0;
  
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    temperature = event.temperature;
    Serial.print(F("Temperature: "));
    Serial.print(temperature);
    Serial.println(F("°C"));

  }

  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    humidity = event.relative_humidity;
    Serial.print(F("Humidity: "));
    Serial.print(humidity);
    Serial.println(F("%"));
  }

  Serial.print("Soil moisture: ");
  float soil= analogRead(MOISTPIN);
  Serial.println(soil);
  
  if(soil > 500) {
    Blynk.virtualWrite(V4, HIGH);
  } else {
    Blynk.virtualWrite(V4, LOW);
  }

  Blynk.virtualWrite(V5, temperature);
  Blynk.virtualWrite(V6, humidity);
}

void setup() {
  Serial.begin(9600);
  // Initialize device.
  Blynk.begin(auth, ssid, pass);
  dht.begin();

  timer.setInterval(1000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}