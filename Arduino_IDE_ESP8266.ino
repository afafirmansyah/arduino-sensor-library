/*****
 
 All the resources for this project:
 https://github.com/afafirmansyah/arduino-sensor-library
 
*****/

#define en_BZR_BTN 1
#define en_ENCODER_KY040 0
#define en_DHT_11 0
#define en_HC_SR04 0
#define en_BLINK 0

/*****
 
 Encoder Library - Basic Example
 http://www.pjrc.com/teensy/td_libs_Encoder.html
 
*****/

#if en_ENCODER_KY040
#include <Encoder.h>
// Change these two numbers to the pins connected to your encoder.
// Best Performance: both pins have interrupt capability
// Good Performance: only the first pin has interrupt capability
// Low Performance:  neither pin has interrupt capability

Encoder myEnc(4, 5);  // Avoid using pins with LEDs attached
long oldPosition  = -999;
#define BTN_3 0
#endif

/*****
 
 REQUIRES the following Arduino libraries:
 - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
 - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
 
*****/

#if en_DHT_11
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2  // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT11

// See guide for details on sensor wiring and usage:
// https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;
#endif

/*****
 
 Complete project details at 
 https://RandomNerdTutorials.com/esp32-hc-sr04-ultrasonic-arduino/
 
*****/

#if en_HC_SR04
#define trig 14
#define echo 13

// Define sound speed in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;
#endif

#define LED_1 1
#define LED_2 3
#define BTN_1 16
#define BTN_2 13
#define BZR 15

int BZR_enable = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(BZR, OUTPUT);
  pinMode(BTN_1, INPUT);
  pinMode(BTN_2, INPUT);
  digitalWrite(LED_1, HIGH);
  digitalWrite(LED_2, HIGH);

#if en_ENCODER_KY040
  Serial.begin(9600);
  pinMode(BTN_3, INPUT);
#endif

#if en_DHT_11
  Serial.begin(9600);
  dht.begin();  
  // Initialize device.
  sensor_t sensor;
  
  // Print temperature sensor details
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
#endif

#if en_HC_SR04
  Serial.begin(9600);
  pinMode(trig, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echo, INPUT);   // Sets the echoPin as an Input
#endif
}

void loop() {
// put your main code here, to run repeatedly:

#if en_ENCODER_KY040
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition);
  }
#endif

#if en_DHT_11
  // Delay between measurements.
  delay(delayMS);
  
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
#endif

#if en_HC_SR04
  // Clears the trigPin
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY / 2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);
  delay(1000);
#endif


#if en_BZR_BTN
  if (digitalRead(BTN_1) == 0) {
    digitalWrite(LED_1, LOW);
    BZR_enable = 1;
    BUZZER();
    while (digitalRead(BTN_1) != 1) {

    }
  }
  if (digitalRead(BTN_2) == 0) {
    digitalWrite(LED_1, HIGH);
    BZR_enable = 1;
    BUZZER();
    while (digitalRead(BTN_2) != 1) {

    }
  }
#endif

#if en_BLINK
  digitalWrite(LED_2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                 // wait for a second
  digitalWrite(LED_2, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                 // wait for a second
#endif
}

void BUZZER() {
  if (BZR_enable == 1) {
    digitalWrite(BZR, HIGH);
    delay(100);
    digitalWrite(BZR, LOW);
    BZR_enable = 0;
  }
}
