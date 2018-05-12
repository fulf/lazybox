#include "lazybox-core.h"

LazyBoxCore::LazyBoxCore(uint8_t board) {
  switch (board) {
    case LazyBoxCore::WEMOSD1miniPro:
      _pinCount = WEMOSD1miniPro::PinCount;
      _pins = WEMOSD1miniPro::Pins;
      break;
    default:
      _pinCount = 0;
      _pins = {};
      break;
  }
}

LazyBoxPin* LazyBoxCore::getPins() {
  return _pins;
}
LazyBoxPin LazyBoxCore::getPin(uint8_t pin) {
  return _pins[pin];
}

uint8_t LazyBoxCore::getPinCount() {
  return _pinCount;
}

void LazyBoxCore::setPinMode(LazyBoxPin pin, const char* mode) {
  uint8_t pinIndex = LazyBoxCore::findIndexByPin(pin);

  if (!strcmp(mode, "INPUT")) {
    pinMode(pin.pin, INPUT);
    _pins[pinIndex].mode = "input";
  } else if (!strcmp(mode, "OUTPUT")) {
    pinMode(pin.pin, OUTPUT);
    _pins[pinIndex].mode = "output";
  } else if (!strcmp(mode, "PWM")) {
    pinMode(pin.pin, OUTPUT);
    _pins[pinIndex].mode = "pwm";
  }
}

void LazyBoxCore::setPinOutput(LazyBoxPin pin, uint16_t output) {
  uint8_t pinIndex = LazyBoxCore::findIndexByPin(pin);
  if (!strcmp(pin.mode, "output")) {
    uint16_t val = clamp(output, 0, 1);
    digitalWrite(pin.pin, val);
    _pins[pinIndex].value = val;
  } else if (!strcmp(pin.mode, "pwm")) {
    uint16_t val = clamp(output, 0, 1023);
    analogWrite(pin.pin, val);
    _pins[pinIndex].value = val;
  }
}

uint8_t LazyBoxCore::findIndexByPin(LazyBoxPin pin) {
  for (uint8_t i = 0; i < _pinCount; ++i) {
    if (_pins[i].pin == pin.pin) return i;
  }
}

uint16_t LazyBoxCore::clamp(uint16_t value, uint16_t min, uint16_t max) {
  if (value < min) return min;
  if (value > max) return max;
  return value;
}
