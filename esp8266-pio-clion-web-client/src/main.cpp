#include <ESP8266WiFi.h>

void initWiFi() {
  const char * ssid = "Guest";
  const char * password = "";

  Serial.println();
  Serial.println(ssid);
  Serial.println(password);

  Serial.println("STATION");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("WiFi connected. ");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup()
{
  delay(3000);
  Serial.begin(115200);

  initWiFi();
}

void loop()
{
  delay(1000);
}