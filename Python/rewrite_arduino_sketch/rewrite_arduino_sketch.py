
# filePath = "/arduino_brewerury.ino"
filePath = "D:/GIT/PersonalRepos/brewerury/Python/rewrite_arduino_sketch/arduino_brewerury.ino"
temperature_set = 19        # graden
measure_interval = 5000     # miliseconden
fan_delay = 24              # cycles (*measure_interval)

with open(filePath, 'r') as file:
    data = file.read()

# print(data)

print(data.split("/// SETUP START PARAMETERS")[0])

parameters = """
/// SETUP START PARAMETERS

  Tset={0};                //desired temperature
  Tdelay={1};           //data log&measure interval (ms)
  FanDelay={2};            //Amount of cycles (Tdelay) after which the fan stop
  Serial.begin(9600);
  Serial.println(F("Beste programma V1.6 nu met fanDelay."));
  //pinMode(8, OUTPUT);    // sets the digital pin 8 as output
  pinMode(9, OUTPUT);    // sets the digital pin 9 as output for power switch 
  pinMode(4, OUTPUT);    // sets the digital pin 4 as output for Fan
  //pinMode(2,OUTPUT);    // Used for T/H sensor
  pinMode(7,OUTPUT);    // Display port
  pinMode(8, OUTPUT);    // Display port
  // CS of SD card module to pin 10 of Arduino
  // MOSI of SD card module to pin 11 of Arduino
  // MISO of SD card module to pin 12 of Arduino
  // SCK of SD card module to pin 13 of Arduino
  pinMode(pinCS, OUTPUT);
/// SETUP START PARAMETERS
""".format(temperature_set, measure_interval, fan_delay)

new_data = data.split("/// SETUP START PARAMETERS")[0] + parameters + data.split("/// SETUP START PARAMETERS")[2]

print(new_data)

with open(filePath, "w") as file:
    file.write(new_data)