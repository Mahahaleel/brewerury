//Include libraries 
#include "DHT.h" //DHT sensor //for DHT sensor and display of adafruit
#include "TM1637.h" //Display https://github.com/avishorp/TM1637
#include <ZzzMovingAvg.h> //Moving average: https://github.com/seralo/ZzzMovingAvg

//define moving average settings
ZzzMovingAvg <8, float, float> MovingAverageTemp;
int MovAv_counter;
float MovingAv;


// //Defining variables of the display
// int CLK = 8;  
// int DIO = 7;
// //define output DHT sensor.
// #define DHTPIN 2     // Digital pin connected to the DHT sensor
// #define DHTTYPE DHT11   // DHT 11

// // Initialize DHT sensor.
// DHT dht(DHTPIN, DHTTYPE);

// //anne definition of tm (display)
// TM1637 tm(CLK,DIO);

//Thermistor Setup
int ThermistorPin = 0;
int Vo;

float R1 = 10000;
float logR2, R2, T_Therm;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

// define variables
float Tset;
float Tdelay;
int NickTalkLength=10; //amount of variables exported to pi
int Counter;
int LampBool;
int FanDelay;
int FanDelayCounter;

// //DataLogger SD 
// #include <SD.h>
// #include <SPI.h>
// File DataLogger;
// int pinCS=10;

// used pins:0,2,7,8,9,10,11,12,13,analog1
void setup() {
/// SETUP START PARAMETERS

    Tset=21;                //desired temperature 
    Tdelay=10000;           //data log&measure interval (ms)
    FanDelay=45;            //Amount of cycles (Tdelay) after which the fan stop
    Serial.begin(9600);
    Serial.println(F("Beste programma V1.73 nu opgeschoond."));
//    pinMode(8, OUTPUT);    // sets the digital pin 8 as output
    pinMode(8, OUTPUT);    // sets the digital pin 8 as output for power switch 
    pinMode(7, OUTPUT);    // sets the digital pin 7 as output for Fan
    //pinMode(2,OUTPUT);    // Used for T/H sensor
    // pinMode(7,OUTPUT);    // Display port
    // pinMode(8, OUTPUT);    // Display port
    // CS of SD card module to pin 10 of Arduino
    // MOSI of SD card module to pin 11 of Arduino
    // MISO of SD card module to pin 12 of Arduino
    // SCK of SD card module to pin 13 of Arduino
//    pinMode(pinCS, OUTPUT);
  /// SETUP START PARAMETERS

  // // SD Card Initialization
  // if (SD.begin())
  // {
  //   Serial.println("SD card is ready to use.");
  // } else
  // {
  //   Serial.println("SD card initialization failed");
  //   return;
  // }
 // rtc.begin(); 
 Counter=0;  
 LampBool=0;
 
 //analog 1             //Thermistor input
  // dht.begin();


  // //anne

  //   // put your setup code here, to run once:
  // tm.init();

  // // set brightness; 0-7
  // tm.set(2);

}

void loop() {
  // // Reading temperature or humidity
  // float h = dht.readHumidity();
  // // Read temperature as Celsius (the default)
  // float t = dht.readTemperature();
  // // Read temperature as Fahrenheit (isFahrenheit = true)
  // //float f = dht.readTemperature(false);

// Test Lamp part;
//  if(Counter%2==0) {
//    digitalWrite(8, LOW);
//  } else {
//    digitalWrite(8, HIGH);
//  };
  
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

// //Display reset
// int dig1 = 0;
// int dig2 = 0;
// int dig3 = 0;

// //DHT reading; Check if any reads failed and exit early (to try again).
//   if (isnan(h) || isnan(t) ) {
//     Serial.println(F("Failed to read from DHT sensor!"));
//     return;
//   }

  // // Compute heat index in Fahrenheit (the default)
  // //float hif = dht.computeHeatIndex(f, h);
  // // Compute heat index in Celsius (isFahreheit = false)
  // float hic = dht.computeHeatIndex(t, h, true);
  
  // add to moving average average
    MovingAverageTemp.add(T_Therm);
    MovingAv= MovingAverageTemp.get();
  
  ///// Switching of light and fan based on measured data of moving average of the thermistor, bucket temperature
  if (MovingAv > Tset+0.1) {
    digitalWrite(8, HIGH);
      Serial.println(("No Heating"));
    LampBool=0;
    ;
  }
  if (MovingAv < Tset-0.3) {
    digitalWrite(8, LOW);
      Serial.println(("Heating on"));
    LampBool=1;
    ;
  }

 if (LampBool==1) {
    digitalWrite(7, LOW);
    Serial.println(("Fan on"));
    FanDelayCounter=0;
  }
 if (LampBool==0) {
    FanDelayCounter=FanDelayCounter+1;
    if (FanDelayCounter>FanDelay) {
      Serial.println(("Fan off"));
      digitalWrite(7, HIGH);
    }
  }
  
  //print output to internal port
  Serial.print(F("Temperature from thermistor: "));
  Serial.println(T_Therm);
  // Serial.println(F("Humidity: "));
  // Serial.print(h);
  // Serial.print("10");
  // Serial.println(F("Temperature: "));
  // Serial.print(t);
  // Serial.print("10");

  // Serial.print(F("    °C "));
  //Serial.print(f);
  // Serial.println(F("Heat index: "));
  // Serial.print(hic);
  // Serial.print("10");
  // Serial.println(F(" °C "));
  //Serial.print(hif);
  //Serial.println(F("°F"));
  Serial.print ("Moving average Check: ");
  Serial.println (MovingAv);
  Serial.print ("Origin moving average: ");
  Serial.println (MovingAverageTemp.get());

  // To Fix database script
  Serial.print("Writing to file... Counter: ");
  Serial.println(Counter);

  Serial.print("Fan Delay Counter: ");
  Serial.println(FanDelayCounter);
  Serial.println("  ");

  Serial.print("Lampbool: ");
  Serial.println(LampBool);
  Serial.println("  ");

// //log data to SD kaart
//    // Create/Open file 
//   DataLogger = SD.open("test.txt", FILE_WRITE);
  
  // // if the file opened okay, write to it:
  // if (DataLogger) {
  //   // Serial.print("Writing to file... Counter: ");
  //   // Serial.println(Counter);
  //   // Write to file
  //   DataLogger.print(Counter);
  //   DataLogger.print(";");
  //   DataLogger.print(t);
  //   DataLogger.print(";");
  //   DataLogger.print(T_Therm);
  //   DataLogger.print(";");
  //   DataLogger.print(MovingAverageTemp.get());    
  //   DataLogger.print(";");
  //   DataLogger.println(LampBool);
  //   DataLogger.close(); // close the file


    //wtf is this
//    String NickTalk = 'Grafana Measurements=Counter:' + Counter + "ThermistorTemperature:"+  T_Therm+ ";AirTemperature:"+  t+  ";MovingAverageTemp:"+  
//   MovingAverageTemp.get()+ "LampBool:"+ LampBool
//   Serial.println(NickTalk)
    //char NickTalk[]={'Grafana Measurements=Counter:', Counter, "ThermistorTemperature:",  T_Therm, ";AirTemperature:",  t,  ";MovingAverageTemp:",  
    //MovingAverageTemp.get(), "LampBool:", LampBool};

    //for (int i = 0; i < NickTalkLength; i++) Serial.print(NickTalk[i]);
    //Serial.println(NickTalk[2]);

    
//     Serial.println("Done.");

// //    Serial.println("Grafana Measurements=Counter:" Counter ";ThermistorTemperature:"  T_Therm ";AirTemperature:"  t  ";MovingAverageTemp:"  MovingAverageTemp.get() ";LampBool:" LampBool)
//   }
//   // if the file didn't open, print an error:
//   else {
//     Serial.println("error opening test.txt");
  // }


// //print to screen
// dig1 = T_Therm/10;
// dig2 = T_Therm - (dig1 * 10);
// dig3 = (T_Therm * 10) - (dig1 * 100) - (dig2 * 10); 

// //Serial.print(dig3);
  
//   tm.display(0,dig1);
//   tm.display(1,dig2);
//   tm.point(1);
//   tm.display(2,dig3);
//   tm.display(3,12);

  // Wait a few seconds between measurements.
  delay(Tdelay);
  
}
