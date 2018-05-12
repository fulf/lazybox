#ifndef LAZYBOX_CORE_H
#define LAZYBOX_CORE_H

#include <Arduino.h>
#include <lazybox-boards.h>
#include <map>

class LazyBoxCore
{
  private:
    uint8_t _pinCount;
    LazyBoxPin* _pins;
    uint8_t findIndexByPin(LazyBoxPin);
    uint16_t clamp(uint16_t, uint16_t, uint16_t);
  public:
    enum {
      WEMOSD1miniPro,
    };

    LazyBoxCore(uint8_t);
    LazyBoxPin* getPins();
    LazyBoxPin getPin(uint8_t);
    LazyBoxPin getPin(String);
    uint8_t getPinCount();
    void setPinMode(LazyBoxPin, const char*);
    void setPinOutput(LazyBoxPin, uint16_t);
};

#endif
