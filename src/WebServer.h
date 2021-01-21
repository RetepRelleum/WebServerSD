#ifndef WEBSERVER_H
#define WEBSERVER_H
#include <Arduino.h>
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <vector>
#include "WebClient.h"

class WebServer
{
private:
    WiFiServer server;
    std::vector<WebClient *> v;
    std::vector<WebClient *>::iterator ptr;
    String webRoot;
    WebClient *webClient;

public:
    WebServer(WiFiServer server,String webRoot);
    ~WebServer();
    void begin();
    void loop();
};

#endif
