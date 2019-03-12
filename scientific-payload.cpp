#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);
Adafruit_L3GD20_Unified       gyro  = Adafruit_L3GD20_Unified(20);

RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


// setup code

void setup() {

  Serial.begin(9600);
  Serial.print("Initializing Datalogger and RTC...");

  pinMode(53, OUTPUT);

  if (!SD.begin(53)) {
    Serial.println("Couldn't find SD");
    while (1);
  };
    if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  };

rtc.adjust(DateTime(2016, 11, 21, 13, 19, 0));

  Serial.println("done.");

// initialize sensors

  sensor_t sensor;
  accel.getSensor(&sensor);
  gyro.getSensor(&sensor);
  mag.getSensor(&sensor);
  bmp.getSensor(&sensor);

  delay(500);

  accel.begin();
  mag.begin();
  bmp.begin();
  gyro.begin();

}


// data loop

void loop() {

  sensors_event_t event;

  String accelString = "";
  String chemString = "";
  String magString = "";
  String gyroString = "";

// read x, y, z acceleration

  accel.getEvent(&event);
  accelString = String(event.acceleration.x) + String(" ")
                 + String(event.acceleration.y) + String(" ")
                     + String(event.acceleration.z) + String(" ");

// read temperature, pressure, altitude

  bmp.getEvent(&event);
  if (event.pressure)
  {
    // collect atmospheric pressure in hPa

    String pressure = String(event.pressure);

    // collect ambient temperature in C

    float temperature;
    bmp.getTemperature(&temperature);
    String temp = String(temperature);

    // reference current sea level pressure

    float seaLevelPressure = 1013.25;

    // compute pressure differential to read altitude

    String altitude = String(bmp.pressureToAltitude(seaLevelPressure,
                             event.pressure,
                             temperature));

    chemString = pressure + String(" ")
                              + temp + String(" ")
                                         + altitude + String(" ");
  }

// read x, y, z magnetic field

  mag.getEvent(&event);
  magString = String(event.magnetic.x) + String(" ")
               + String(event.magnetic.y ) + String(" ")
                  + String(event.magnetic.z) + String(" ");

// read roll, pitch, yaw

  gyro.getEvent(&event);
  gyroString = String(event.gyro.x) + String(" ")
                + String(event.gyro.y) + String(" ")
                  + String(event.gyro.z) + String(" ");

// print data to log

  Serial.println("Acceleration String");
  Serial.println(accelString);
  Serial.println("Chem String");
  Serial.println(chemString);
  Serial.println("Mag String");
  Serial.println(magString);
  Serial.println("Gyro String");
  Serial.println(gyroString);


// concatenate data into single row

  String dataString = accelString
                    + chemString
                    + magString
                    + gyroString;

  Serial.println("Total String");
  Serial.println(dataString);



// read time


    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    Serial.print("Unixtime = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");

// delay in milliseconds

  delay(100);

// write data to SD card

  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (!dataFile) {
    Serial.println("couldn't open dataFile");
    while (1);
  } else {
    dataFile.println(dataString);
    dataFile.close();
  }
}
