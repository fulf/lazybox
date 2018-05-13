#ifndef LAZYBOX_API_H
#define LAZYBOX_API_H

#include <lazybox-webserver.h>
#include <lazybox-core.h>

class LazyBoxApi {
  private:
    LazyBoxWebServer *_web_server;
    LazyBoxCore *_core;
    void setPinHandlers(),
    handlePinGet(uint8_t),
    handlePinPost(uint8_t),
    handlePinPut(uint8_t),
    handlePinDelete(uint8_t),
    handlePinOptions(uint8_t),

    setWiFiHandlers(),
    handleWiFiGet(),
    handleWiFiPost(),
    handleWiFiDelete();
  public:
    LazyBoxApi(LazyBoxCore*, LazyBoxWebServer*);
};

#endif
