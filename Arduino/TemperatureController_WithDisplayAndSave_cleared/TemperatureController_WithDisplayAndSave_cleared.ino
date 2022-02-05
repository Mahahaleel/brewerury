//Include libraries for DHT sensor and display
#include "DHT.h" //DHT sensor
#include "TM1637.h" //Display
#include <ZzzMovingAvg.h> //Moving average: https://github.com/seralo/ZzzMovingAvg

//define moving average settings
ZzzMovingAvg <8, float, float> MovingAverageTemp;
int MovAv_counter;
float MovingAv;


//Defining variables of the display
int CLK = 8;
int DIO = 7;
//define output DHT sensor.
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

//anne definition of tm (display)
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
float Tdelay;
int NickTalkLength=10; //amount of variables exported to pi

//DataLogger SD 
#include <SD.h>
#include <SPI.h>
File DataLogger;
int pinCS=10;

// used pins:2,4,7,8,9,10,11,12,13,analog1
void setup() {
  Tset=21;                //desired temperature
  Tdelay=10000;           //data log&measure interval (ms)
  Serial.begin(9600);
  Serial.println(F("Beste programma V1.4 nu met moving av."));
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

  // Reading temperature or humidity
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

//Display reset
int dig1 = 0;
int dig2 = 0;
int dig3 = 0;

//DHT reading; Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  //float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, true);
  
  // add to moving average average
    MovingAverageTemp.add(T_Therm);
    MovingAv= MovingAverageTemp.get();
  
  ///// Switching of light and fan based on measured data of moving average of the thermistor, bucket temperature
  if (MovingAv > Tset+0.1) {
    digitalWrite(9, LOW);
    digitalWrite(4, LOW);
    LampBool=0;
    ;
  }
  if (MovingAv < Tset-0.3) {
    digitalWrite(9, HIGH);
    digitalWrite(4, HIGH);
    LampBool=1;
    ;
  }

  //print output to internal port
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
  Serial.print ("Moving average Check: ");
  Serial.print (MovingAv);
  Serial.print (" Origin moving average: ");
  Serial.println (MovingAverageTemp.get());

//log data to SD kaart
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
    DataLogger.print(MovingAverageTemp.get());    
    DataLogger.print(";");
    DataLogger.println(LampBool);
    DataLogger.close(); // close the file
    

    string NickTalk = 'Grafana Measurements=Counter:' + Counter + "ThermistorTemperature:"+  T_Therm+ ";AirTemperature:"+  t+  ";MovingAverageTemp:"+  
    MovingAverageTemp.get()+ "LampBool:"+ LampBool
    Serial.println(NickTalk)
    //char NickTalk[]={'Grafana Measurements=Counter:', Counter, "ThermistorTemperature:",  T_Therm, ";AirTemperature:",  t,  ";MovingAverageTemp:",  
    //MovingAverageTemp.get(), "LampBool:", LampBool};

    //for (int i = 0; i < NickTalkLength; i++) Serial.print(NickTalk[i]);
    //Serial.println(NickTalk[2]);

    
    Serial.println("Done.");

//    Serial.println("Grafana Measurements=Counter:" Counter ";ThermistorTemperature:"  T_Therm ";AirTemperature:"  t  ";MovingAverageTemp:"  MovingAverageTemp.get() ";LampBool:" LampBool)
  }
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening test.txt");
  }


//print to screen
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
  delay(Tdelay);
  
}
