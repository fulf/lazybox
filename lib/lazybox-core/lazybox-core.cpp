#include "lazybox-core.h"

LazyBoxCore::LazyBoxCore() {
  char* hash = "17574331";
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP("lazybox", hash);

  _web_server = new LazyBoxWebServer();
}

void LazyBoxCore::run() {
  _web_server->handleClient();
  yield();
}
