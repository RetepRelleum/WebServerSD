#include "WebServer.h"

WebServer::WebServer(WiFiServer server, String webRoot)
{
    this->server = server;
    if (webRoot.endsWith("/") | webRoot.endsWith("\\"))
    {
        webRoot = webRoot.substring(webRoot.length() - 1);
    }
    this->webRoot = webRoot;
}

WebServer::~WebServer()
{
}

void WebServer::loop()
{
    webClient = new WebClient(server.available(), &v, webRoot);

    if (webClient->client)
    {
        webClient->begin();
        v.push_back(webClient);
    }
    else {
        delete webClient;
    }
    ptr = std::begin(v);
    for (int i = 0; i < v.size(); i++)
    {
        ptr[i]->loop();
    }
}

void WebServer::begin()
{
    server.begin();
}
