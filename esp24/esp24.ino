#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include "buzzer.hpp"
#include "display.hpp"
#include "imu.hpp"
#include "NuPacket.hpp"
#include "NimBLEDevice.h"

#define SEALEVELPRESSURE_HPA (1013.25)
#define SLEEP_INTERVAL 5 //Sleep interval for updates in ms. Larger intervals should count multiples of SLEEP_INTERVAL.

#define WIRE Wire
#define I2C_SDA 48
#define I2C_SCL 0

//System declarations.
DFRobot_BMX160 imu(&WIRE);
Display display;
Buzzer buzzer;
Adafruit_BMP3XX bmp;

int32_t val;
int32_t idler;

void printBaroData(Adafruit_BMP3XX *bmp)
{
  Serial.print("Temperature = ");
  Serial.print(bmp->temperature);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bmp->pressure / 100.0);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bmp->readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0);  // Red
    delay(1000);
    neopixelWrite(RGB_BUILTIN, 0, 0, 0);  // Off / black
    delay(1000);
  }

  Serial.println("Setup");

  if (WIRE.begin(I2C_SDA, I2C_SCL, 100000) != true){
    Serial.println("i2c init false");
    while(1) {
      neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0);  // Red
      delay(1000);
      neopixelWrite(RGB_BUILTIN, 0, 0, 0);  // Off / black
      delay(1000);
    }
  }
  if (!bmp.begin_I2C(0x76, &WIRE)) {
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1) {
      neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0);  // Red
      delay(1000);
      neopixelWrite(RGB_BUILTIN, 0, 0, 0);  // Off / black
      delay(1000);
    }
  }

  if (imu.begin() != true){
    Serial.println("imu init false");
    while(1) {
      neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0);  // Red
      delay(1000);
      neopixelWrite(RGB_BUILTIN, 0, 0, 0);  // Off / black
      delay(1000);
    }
  }

  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  val = -199;
  idler = 0;
  
  NimBLEDevice::init("SENDING DATA IN NMEA PROTOCOL");
  NuPacket.start();
}

void loop() {
  if (NuPacket.connect()) 
  {
    NuPacket.send("Data from sensors in NMEA format");
  }
  else
  {
    Serial.println("--Waiting for connection--");
    while (!NuPacket.connect())
        delay(500);
    Serial.println("--Connected--");
  }
  // if (! bmp.performReading()) {
  //   Serial.println("Failed to perform reading :(");
  //   return;
  // }

  // // printBaroData(&bmp);
  // sBmx160SensorData_t Omagn, Ogyro, Oaccel;
  
  // imu.getAllData(&Omagn, &Ogyro, &Oaccel);
  // // imu.printAllData(&Omagn, &Ogyro, &Oaccel);

  // //Updates every SLEEP_INTERVAL.
  // idler = (idler+1)%10;
  // if(idler == 0)val++;
  // display.write(val);
  // buzzer.play(val);

  // delay(SLEEP_INTERVAL);
}
