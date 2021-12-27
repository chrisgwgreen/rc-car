#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <html.h>

#include <iostream>
#include <fstream>

const char *ssid = "RC-CAR";
const char *password = "123456789";

HTML html;

bool ledState = 0;
const int ledPin = 2;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;

    Serial.print("MESSAGE - ");
    Serial.println((char *)data);

    if (strcmp((char *)data, "toggle") == 0)
    {
      ledState = !ledState;
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    break;
  case WS_EVT_DATA:
    handleWebSocketMessage(arg, data, len);
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}

void initWebSocket()
{
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void setup()
{
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.softAP(ssid, password);

  if (!MDNS.begin("rc"))
  {
    Serial.println("Error starting mDNS");
    return;
  }

  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP - ");
  Serial.println(IP);
  Serial.println("mDNS - http://rc.local");

  initWebSocket();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", html.getHTML()); });

  server.begin();
}

void loop()
{
  ws.cleanupClients();
  digitalWrite(ledPin, ledState);
}