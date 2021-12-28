#define LED_BUILTIN 2

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <html.h>
#include <motor.h>

const char *ssid = "CAR-1";

HTML html;
MOTOR motor;

bool isConnected = 0;
const int ledPin = 2;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

/**
 * @brief handleWebSocketMessage processes the message recieved from the controller (in the form or '-100,100').
 * 
 * @param arg 
 * @param data 
 * @param len 
 */
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;

    char *coords[2];
    char *parts;
    int partIndex = 0;

    parts = strtok((char *)data, ",");

    while (parts && partIndex < 2)
    {
      coords[partIndex] = parts;
      parts = strtok(NULL, ",");
      partIndex++;
    };

    int x = atoi(coords[0]);
    int y = atoi(coords[1]);

    motor.setMotor(x, y);
  }
}

/**
 * @brief handleEvent is triggered when a websocket message is sent from the html (controller).
 * 
 * @param server 
 * @param client 
 * @param type 
 * @param arg 
 * @param data 
 * @param len 
 */
void handleEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                 void *arg, uint8_t *data, size_t len)
{

  switch (type)
  {
  case WS_EVT_CONNECT:

    Serial.print("IS CONNECTED -> ");
    Serial.println(isConnected);

    if (isConnected)
      client->close();

    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());

    client->text((char *)ssid);
    isConnected = 1;
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    ws.textAll("disconnected");
    isConnected = 0;
    break;
  case WS_EVT_DATA:
    handleWebSocketMessage(arg, data, len);
    isConnected = 1;
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}

/**
 * @brief 
 * setup initialises the esp32's gpio pins and core functionality (e.g. setting up the wireless access point)
 */
void setup()
{
  Serial.begin(115200);

  // ACCESS POINT
  WiFi.softAP(ssid);

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
  ws.onEvent(handleEvent);
  server.addHandler(&ws);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", html.getHTML()); });

  server.begin();

  // MOTOR
  // ----------
  motor.setup();

  // LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

/**
 * @brief loop repeats continously whilst the esp32 is powered. In this loop, the led state is maintained (based on whether there are any connections) and the websockets are managed here. 
 */
void loop()
{
  ws.cleanupClients();
  digitalWrite(LED_BUILTIN, isConnected);
}