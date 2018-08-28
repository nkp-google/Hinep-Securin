
#include <LCD5110_Graph.h>
#include <stdio.h>
#include <SimpleDHT.h>
#include <Wire.h>
#include <LSM6.h>
#include <LIS3MDL.h>
#include <LPS.h>
#include <Adafruit_GPS.h>

LCD5110 myGLCD(52,50,51,48,53);
LSM6 imu;
LIS3MDL mag;
LPS ps;

extern unsigned char SmallFont[];
extern unsigned char TinyFont[];

float y;
uint8_t* bm;
int pacy;

int pinDHT11 = 35;
SimpleDHT11 dht11;

char bufferT[10];
char bufferH[10];
char reportAcc[80];
char reportMag[80];
char buffermBar[10];

int y0;
int y1;
int y2;
int y3;

#define GPSSerial Serial1
Adafruit_GPS GPS(&GPSSerial);
#define GPSECHO false
uint32_t timer = millis();

void setup()
{
  Serial.begin(115200);
  GPS.begin(9600);
  Serial2.begin(9600);
  
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  randomSeed(analogRead(7));
  
  Wire.begin();
  pinMode(34, OUTPUT);
  
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  GPSSerial.println(PMTK_Q_RELEASE);

if (!imu.init())
  {
    Serial.println("Failed to detect and initialize IMU!");
    while (1);
  }
  imu.enableDefault();
{
   if (!mag.init())
  {
    Serial.println("Failed to detect and initialize magnetometer!");
    while (1);
  }

  mag.enableDefault();
  {
      if (!ps.init())
  {
    Serial.println("Failed to autodetect pressure sensor!");
    while (1);
  }

  ps.enableDefault();

}
  }
}



void loop()

 {
  char c = GPS.read();
  if (GPSECHO)
    if (c) Serial.print(c);
  if (GPS.newNMEAreceived()) {
    //Serial.println(GPS.lastNMEA()); 
    if (!GPS.parse(GPS.lastNMEA())) 
    return;
  }
  if (timer > millis()) timer = millis();
     
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
    Serial.print("\nTime: ");
    Serial.print(GPS.hour, DEC); Serial.print(':');
    Serial.print(GPS.minute, DEC); Serial.print(':');
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
      Serial.println();
 {
  ble();
 {
   lcd1();
   {
    tempDHT();
    imu1();
    mag1();
    baro();
  //  {
  //digitalWrite(34, HIGH); 
 // delay(500); 
 // digitalWrite(34, LOW);  
 // delay(500);  
//}
}
}
}
}
}
}



void ble()
// BLE HM-10
 {
    if (Serial2.available()) {
    Serial.write(Serial2.read());
   
  }
    if (Serial.available()) {
    Serial2.write(Serial.read());
  }}

  void tempDHT()
  // temperatur sensor DHT-11
  {
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return;
  }
  Serial.print("Temperature ");Serial.print((int)temperature); Serial.print(" *C ,"); 
  Serial.print("Humidity ");Serial.print((int)humidity); Serial.println(" H");
  dtostrf(temperature,2,0,bufferT);
  dtostrf(humidity,2,0,bufferH);
  Serial.println();
  delay(1500);
}

void imu1()
// LSM6DS33 3-axis accelrometer and gyroscope
{
  imu.read();

  snprintf(reportAcc, sizeof(reportAcc), "Accelerometer: %6d %6d %6d    Gyrometer: %6d %6d %6d",
  imu.a.x, imu.a.y, imu.a.z,
  imu.g.x, imu.g.y, imu.g.z);
  Serial.println(reportAcc);
  Serial.println();
  delay(100);
}

void mag1()
// LIS3MDL 3-axis magnetometer
{
  mag.read();

  snprintf(reportMag, sizeof(reportMag), "Magnetometer: %6d %6d %6d",
    mag.m.x, mag.m.y, mag.m.z);
  Serial.println(reportMag);
  Serial.println();
  delay(100);
}

void baro()
// LPS25H barometer
{
  float pressure = ps.readPressureMillibars();
  float altitude = ps.pressureToAltitudeMeters(pressure);
  float temperature = ps.readTemperatureC();
  
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.print(" mbar\ta: ");
  Serial.print(altitude);
  Serial.print(" m\tt: ");
  Serial.print(temperature);
  Serial.println(" deg C");
  dtostrf(pressure, 5, 0, buffermBar);
  Serial.println();
  delay(100);
}

void gps()
{
  char c = GPS.read();
  if (GPSECHO)
    if (c) Serial.print(c);
  if (GPS.newNMEAreceived()) {
    Serial.println(GPS.lastNMEA()); 
    if (!GPS.parse(GPS.lastNMEA())) 
    return;
  }
  if (timer > millis()) timer = millis();
     
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
    Serial.print("\nTime: ");
    Serial.print(GPS.hour, DEC); Serial.print(':');
    Serial.print(GPS.minute, DEC); Serial.print(':');
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    }
  }
}

void lcd1()
{
  y0 = (0, 0);  /// change to move position of scrolling text
  y1 = (0, 16);
  y2 = (0, 24);
  y3 = (0, 32);
  for (int i=84; i>=-(34*6); i--)
 { 
  myGLCD.print("ATOM R/C EXAMPLE",i, y0);
  myGLCD.update();
  delay(50);
  {
   myGLCD.print(buffermBar, 0, 40);
   myGLCD.print("Mbar",40, 40);
   myGLCD.update();
   delay(50);
  {
  myGLCD.print(bufferT, 28, 8);
  myGLCD.print("Temp",0, 8);
  myGLCD.print(bufferH, 66, 8);
  myGLCD.print("Hum",45, 8);
  myGLCD.print("BLE Active", i, y1);
  myGLCD.print("IMU Active", i, y2);
  myGLCD.print("GPS Active", i, y3);
  myGLCD.update();
  delay(20);
 }
 }
}
}



