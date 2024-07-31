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

TaskHandle_t measBaro_handle = NULL;
TaskHandle_t showResult_handle = NULL;

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

void measBaro(void *parameters) {
  for (;;) {
    if (! bmp.performReading()) {
          Serial.println("Failed to perform reading :(");
          return;
    }
    cur_height = bmp.readAltitude(SEALEVELPRESSURE_HPA);
    climb_rate = cur_height - prev_height;
    prev_height = cur_height;
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
void showResult(void *parameters) {
  for (;;) {
    int climb_rate_dm = static_cast<int>(climb_rate*10);
    for (size_t i = 0; i < 4; i++)
    {
      display.write(climb_rate_dm);
      buzzer.play(climb_rate_dm);
      delay(1);
    }
    vTaskDelay(5 / portTICK_PERIOD_MS);
  }
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
  bmp.setOutputDataRate(BMP3_ODR_3_1_HZ);
  if (! bmp.performReading()) {
      Serial.println("Failed to perform reading :(");
      return;
  }
  prev_height = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  cur_height = prev_height;
  climb_rate = 0;

  NimBLEDevice::init("Variometer");
  NuPacket.start();

  xTaskCreate(
                  measBaro,          /* Task function. */
                  "measBaro",        /* String with name of task. */
                  10000,            /* Stack size in bytes. */
                  NULL,             /* Parameter passed as input of the task */
                  1,                /* Priority of the task. */
                  &measBaro_handle);            /* Task handle. */
  xTaskCreate(
                  showResult,          /* Task function. */
                  "showResult",        /* String with name of task. */
                  10000,            /* Stack size in bytes. */
                  NULL,             /* Parameter passed as input of the task */
                  1,                /* Priority of the task. */
                  &showResult_handle);            /* Task handle. */
                  
  Serial.println("Setup Done");             
}

void loop() {

  if (NuPacket.connect(2000) | NuPacket.isConnected()) 
  {
    if (showResult_handle != NULL) {
      vTaskSuspend(showResult_handle);
      display.reset();
      buzzer.stop();
    }

    Serial.println("BT is Connected, sending data via BLE");
    std::string nmea_message = setNmeaShortLXWP0(cur_height, climb_rate);
    NuPacket.send(nmea_message.c_str());
  }
  else
  {
    Serial.println("BT is not Connected, normal OP");
    if (showResult_handle != NULL) vTaskResume(showResult_handle);
  }

  // if (! bmp.performReading()) {
  //   Serial.println("Failed to perform reading :(");
  //   return;
  // }
  // // printBaroData(&bmp);
  // sBmx160SensorData_t Omagn, Ogyro, Oaccel;
  // imu.getAllData(&Omagn, &Ogyro, &Oaccel);
  // // imu.printAllData(&Omagn, &Ogyro, &Oaccel);

}
