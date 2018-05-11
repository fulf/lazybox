#include "lazybox-webserver.h"

LazyBoxWebServer::LazyBoxWebServer() {
  _web_server.on("/", HTTP_GET, std::bind(&LazyBoxWebServer::handleRoot, this));
  _web_server.onNotFound(std::bind(&LazyBoxWebServer::handleNotFound, this));

  _web_server.begin();
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

void LazyBoxWebServer::sendJSON(String message, int code, bool raw) {
  String success = (code/100) == 2 ? "true" : "false";
  _web_server.send(code, "application/json", raw ? message : "{\"success\":" + success+ ",\"message\":\"" + message+ "\"}");
}
