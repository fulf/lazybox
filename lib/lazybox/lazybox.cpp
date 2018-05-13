#include "lazybox.h"

LazyBox::LazyBox(uint8_t board) {
  Serial.begin(115200);

  const char* id = "kitchen";
  const char* password = "17574331";

  _core = new LazyBoxCore(board);

  _web_server = new LazyBoxWebServer(_core);

  setWiFi(password);

  _core->setPinMode(_core->getPin("D4"), "OUTPUT");
  if (WiFi.status() == WL_CONNECTED) {
    delay(200);
    _core->setPinOutput(_core->getPin("D4"), 1);
    delay(200);
    _core->setPinOutput(_core->getPin("D4"), 0);
    delay(200);
    _core->setPinOutput(_core->getPin("D4"), 1);
    delay(200);
    _core->setPinOutput(_core->getPin("D4"), 0);
  }

  _poller = new LazyBoxPoller(id, "http://lazybox-api.herokuapp.com/intent", new LazyBoxActor(_core));
}

void LazyBox::run() {
  _web_server->handleClient();
  _poller->poll();
  delay(10000);
  yield();
}

void LazyBox::setWiFi(const char* password) {
  WiFi.disconnect();
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("lazybox", password);
  _core->connectWiFi("HackTM");
}
