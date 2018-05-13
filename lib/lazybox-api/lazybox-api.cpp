#include "lazybox-api.h"

LazyBoxApi::LazyBoxApi(LazyBoxCore* core, LazyBoxWebServer* web_server) {
  _web_server = web_server;
  _core = core;

  setPinHandlers();
  setWiFiHandlers();
}

void LazyBoxApi::setPinHandlers() {
  LazyBoxPin* pins = _core->getPins();
  uint8_t totalPins = _core->getPinCount();
  for(uint8_t i = 0; i < totalPins; ++i) {
    _web_server->addGet(("/pin/" + pins[i].name).c_str(), std::bind(&LazyBoxApi::handlePinGet, this, i));
    _web_server->addPost(("/pin/" + pins[i].name).c_str(), std::bind(&LazyBoxApi::handlePinPost, this, i));
    _web_server->addPut(("/pin/" + pins[i].name).c_str(), std::bind(&LazyBoxApi::handlePinPut, this, i));
    _web_server->addDelete(("/pin/" + pins[i].name).c_str(), std::bind(&LazyBoxApi::handlePinDelete, this, i));
    _web_server->addOptions(("/pin/" + pins[i].name).c_str(), std::bind(&LazyBoxApi::handlePinOptions, this, i));
  }
}

void LazyBoxApi::setWiFiHandlers() {
  _web_server->addGet("/wifi", std::bind(&LazyBoxApi::handleWiFiGet, this));
  _web_server->addPost("/wifi", std::bind(&LazyBoxApi::handleWiFiPost, this));
  _web_server->addDelete("/wifi", std::bind(&LazyBoxApi::handleWiFiDelete, this));
}

void LazyBoxApi::handleWiFiGet() {
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

  _web_server->sendJSON(response, 200, true);
}

void LazyBoxApi::handleWiFiPost() {
  uint8_t status;
  String ssid = _web_server->getArgument("ssid");
  String pass = _web_server->getArgument("pass");

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

  _web_server->sendJSON(respMessage, respCode);
}

void LazyBoxApi::handleWiFiDelete() {
  WiFi.disconnect();
  _web_server->sendJSON("Disconnected succesfully!");
}

void LazyBoxApi::handlePinGet(uint8_t pin) {
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

  _web_server->sendJSON(response, 200, true);
}

void LazyBoxApi::handlePinPost(uint8_t pin) {
  LazyBoxPin p = _core->getPin(pin);
  uint16_t output = atoi(_web_server->getArgument("output"));

  _core->setPinOutput(p, output);
  _web_server->sendJSON("POST pin " + String(p.pin));
}

void LazyBoxApi::handlePinPut(uint8_t pin) {
  LazyBoxPin p = _core->getPin(pin);

  _core->setPinMode(p, _web_server->getArgument("mode"));
  _web_server->sendJSON("PUT pin" + String(p.pin));
}

void LazyBoxApi::handlePinDelete(uint8_t pin) {
  LazyBoxPin p = _core->getPin(pin);

  _core->setPinOutput(p, 0);
  _web_server->sendJSON("DELETE pin " + String(p.pin));
}

void LazyBoxApi::handlePinOptions(uint8_t pin) {
  _web_server->send(200);
}
