#include "lazybox-webserver.h"

LazyBoxWebServer::LazyBoxWebServer(LazyBoxCore* lbc) {
  _core = lbc;

  _web_server.on("/", HTTP_GET, std::bind(&LazyBoxWebServer::handleRoot, this));
  _web_server.onNotFound(std::bind(&LazyBoxWebServer::handleNotFound, this));

  setPinHandlers();
  setWiFiHandlers();
  _web_server.begin();
}

void LazyBoxWebServer::addSensor(String name, int *val) {
  _web_server.on(("/sensor/" + name).c_str(), HTTP_GET, [=](){
    sendJSON(String(*val));
  });
}

void LazyBoxWebServer::addSensor(String name, float *val) {
  _web_server.on(("/sensor/" + name).c_str(), HTTP_GET, [=](){
    sendJSON(String(*val));
  });
}

void LazyBoxWebServer::addSensor(String name, String *val) {
  _web_server.on(("/sensor/" + name).c_str(), HTTP_GET, [=](){
    sendJSON(String(*val));
  });
}

void LazyBoxWebServer::addSensor(String name, int (*cb)()) {
  _web_server.on(("/sensor/" + name).c_str(), HTTP_GET, [=](){
    sendJSON(String(cb()));
  });
}

void LazyBoxWebServer::addSensor(String name, float (*cb)()) {
  _web_server.on(("/sensor/" + name).c_str(), HTTP_GET, [=](){
    sendJSON(String(cb()));
  });
}

void LazyBoxWebServer::addSensor(String name, String (*cb)()) {
  _web_server.on(("/sensor/" + name).c_str(), HTTP_GET, [=](){
    sendJSON(String(cb()));
  });
}

void LazyBoxWebServer::setPinHandlers() {
  LazyBoxPin* pins = _core->getPins();
  uint8_t totalPins = _core->getPinCount();
  for(uint8_t i = 0; i < totalPins; ++i) {
    _web_server.on(("/pin/" + pins[i].name).c_str(), HTTP_GET, std::bind(&LazyBoxWebServer::handlePinGet, this, i));
    _web_server.on(("/pin/" + pins[i].name).c_str(), HTTP_POST, std::bind(&LazyBoxWebServer::handlePinPost, this, i));
    _web_server.on(("/pin/" + pins[i].name).c_str(), HTTP_PUT, std::bind(&LazyBoxWebServer::handlePinPut, this, i));
    _web_server.on(("/pin/" + pins[i].name).c_str(), HTTP_DELETE, std::bind(&LazyBoxWebServer::handlePinDelete, this, i));
    _web_server.on(("/pin/" + pins[i].name).c_str(), HTTP_OPTIONS, std::bind(&LazyBoxWebServer::handlePinOptions, this, i));
  }
}

void LazyBoxWebServer::setWiFiHandlers() {
  _web_server.on("/wifi", HTTP_GET, std::bind(&LazyBoxWebServer::handleWiFiGet, this));
  _web_server.on("/wifi", HTTP_POST, std::bind(&LazyBoxWebServer::handleWiFiPost, this));
  _web_server.on("/wifi", HTTP_DELETE, std::bind(&LazyBoxWebServer::handleWiFiDelete, this));
}

void LazyBoxWebServer::handleWiFiGet() {
  String response = "[";
  int n = WiFi.scanNetworks();
  for(int i=0; i<n; ++i) {
    String temp = "{\
      \"id\":{i},\
      \"attributes\":{\
        \"ssid\":\"{s}\",\
        \"rssi\":{r},\
        \"encryption\":\"{e}\"\
      }\
    }";

    temp.replace("{i}", String(i+1));
    temp.replace("{s}", WiFi.SSID(i));
    temp.replace("{r}", String(WiFi.RSSI(i)));
    temp.replace("{e}", _core->getWiFiEncryptionType(i));

    response += temp + (i<n-1 ? "," : "");
  }
  response += "]";

  sendJSON(response, 200, true);
}

void LazyBoxWebServer::handleWiFiPost() {
  const uint8_t MAX_TRIES = 30;
  uint8_t status;
  String ssid = _web_server.arg("ssid");
  String pass = _web_server.arg("pass");

  uint16_t respCode = 200;
  String respMessage;

  if(ssid.length() > 31 || ssid.length() == 0) {
    respCode = 400;
    respMessage = "Invalid SSID provided.";
  } else if((pass.length() > 0 && pass.length() < 8) || pass.length() > 63) {
    respCode = 400;
    respMessage = "Invalid password provided.";
  }

  if(respCode == 200) {
    _core->connectWiFi(ssid.c_str(), pass.c_str());

    if(status != WL_CONNECTED) {
      respCode = 403;
      respMessage = "Invalid credentials provided.";
    } else {
      respCode = 200;
      respMessage = "Connected succesfully to provided WiFi.";
    }
  }

  sendJSON(respMessage, respCode);
}

void LazyBoxWebServer::handleWiFiDelete() {
  WiFi.disconnect();
  sendJSON("Disconnected succesfully!");
}

void LazyBoxWebServer::sendJSON(String message, uint16_t code, bool raw) {
  String success = (code/100) == 2 ? "true" : "false";

  if (!raw) {
    message = "\"" + message + "\"";
  }

  send(code, "application/json", "{\"success\":" + success + ",\"message\":" + message + "}");
}

void LazyBoxWebServer::handlePinGet(uint8_t pin) {
  String response = "{\
    \"name\": \"{nm}\",\
    \"pin\": {pn},\
    \"digital\": {dg},\
    \"mode\": \"{md}\",\
    \"value\": {vl}\
  }";

  LazyBoxPin p = _core->getPin(pin);

  response.replace("{nm}", p.name);
  response.replace("{pn}", String(p.pin));
  response.replace("{dg}", p.digital ? "true" : "false");
  response.replace("{md}", p.mode);
  response.replace("{vl}", String(p.value));

  sendJSON(response, 200, true);
}

void LazyBoxWebServer::handlePinPost(uint8_t pin) {
  LazyBoxPin p = _core->getPin(pin);
  uint16_t output = atoi(_web_server.arg("output").c_str());

  _core->setPinOutput(p, output);
  sendJSON("POST pin " + String(p.pin));
}

void LazyBoxWebServer::handlePinPut(uint8_t pin) {
  LazyBoxPin p = _core->getPin(pin);

  _core->setPinMode(p, _web_server.arg("mode").c_str());
  sendJSON("PUT pin" + String(p.pin));
}

void LazyBoxWebServer::handlePinDelete(uint8_t pin) {
  LazyBoxPin p = _core->getPin(pin);

  _core->setPinOutput(p, 0);
  sendJSON("DELETE pin " + String(p.pin));
}

void LazyBoxWebServer::handlePinOptions(uint8_t pin) {
  send(200);
}

void LazyBoxWebServer::handleClient() {
  _web_server.handleClient();
}

void LazyBoxWebServer::handleNotFound() {
  sendJSON("The requested page could not be found", 404);
}

void LazyBoxWebServer::handleRoot() {
  sendJSON("Welcome to the root page", 200);
}

void LazyBoxWebServer::send(uint8_t code, const char *content_type, const String &content) {
  _web_server.sendHeader("Access-Control-Max-Age", "10000");
  _web_server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
  _web_server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  _web_server.send(code, content_type, content);
}
