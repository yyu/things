#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>

const static uint EEPROM_SIZE = 512;

class Parameters {
public:
    void load() {
        _read_eeprom(_eeprom_all);
        _populate_params(_eeprom_all);
    }

    char * get_ssid_as_ap() { return _ssid_as_ap; }
    char * get_pwd_as_ap() { return _pwd_as_ap; }
    char * get_ssid() { return _ssid; }
    char * get_pwd() { return _pwd; }
    char * get_hue_auth() { return _hue_auth; }

private:
    char * _read_eeprom(char * eeprom_all) {
        memset(eeprom_all, 0, EEPROM_SIZE);

        EEPROM.begin(EEPROM_SIZE);
        for (uint i = 0; i < EEPROM_SIZE; i++) {
            eeprom_all[i] = EEPROM.read(i);
        }
        EEPROM.end();

        return eeprom_all;
    }

    void _populate_params(char * eeprom_all) {
        char * p = eeprom_all;

        while (*p) {
            char * key = p;
            char * eq = _find_char(p, '=');
            *eq = 0;

            char * value = eq + 1;
            char * spc = _find_char(value, ' ');
            *spc = 0;

            p = spc + 1;

            if (strcmp(key, "SSID_AS_AP") == 0) {
                _ssid_as_ap = value;
            } else if (strcmp(key, "PASSWORD_AS_AP") == 0) {
                _pwd_as_ap = value;
            } else if (strcmp(key, "SSID") == 0){
                _ssid = value;
            } else if (strcmp(key, "PASSWORD") == 0) {
                _pwd = value;
            } else if (strcmp(key, "HUE_AUTH") == 0) {
                _hue_auth = value;
            }

            Serial.print(key);
            Serial.print(": ");
            Serial.print(value);
            Serial.println();
        }
    }

    char * _find_char(char * s, char c) {
        char * p = s;
        for (; *p; p++) {
            if (*p == c) {
                return p;
            }
        }
        return p;
    }

    char _eeprom_all[EEPROM_SIZE];
    char * _ssid_as_ap = 0;
    char * _pwd_as_ap = 0;
    char * _ssid = 0;
    char * _pwd = 0;
    char * _hue_auth = 0;
};

Parameters params;
const byte DNS_PORT = 53;
IPAddress ap_ip(192, 168, 4, 1);
DNSServer dns_server;
WiFiUDP udp;
bool status = true; // for hue light bulb
unsigned long timestamp = 0;

void init_wifi() {
    char * ap = params.get_ssid_as_ap();
    char * password_as_ap = params.get_pwd_as_ap();
    char * ssid = params.get_ssid();
    char * password = params.get_pwd();

    Serial.println();

    Serial.println(ap);
    Serial.println(password_as_ap);
    Serial.println(ssid);
    Serial.println(password);

    Serial.println("AP");
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ap); // , password_as_ap, /* channel */ 1, /* hide ssid */ 0, /* max_connection */ 4);

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

void init_dns_server() {
    // modify TTL associated  with the domain name (in seconds)
    // default is 60 seconds
    dns_server.setTTL(300);

    // set which return code will be used for all other domains (e.g. sending ServerFailure
    // instead of NonExistentDomain will reduce number of queries sent by clients)
    // default is DNSReplyCode::NonExistentDomain
    dns_server.setErrorReplyCode(DNSReplyCode::ServerFailure);

    // start DNS server for a specific domain name
    dns_server.start(DNS_PORT, "0.amazon.pool.ntp.org", ap_ip);
}

void init_udp() {
    udp.begin(123);

    Serial.println("UDP server started");
}

void showClients()
{
    struct station_info *stat_info;
    stat_info = wifi_softap_get_station_info();
    uint8_t client_count = wifi_softap_get_station_num();
    String str = "Number of clients = ";
    str += String(client_count);
    str += "\r\nList of clients : \r\n";
    int i = 1;
    while (stat_info) {
        str += "Station #";
        str += String(i);
        str += " : ";
        str += String(stat_info->bssid[0], HEX);
        str += ":";
        str += String(stat_info->bssid[1], HEX);
        str += ":";
        str += String(stat_info->bssid[2], HEX);
        str += ":";
        str += String(stat_info->bssid[3], HEX);
        str += ":";
        str += String(stat_info->bssid[4], HEX);
        str += ":";
        str += String(stat_info->bssid[5], HEX);

        IPAddress ip(stat_info->ip.addr);
        str += " ";
        str += ip;
        str += " ~ ";

        for (int k = 0; k < 4; k++) {
            Serial.print(ip[k], DEC);
            if (k < 3) {
                Serial.print(".");
            }
        }

        str += "\r\n";

        i++;
        stat_info = STAILQ_NEXT(stat_info, next);
    }

    Serial.println(str);
}

void switch_hue() {
    char * hue_auth = params.get_hue_auth();
    Serial.print("<");
    Serial.print(hue_auth);
    Serial.println(">");

    unsigned long now = millis();

    if (now - timestamp < 10000) {
        return;
    }

    timestamp = now;

    HTTPClient http;

    String url_prefix = "http://192.168.10.110/api/";
    String url = url_prefix + hue_auth + "/lights/2/state";

    Serial.println(url);

    if (!http.begin(url)) {
        Serial.println("failed to begin http");
        return;
    }
//    http.addHeader("Content-Type", "text/plain");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("Accept", "*/*");

    Serial.println("before HTTP PUT");
    int httpCode;
    if (status) {
        httpCode = http.PUT("{\"on\":false}");
    } else {
        httpCode = http.PUT("{\"on\":true}");
    }
    if (httpCode == 200) {
        status = !status;
        showClients();
    }
    Serial.println(" after HTTP PUT");
    Serial.println(httpCode);

    String response = http.getString();

    Serial.println(response);

    http.end();
}

// buffers for receiving and sending data
char packet_buffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char reply_buffer[] = "acknowledged";       // a string to send back

bool should_switch_hue = false;

void handle_ntp_request() {
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

        should_switch_hue = true;

        // read the packet into packetBufffer
        udp.read(packet_buffer, UDP_TX_PACKET_MAX_SIZE);
        Serial.println("Contents:");
        Serial.println(packet_buffer);

        // send a reply, to the IP address and port that sent us the packet we received
        udp.beginPacket(udp.remoteIP(), udp.remotePort());
        udp.write(reply_buffer);
        udp.endPacket();
    }
}

void setup() {
    delay(10000); // so that I have plenty of time to open serial monitor

    Serial.begin(115200);

    params.load();

    init_wifi();
    init_dns_server();
    init_udp();
}

void loop() {
    should_switch_hue = false;

//    switch_hue();
    dns_server.processNextRequest();

    handle_ntp_request();

    if (should_switch_hue) {
        switch_hue();
    }

    delay(100);
}
