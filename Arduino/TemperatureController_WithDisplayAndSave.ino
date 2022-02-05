// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"

//including library of the display
#include "TM1637.h"

//Defining variables of the display
int CLK = 8;
int DIO = 7;

#define DHTPIN 2     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

//anne definition of tm
TM1637 tm(CLK,DIO);

//Thermistor Setup
int ThermistorPin = 0;
int Vo;
float Counter;
int LampBool;
float R1 = 10000;
float logR2, R2, T_Therm;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
float Tset;

//DataLogger SD 
#include <SD.h>
#include <SPI.h>
File DataLogger;
int pinCS=10;

// used pins:2,4,7,8,9,10,11,12,13,analog1
void setup() {
  Tset=19;
  Serial.begin(9600);
  Serial.println(F("Beste programma V1.2"));
  pinMode(8, OUTPUT);    // sets the digital pin 8 as output
  pinMode(9, OUTPUT);    // sets the digital pin 9 as output
  pinMode(4, OUTPUT);    // sets the digital pin 4 as output for power switch
  //pinMode(2,OUTPUT);    // Used for T/H sensor
  //pinMode(7,OUTPUT);    // Display port
  //pinMode(8, OUTPUT);    // Display port
  // CS of SD card module to pin 10 of Arduino
  // MOSI of SD card module to pin 11 of Arduino
  // MISO of SD card module to pin 12 of Arduino
  // SCK of SD card module to pin 13 of Arduino
  pinMode(pinCS, OUTPUT);

  // SD Card Initialization
  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
 // rtc.begin(); 
 Counter=0;  
 LampBool=0;
 
 //analog 1             //Thermistor input
  dht.begin();


  //anne

    // put your setup code here, to run once:
  tm.init();

  // set brightness; 0-7
  tm.set(2);

}

void loop() {


  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float f = dht.readTemperature(false);

  
  //Thermistor measurement
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T_Therm = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T_Therm = T_Therm - 273.15;
  T_Therm = (T_Therm * 9.0)/ 5.0 + 32.0; 
  T_Therm = (T_Therm - 32) * 5/9;
  Counter=Counter+1;
  
  Serial.print("Temperature thermistor: "); 
  Serial.print(T_Therm);
  Serial.println(" C"); 



  
//anne
int dig1 = 0;
int dig2 = 0;
int dig3 = 0;

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  //float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, true);

  ///// switching
  // light control based on humidity
  if (h > 70) {
    //digitalWrite(9, HIGH);
    //digitalWrite(4, HIGH);
    ;
  }
  if (h < 65) {
    //digitalWrite(9, LOW);
    //digitalWrite(4, LOW);
    ;
  }

  //temperature switch red light
  if (T_Therm > Tset+0.3) {
    digitalWrite(9, LOW);
    digitalWrite(4, LOW);
    LampBool=0;
    ;
  }
  if (T_Therm < Tset-0.3) {
    digitalWrite(9, HIGH);
    digitalWrite(4, HIGH);
    LampBool=1;
    ;
  }
  
  Serial.print(F("Temperature from thermistor: "));
  Serial.print(T_Therm);
  Serial.print(F("°C Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  //Serial.print(f);
  Serial.print(F(" Heat index: "));
  Serial.print(hic);
  Serial.println(F("°C "));
  //Serial.print(hif);
  //Serial.println(F("°F"));

   // Create/Open file 
  DataLogger = SD.open("test.txt", FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (DataLogger) {
    Serial.print("Writing to file... Counter: ");
    Serial.println(Counter);
    // Write to file
    DataLogger.print(Counter);
    DataLogger.print(";");
    DataLogger.print(t);
    DataLogger.print(";");
    DataLogger.print(T_Therm);
    DataLogger.print(";");
    DataLogger.println(LampBool);
    DataLogger.close(); // close the file
    Serial.println("Done.");
  }
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening test.txt");
  }


// anne
// example

dig1 = T_Therm/10;
dig2 = T_Therm - (dig1 * 10);
dig3 = (T_Therm * 10) - (dig1 * 100) - (dig2 * 10); 

//Serial.print(dig3);
  
  tm.display(0,dig1);
  tm.display(1,dig2);
  tm.point(1);
  tm.display(2,dig3);
  tm.display(3,12);

  // Wait a few seconds between measurements.
  delay(30000);
  
}
