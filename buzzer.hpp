#pragma once

#include <cinttypes>

struct Buzzer{
  Buzzer();
  ~Buzzer();

  void toneTest();
  void play(int32_t value);

  uint16_t counter;
  uint16_t toggle;
};
