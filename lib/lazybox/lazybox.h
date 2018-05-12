#ifndef LAZYBOX_H
#define LAZYBOX_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <lazybox-webserver.h>
#include <map>

class LazyBox
{
  private:
    LazyBoxCore* _core;
    LazyBoxWebServer* _web_server;
    std::map<const char*, LazyBoxPin> _pins;
    
  public:
    enum {
      WEMOSD1miniPro,
    };
    LazyBox(uint8_t);
    void run();
};

#endif
