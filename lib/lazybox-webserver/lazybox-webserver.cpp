#include "lazybox-webserver.h"

LazyBoxWebServer::LazyBoxWebServer(LazyBoxCore* lbc) {
  _core = lbc;

  _web_server.on("/", HTTP_GET, std::bind(&LazyBoxWebServer::handleRoot, this));
  _web_server.onNotFound(std::bind(&LazyBoxWebServer::handleNotFound, this));

  _web_server.begin();
}

const char* LazyBoxWebServer::getArgument(const char * name) {
  return _web_server.arg(name).c_str();
}

void LazyBoxWebServer::sendJSON(String message, uint16_t code, bool raw) {
  String success = (code/100) == 2 ? "true" : "false";

  if (!raw) {
    message = "\"" + message + "\"";
  }

  send(code, "application/json", "{\"success\":" + success + ",\"message\":" + message + "}");
}

void LazyBoxWebServer::addGet(const char * route, LazyBoxWebServer::THandlerFunction cb) {
  _web_server.on(route, HTTP_GET, cb);
}

void LazyBoxWebServer::addPost(const char * route, LazyBoxWebServer::THandlerFunction cb) {
  _web_server.on(route, HTTP_POST, cb);
}

void LazyBoxWebServer::addPut(const char * route, LazyBoxWebServer::THandlerFunction cb) {
  _web_server.on(route, HTTP_PUT, cb);
}

void LazyBoxWebServer::addDelete(const char * route, LazyBoxWebServer::THandlerFunction cb) {
  _web_server.on(route, HTTP_DELETE, cb);
}

void LazyBoxWebServer::addOptions(const char * route, LazyBoxWebServer::THandlerFunction cb) {
  _web_server.on(route, HTTP_OPTIONS, cb);
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
