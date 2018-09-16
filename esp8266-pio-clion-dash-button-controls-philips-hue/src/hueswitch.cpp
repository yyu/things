#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266HTTPClient.h>

const char * ap = "*";
const char * ssid = "*";
const char * password = "*";
const char * HUE_AUTH = "*";

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged";       // a string to send back

WiFiUDP udp;

bool status = true; // for hue light bulb

void initWiFi() {
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ap);
    WiFi.begin(ssid, password);

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void initDNSServer() {
    // modify TTL associated  with the domain name (in seconds)
    // default is 60 seconds
    dnsServer.setTTL(300);

    // set which return code will be used for all other domains (e.g. sending ServerFailure
    // instead of NonExistentDomain will reduce number of queries sent by clients)
    // default is DNSReplyCode::NonExistentDomain
    dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);

    // start DNS server for a specific domain name
    dnsServer.start(DNS_PORT, "0.amazon.pool.ntp.org", apIP);
}

void initUDP() {
    udp.begin(123);

    Serial.println("UDP server started");
}

void switch_hue() {
    HTTPClient http;

    String url_prefix = "http://192.168.10.110/api/";
    String url = url_prefix + HUE_AUTH + "/lights/2/state";

    http.begin(url);
    http.addHeader("Content-Type", "text/plain");

    int httpCode;
    if (status) {
        httpCode = http.PUT("{\"on\":false}");
        status = false;
    } else {
        httpCode = http.PUT("{\"on\":true}");
        status = true;
    }

    String response = http.getString();

    Serial.println(httpCode);
    Serial.println(response);

    http.end();
}

void setup() {
    delay(10000); // so that I have plenty of time to open serial monitor

    Serial.begin(115200);
    Serial.println();
    Serial.print("Configuring access point...");

    initWiFi();
    initDNSServer();
    initUDP();
}

void loop() {
    dnsServer.processNextRequest();

    // if there's data available, read a packet
    int packetSize = udp.parsePacket();
    if (packetSize) {
        Serial.print("Received packet of size ");
        Serial.println(packetSize);
        Serial.print("From ");
        IPAddress remote = udp.remoteIP();
        for (int i = 0; i < 4; i++) {
            Serial.print(remote[i], DEC);
            if (i < 3) {
                Serial.print(".");
            }
        }
        Serial.print(", port ");
        Serial.println(udp.remotePort());

        // read the packet into packetBufffer
        udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
        Serial.println("Contents:");
        Serial.println(packetBuffer);

        // send a reply, to the IP address and port that sent us the packet we received
        udp.beginPacket(udp.remoteIP(), udp.remotePort());
        udp.write(ReplyBuffer);
        udp.endPacket();

        switch_hue();
    }

    delay(10000); // so that the light won't be turned on/off too frequently
}
