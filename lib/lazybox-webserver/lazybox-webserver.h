#ifndef LAZYBOX_WEBSERVER_H
#define LAZYBOX_WEBSERVER_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <lazybox-core.h>
#include <map>

class LazyBoxWebServer
{
  private:
    ESP8266WebServer _web_server;
    LazyBoxCore *_core;
    void handleRoot(),
      handleNotFound(),

      // route /pin/{int}
      handlePinGet(uint8_t),
      handlePinPost(uint8_t),
      handlePinPut(uint8_t),
      handlePinDelete(uint8_t),
      handlePinOptions(uint8_t),

      send(uint8_t code, const char *content_type=NULL, const String &content=String("")),
      setPreflightHeaders();


  public:
    LazyBoxWebServer(LazyBoxCore*);

    void setPinHandlers(LazyBoxPin*, uint8_t),
      sendJSON(String, uint16_t = 200, bool = false),
      handleClient();
};

#endif
