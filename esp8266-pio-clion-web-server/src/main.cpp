#include <ESP8266WiFi.h>
#include <Servo.h>
//#include <LiquidCrystal.h>
//
//const int RS = D1, EN = D2, d4 = D4, d5 = D5, d6 = D6, d7 = D7;
//LiquidCrystal lcd(RS, EN, d4, d5, d6, d7);

const int servopin = 15;

WiFiServer server(8888);
Servo blueservo;

void initWiFi() {
    const char * ap = "Guest8266";
    const char * password_as_ap = "";
    const char * ssid = "N2G";
//    const char * ssid = "Guest";
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
//    WiFi.mode(WIFI_STA);
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

void servogo(int a) {
    if (!blueservo.attached()) {
        blueservo.attach(servopin);
    }
    blueservo.write(a);
    delay(1000);
//    blueservo.detach();
}

void lcdprint(const String& s)
{
//    lcd.setCursor(0, 1);
//    lcd.print(s);
}

void lcdprint(const int& i)
{
    String s;
    s.concat(i);
    lcdprint(s);
}

void setup()
{
//    lcd.begin(16, 2);

    delay(3000);
    Serial.begin(115200);
    Serial.println();
    Serial.print("Configuring access point...");

    initWiFi();

//    lcd.print(WiFi.softAPIP().toString());

    server.begin();
    Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());

    blueservo.attach(servopin);
    Serial.printf("angle: %d \n", blueservo.read());
    servogo(60);
    Serial.printf("angle: %d \n", blueservo.read());
}


// prepare a web page to be send to a client (web browser)
String prepareHtmlPage()
{
  String htmlPage =
          String("HTTP/1.1 200 OK\r\n") +
          "Content-Type: text/html\r\n" +
          "Connection: close\r\n" +  // the connection will be closed after completion of the response
          "Refresh: 5\r\n" +  // refresh the page automatically every 5 sec
          "\r\n" +
          "<!DOCTYPE HTML>" +
          "<html>" +
          "Analog input:  " + String(analogRead(A0)) +
          "</html>" +
          "\r\n";
  return htmlPage;
}

void loop()
{
    WiFiClient client = server.available();
    // wait for a client (web browser) to connect
    if (client)
    {
        Serial.println("\n[Client connected]");
        while (client.connected())
        {
            // read line by line what the client (web browser) is requesting
            if (client.available())
            {
                String line = client.readStringUntil('\r');
                Serial.print(line);
                // wait for end of client's request, that is marked with an empty line
                if (line.length() == 1 && line[0] == '\n')
                {
                    Serial.println("printing html");
                    client.println(prepareHtmlPage());
                    break;
                }

                int k = line.indexOf("angle");
                Serial.printf("<k:%d>\n", k);

                if (k != -1) {
                    int endidx = line.indexOf("degree");
                    int a = line.substring(k + 5, endidx).toInt();
                    Serial.printf("<a:%d>\n", a);
                    lcdprint(a);
                    servogo(a);
                } else if (line.indexOf("north") != -1) {
                    servogo(0);
                } else if (line.indexOf("south") != -1) {
                    servogo(180);
                } else if (line.indexOf("center") != -1) {
                    servogo(90);
                } else if (line.indexOf("stop") != -1) {
                    if (blueservo.attached()) {
                        blueservo.detach();
                    }
                }
            }
        }
        delay(1); // give the web browser time to receive the data

        // close the connection:
        client.stop();
        Serial.println("[Client disconnected]");
    }
}