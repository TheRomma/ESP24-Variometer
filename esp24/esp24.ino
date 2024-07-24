#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include "buzzer.hpp"
#include "display.hpp"
#include "imu.hpp"
#include "NuPacket.hpp"
#include "NimBLEDevice.h"
#include "nmea.hpp"

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

float prev_height = 0;
float cur_height = 0;
float climb_rate = 0;
int gum_fix_counter = 0;

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


  if (imu.begin() != true){
    Serial.println("imu init false");
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
  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_200_HZ);
  if (! bmp.performReading()) {
      Serial.println("Failed to perform reading :(");
      return;
  }
  prev_height = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  cur_height = prev_height;
  climb_rate = 0;

  NimBLEDevice::init("Variometer");
  NuPacket.start();

}

void loop() {
  if (gum_fix_counter % 100 == 0) {
    if (! bmp.performReading()) {
        Serial.println("Failed to perform reading :(");
        return;
    }
    cur_height = bmp.readAltitude(SEALEVELPRESSURE_HPA);
    climb_rate = cur_height - prev_height;
    prev_height = cur_height;
    gum_fix_counter = 1;
  }

  if (NuPacket.connect(5)) 
  {
    Serial.println("BT Connected");
    std::string nmea_message = setNmeaShortLXWP0(cur_height, climb_rate);
    NuPacket.send(nmea_message.c_str());
  }
  else
  {
    int climb_rate_dm = static_cast<int>(climb_rate*10);
    for (size_t i = 0; i < 4; i++)
    {
      display.write(climb_rate_dm);
      buzzer.play(climb_rate_dm);
      delay(SLEEP_INTERVAL)
    }
  }

  // if (! bmp.performReading()) {
  //   Serial.println("Failed to perform reading :(");
  //   return;
  // }

  // // printBaroData(&bmp);
  // sBmx160SensorData_t Omagn, Ogyro, Oaccel;
  
  // imu.getAllData(&Omagn, &Ogyro, &Oaccel);
  // // imu.printAllData(&Omagn, &Ogyro, &Oaccel);

  gum_fix_counter++;
  delay(SLEEP_INTERVAL);
}
