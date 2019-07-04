#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Moisture Configuration
#define MOISTPIN 0

// DHT Configuration
#define DHTPIN 2    
#define DHTTYPE    DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);

//Blynk
#define BLYNK_PRINT Serial
BlynkTimer timer;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char blynk_token[] = "YourAuthToken";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";


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

  Serial.print("Light? ");
  int light = digitalRead(D2);
  Serial.println(light);


  Blynk.virtualWrite(V5, temperature);
  Blynk.virtualWrite(V6, humidity);
}

void setup() {
  Serial.begin(9600);

  // Initialize device.
  Blynk.begin(blynk_token, ssid, pass);
  dht.begin();

  pinMode(D2, INPUT);

  timer.setInterval(1000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}