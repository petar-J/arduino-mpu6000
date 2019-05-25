// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// MPU-6000
// This code is designed to work with the MPU-6000_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Accelorometer?sku=MPU-6000_I2CS#tabs-0-product_tabset-2

#include <Wire.h>
int xGyro, yGyro, zGyro, xAccl, yAccl, zAccl;

// MPU-6000 I2C address is 0x68(104b)
#define Addr 0x69
int threshold = 4000;
int brojKoraka = 0;
bool pic = false ;
int dist = 0;
int md = 150;
int d1 = 190, d2 = 190, d3 = 190, d4 = 190;
int acCoef = 10;

void setup()
{
  // Initialise I2C communication as Master
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);
  
  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Select gyroscope configuration register
  Wire.write(0x1B);
  // Full scale range = 2000 dps
  Wire.write(0x18);
  // Stop I2C transmission
  Wire.endTransmission();
  
  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Select accelerometer configuration register
  Wire.write(0x1C);
  // Full scale range = +/-16g
  Wire.write(0x18);
  // Stop I2C transmission
  Wire.endTransmission();
  
  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Select power management register
  Wire.write(0x6B);
  // PLL with xGyro reference
  Wire.write(0x01);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(300);
}

void loop()
{
  unsigned int data[6];

  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x3B);
  // Stop I2C transmission
  Wire.endTransmission();
  
  // Request 6 bytes of data
  Wire.requestFrom(Addr, 6);
  
  // Read 6 byte of data 
  if(Wire.available() == 6)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read(); 
  }
  
  // Convert the data
  xAccl = data[0] * 256 + data[1];
  yAccl = data[2] * 256 + data[3];
  zAccl = data[4] * 256 + data[5];

  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Select data register 
  Wire.write(0x43);
  // Stop I2C transmission
  Wire.endTransmission();
  
  // Request 6 bytes of data
  Wire.requestFrom(Addr, 6);
  
  // Read 6 byte of data 
  if(Wire.available() == 6)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read(); 
  }
  // Convert the data
  xGyro = data[0] * 256 + data[1];
  yGyro = data[2] * 256 + data[3];
  zGyro = data[4] * 256 + data[5];

  // Output data to serial monitor
  //Serial.print("Acceleration in X-Axis : ");

 /*
  Serial.print(xAccl);
  Serial.print(" ");
  Serial.print(yAccl);
  Serial.print(" ");
  Serial.println(zAccl);
*/
  if(d1 > 200) d1 = 200;
  else if(d1 < 150) d1 = 150;
  md = d1 * (int)(((float)d3/d4 + (float)d2/d3 + (float)d1/d2)/3);
  if(md > 200) md = 200;
  else if(md < 150) md = 150;
  double a = sqrt(1.0*xAccl*xAccl+1.0*yAccl*yAccl+1.0*zAccl*zAccl);
  if(a < threshold)
  {
    ++dist;
    a = 0;
    pic = false;
  }
  if(a > 0 && pic)
  {
    ++dist;
  }
  else if(a > 0 && dist >= md - md/acCoef)
  {
    ++brojKoraka;
    d4 = d3;
    d3 = d2;
    d2 = d1;
    d1 = dist;
    dist = 0;
  }
  else if(a > 0 && dist >= md)
  {
    ++brojKoraka;
    d4 = d3;
    d3 = d2;
    d2 = d1;
    d1 = dist;
    dist = 0;
  }
  else if(a > 0 && dist < md && dist != 0)
  {
    pic = true;
    ++dist;
  }
  /*
  Serial.print("X-Axis of Rotation : ");
  Serial.println(xGyro);
  Serial.print("Y-Axis of Rotation : ");
  Serial.println(yGyro);
  Serial.print("Z-Axis of Rotation : ");
  Serial.println(zGyro);
  */
  delay(1);

  Serial.println(brojKoraka);
}
