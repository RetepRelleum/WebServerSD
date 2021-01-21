#include "WebClient.h"
#include <array>

WebClient::WebClient(WiFiClient client, std::vector<WebClient *> *v, String webRoot)
{
    this->client = client;
    this->v = v;
    this->webRoot = webRoot;
}

void WebClient::begin()
{
    if (client.connected())
    { // loop while the client's connected
        delay(100);
        if (client.available())
        {
            String s = client.readStringUntil('\n');
            s.replace("\r", "");
            s.replace("\n", "");
            Serial.println(s);
            if (s.startsWith("GET") & s.endsWith("HTTP/1.1"))
            {
                s.replace("GET", "");
                s.replace("HTTP/1.1", "");
                request = s;
                request.trim();
                request.replace("\\", "/");
                if (!(request.startsWith("/")))
                {
                    request = String("/") + request;
                }
                while (s.length() != 0)
                {
                    s = client.readStringUntil('\n');

                    s.replace("\r", "");
                    s.replace("\n", "");
                    Serial.println(s);
                }
                file = SD.open(webRoot + request);
                client.println("HTTP/1.1 200 OK");
                client.print("Content-type: ");

                client.println(getMimeTypes(request));
                client.println("Content-Language: de");
                if (file)
                {
                    client.println("Content-Length: " + String(file.size()));
                    client.print("Content-type: ");
                    client.println(getMimeTypes(request));
                }
                else
                {
                    client.println("Content-type: text/html");
                }
                client.println("Connection: close");
                client.println();
                return;
            }
        }
    }
    stop();
}
void WebClient::loop()
{
    File root;
    if (request.equalsIgnoreCase("/dir"))
    {
        root = SD.open(webRoot);
        printDirectory(root, 0);
        stop();
    }
    else if (file)
    {
        int size = file.readBytes(buffer, 256);
        client.write(buffer, size);
        if (file.available() == 0)
        {
            stop();
        }
    }
    else
    {
        client.println("Page not found");
        stop();
    }
}
void WebClient::stop()
{
    client.stop();
    for (ptr = std::begin(*v); ptr != std::end(*v); ++ptr)
    {
        if ((*ptr)->client == client)
        {
            v->erase(ptr);
            delete this;
            break;
        }
    }
}
WebClient::~WebClient()
{
}

String WebClient::getMimeTypes(String request)
{
    String e = request.substring(request.lastIndexOf(".") + 1);
    String ret = "text/html";
    for (int i = 0; i < mineTypesSize; i++)
    {
        if (mimeTypes[i][0].equalsIgnoreCase(e))
        {
            return mimeTypes[i][1];
        }
    }
    return "text/html";
}
void WebClient::printDirectory(File dir, int numTabs)
{
    while (true)
    {
        File entry = dir.openNextFile();
        if (!entry)
        {
            break;
        }
        String a = String(entry.name());
        a.replace(webRoot, "");
        if (entry.isDirectory())
        {
            client.println(a);
            client.println("/");
            client.print("</p>");
            printDirectory(entry, numTabs + 1);
        }
        else
        {
            client.print("<p><a href=""");
            client.print(a);
            client.print(""">" +a + "</a>");
            // files have sizes, directories do not
            client.print("  ");
            client.println(entry.size(), DEC);
            client.print("byte </p>");
        }
        entry.close();
    }
}