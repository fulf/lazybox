#include "lazybox.h"

LazyBox::LazyBox(uint8_t board) {
  Serial.begin(115200);

  const char* id = "kitchen";
  const char* password = "17574331";

  _core = new LazyBoxCore(board);

  _web_server = new LazyBoxWebServer(_core);
  _web_server->setPinHandlers(_core->getPins(), _core->getPinCount());

  setWiFi(password);

  _core->setPinMode(_core->getPin("D4"), "OUTPUT");

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
  WiFi.begin("HackTM");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println(WiFi.localIP().toString());
}
