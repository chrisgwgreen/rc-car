#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <html.h>
#include <motor.h>

// #include <iostream>
// #include <fstream>

const char *ssid = "RC-CAR";
const char *password = "123456789";

HTML html;
MOTOR motor;

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

    char *coords[2];
    char *part;
    int partIndex = 0;

    part = strtok((char *)data, ",");

    while (part && partIndex < 2)
    {
      coords[partIndex] = part;
      part = strtok(NULL, ",");
      partIndex++;
    };

    int x = atoi(coords[0]);
    int y = atoi(coords[1]);

    motor.setMotor(x, y);
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    ledState = 1;
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    ledState = 0;
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

  // SETUP MOTOR
  motor.setup();

  // ACCESS POINT
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

  // SERVER
  initWebSocket();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", html.getHTML()); });

  server.begin();

  // MISC
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop()
{
  ws.cleanupClients();
  digitalWrite(ledPin, ledState);
}