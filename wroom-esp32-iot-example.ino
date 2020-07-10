#include "config.h"
#include "connection.h"
#include <ArduinoJson.h>

// You need to create an account on https://torabito.com and then navigate to Access Token section,
// Create a token, and paste it here. This is a sample token string, won't work for you :)
String token = String("59a9e7e2cd40666452a9501b31e91d01d5f08492");


long randomNumber;
long randomNumber2;

// String token = String("e713893e899e4686d550cdc3826cfad35341736a");
String topic = String("/device/" + token + "/set");

void setup()
{
  Serial.begin(115200);
  delay(10);
  randomSeed(analogRead(0));
  connect_to_wifi();
  setup_mqtt();
}

String get_change_message(long value, long value2) {

  DynamicJsonDocument doc(1024);

  String input = "{}";
  deserializeJson(doc, input);
  JsonObject message = doc.as<JsonObject>();

  message["token"] = token;
  message["imei"] = "esp-wroom-12";
  message["type"] = "change";
  message["data"]["temperature1"] = value;
  message["data"]["temperature2"] = value2;
  message["data"]["output1"] = "ON";
  message["data"]["output2"] = "OFF";

  String output;
  serializeJson(doc, output);

  Serial.println(output);

  return output;
}

long counting = 0;

void loop()
{
  connect_to_mqtt();
  mqtt_loop();
  randomNumber = random(15, 40);
  randomNumber2 = random(15, 40);   
  counting += 1;
  String message = get_change_message(randomNumber, counting);
  mqtt_publish_message(topic, message);

  delay(2000);
}
