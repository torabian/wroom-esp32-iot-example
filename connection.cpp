#include "config.h"
#include <WiFi.h>
#include <Update.h>
#include <PubSubClient.h>

WiFiClient client;
PubSubClient mqtt_client(client);

// Utility to extract header value from headers
String getHeaderValue(String header, String headerName)
{
  return header.substring(strlen(headerName.c_str()));
}

void connect_to_mqtt()
{
  // Loop until we're reconnected
  while (!mqtt_client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (mqtt_client.connect("rovx-ecu-esp32"))
    {
      Serial.println("connected");
      mqtt_client.subscribe("/devices/set/wroom1");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 0.5 seconds");
      delay(500);
    }
  }
}

void connect_to_wifi()
{

  Serial.print("Connecting to ");
  Serial.println(CONF_WIFI_SSID);

  WiFi.begin(CONF_WIFI_SSID, CONF_WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

String payload_to_string(byte *payload, unsigned int length)
{
  String payload_buff;
  for (int i = 0; i < length; i++)
  {
    payload_buff = payload_buff + String((char)payload[i]);
  }
  return payload_buff;
}

void on_message_receive_mqtt(char *topic, byte *message, unsigned int length)
{
  Serial.println("MQTT Got a message: ");
  Serial.print("Topic: \"");
  Serial.print(topic);
  Serial.print(payload_to_string(message, length));
  Serial.println("\"");
}

void setup_mqtt()
{
  mqtt_client.setServer(CONF_MQTT_HOSTNAME, CONF_MQTT_PORT);
  mqtt_client.setCallback(on_message_receive_mqtt);
}

void mqtt_publish_message(String topic, String message)
{
  mqtt_client.publish(topic.c_str(), message.c_str());
}

void mqtt_loop()
{
  mqtt_client.loop();
}
 