#pragma once

#include <cinttypes>

struct Display{
  Display();
  ~Display();

  void reset();
  void write(int32_t value);
  
  uint8_t counter;
  uint16_t digits[4];
  uint16_t bits[4];
};
