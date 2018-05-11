#ifndef LAZYBOX_WEBSERVER_H
#define LAZYBOX_WEBSERVER_H

#include <ESP8266WebServer.h>

#define VERSION "0.0.1"

class LazyBoxWebServer
{
  private:
    ESP8266WebServer _web_server;
    void handleRoot(),
      handleNotFound();

  public:
    LazyBoxWebServer();

    void sendJSON(String, int = 200, bool = false),
      handleClient();
};

#endif
