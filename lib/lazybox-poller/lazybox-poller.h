#ifndef LAZYBOX_POLLER_H
#define LAZYBOX_POLLER_H

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <lazybox-actor.h>

class LazyBoxPoller {
  private:
    HTTPClient _http_client;
    LazyBoxActor* _actor;
    const char* _call_sign;
  public:
    LazyBoxPoller(const char*, const char*, LazyBoxActor*);
    void poll();
};

#endif
