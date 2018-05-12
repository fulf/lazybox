#include "lazybox.h"

LazyBox::LazyBox(uint8_t board) {
  const char* hash = "17574331";
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP("lazybox", hash);

  _core = new LazyBoxCore(board);
  _web_server = new LazyBoxWebServer(_core);
  _web_server->setPinHandlers(_core->getPins(), _core->getPinCount());
}

void LazyBox::run() {
  _web_server->handleClient();
  yield();
}
