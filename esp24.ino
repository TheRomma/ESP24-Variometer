#include "display.hpp"
#include "buzzer.hpp"

#include <cinttypes>

//Sleep interval for updates in ms. Larger intervals should count multiples of SLEEP_INTERVAL.
#define SLEEP_INTERVAL 5

//System declarations.
Display display;
Buzzer buzzer;

int32_t val;
int32_t idler;

void setup() {
  //Initial tests.
  //buzzer.toneTest();
  val = -199;
  idler = 0;
}

void loop() {
  //Updates every SLEEP_INTERVAL.
  idler = (idler+1)%10;
  if(idler == 0)val++;
  display.write(val);
  buzzer.play(val);
  delay(SLEEP_INTERVAL);
}
