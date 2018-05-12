#ifndef LAZYBOX_BOARDS_H
#define LAZYBOX_BOARDS_H

#include <Arduino.h>
#include <map>

struct LazyBoxPin{
  String name;
  uint8_t pin;
  bool digital;
  const char* mode;
  int value;
};

namespace WEMOSD1miniPro {
  extern int PinCount;
  extern LazyBoxPin Pins[];
}

#endif
