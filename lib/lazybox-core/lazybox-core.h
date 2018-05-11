#ifndef LAZYBOX_CORE_H
#define LAZYBOX_CORE_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <lazybox-webserver.h>

#define VERSION "0.0.1"

class LazyBoxCore
{
  private:
    LazyBoxWebServer *_web_server;

    void sendJSON(String, int = 200, bool = false);
  public:
    LazyBoxCore();
    void run();
};

#endif
