#include "lazybox-webserver.h"

LazyBoxWebServer::LazyBoxWebServer(LazyBoxCore* lbc) {
  _core = lbc;

  _web_server.on("/", HTTP_GET, std::bind(&LazyBoxWebServer::handleRoot, this));
  _web_server.onNotFound(std::bind(&LazyBoxWebServer::handleNotFound, this));

  _web_server.begin();
}

void LazyBoxWebServer::setPinHandlers(LazyBoxPin* pins, uint8_t totalPins) {
  for(uint8_t i = 0; i < totalPins; ++i) {
    _web_server.on(("/pin/" + pins[i].name).c_str(), HTTP_GET, std::bind(&LazyBoxWebServer::handlePinGet, this, i));
    _web_server.on(("/pin/" + pins[i].name).c_str(), HTTP_POST, std::bind(&LazyBoxWebServer::handlePinPost, this, i));
    _web_server.on(("/pin/" + pins[i].name).c_str(), HTTP_PUT, std::bind(&LazyBoxWebServer::handlePinPut, this, i));
    _web_server.on(("/pin/" + pins[i].name).c_str(), HTTP_DELETE, std::bind(&LazyBoxWebServer::handlePinDelete, this, i));
    _web_server.on(("/pin/" + pins[i].name).c_str(), HTTP_OPTIONS, std::bind(&LazyBoxWebServer::handlePinOptions, this, i));
  }
}

void LazyBoxWebServer::sendJSON(String message, uint16_t code, bool raw) {
  String success = (code/100) == 2 ? "true" : "false";
  LazyBoxWebServer::send(code, "application/json", raw ? message : "{\"success\":" + success+ ",\"message\":\"" + message+ "\"}");
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

  LazyBoxWebServer::sendJSON(response, 200, true);
}

void LazyBoxWebServer::handlePinPost(uint8_t pin) {
  LazyBoxPin p = _core->getPin(pin);
  uint16_t output = atoi(_web_server.arg("output").c_str());

  _core->setPinOutput(p, output);
  LazyBoxWebServer::sendJSON("POST pin " + String(p.pin));
}

void LazyBoxWebServer::handlePinPut(uint8_t pin) {
  LazyBoxPin p = _core->getPin(pin);

  _core->setPinMode(p, _web_server.arg("mode").c_str());
  LazyBoxWebServer::sendJSON("PUT pin" + String(p.pin));
}

void LazyBoxWebServer::handlePinDelete(uint8_t pin) {
  LazyBoxPin p = _core->getPin(pin);

  _core->setPinOutput(p, 0);
  LazyBoxWebServer::sendJSON("DELETE pin " + String(p.pin));
}

void LazyBoxWebServer::handlePinOptions(uint8_t pin) {
  LazyBoxWebServer::send(200);
}

void LazyBoxWebServer::handleClient() {
  _web_server.handleClient();
}

void LazyBoxWebServer::handleNotFound() {
  LazyBoxWebServer::sendJSON("The requested page could not be found", 404);
}

void LazyBoxWebServer::handleRoot() {
  LazyBoxWebServer::sendJSON("Welcome to the root page", 200);
}

void LazyBoxWebServer::send(uint8_t code, const char *content_type, const String &content) {
  _web_server.sendHeader("Access-Control-Max-Age", "10000");
  _web_server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
  _web_server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  _web_server.send(code, content_type, content);
}
