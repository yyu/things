#include <ESP8266WiFi.h>

void initWiFi() {
    const char * ap = "Guest8266";
    const char * password_as_ap = "";
    const char * ssid = "Guest";
    const char * password = "";

    Serial.println();

    Serial.println(ap);
    Serial.println(password_as_ap);
    Serial.println(ssid);
    Serial.println(password);

    Serial.println("AP");
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ap, password_as_ap, /* channel */ 1, /* hide ssid */ 0, /* max_connection */ 4);

    Serial.println("STATION");
    WiFi.begin(ssid, password);

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.print("WiFi connected. ");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void setup() {
    delay(10000);
    Serial.begin(115200);
    Serial.println();
    Serial.print("Configuring access point...");

    initWiFi();
}

void loop() {
    delay(10000);
}
