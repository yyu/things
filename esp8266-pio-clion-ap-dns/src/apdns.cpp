#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

const char *ssid = "ESPap";
const char *password = "thereisnospoon";

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

///* Just a little test message.  Go to http://192.168.4.1 in a web browser
//   connected to this access point to see it.
//*/
//void handleRoot() {
//    webServer.send(200, "text/html", "<h1>You are connected</h1>");
//}


void setup() {
    delay(10000);
    Serial.begin(115200);
    Serial.println();
    Serial.print("Configuring access point...");

//    WiFi.mode(WIFI_AP);
//    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(ssid, password);

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
//    webServer.on("/", handleRoot);
//    webServer.begin();
//    Serial.println("HTTP server started");

    // modify TTL associated  with the domain name (in seconds)
    // default is 60 seconds
    dnsServer.setTTL(300);
    // set which return code will be used for all other domains (e.g. sending
    // ServerFailure instead of NonExistentDomain will reduce number of queries
    // sent by clients)
    // default is DNSReplyCode::NonExistentDomain
    dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);

    // start DNS server for a specific domain name
    dnsServer.start(DNS_PORT, "www.example.com", apIP);

    // simple HTTP server to see that DNS server is working
    webServer.onNotFound([]() {
        String message = "Hello World!\n\n";
        message += "URI: ";
        message += webServer.uri();

        webServer.send(200, "text/plain", message);
    });
    webServer.begin();
}

void loop() {
    dnsServer.processNextRequest();

    webServer.handleClient();
}
