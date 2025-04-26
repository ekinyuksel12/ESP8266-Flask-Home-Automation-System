#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "LocalAreaNetwork";
const char* password =  "99e42C63d97C3";

WiFiServer wifiServer(8080);

int gpio0_status = 0;
int gpio2_status = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  
  Serial.begin(115200);

  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);

  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());

  wifiServer.begin();
}

void loop() {
  ArduinoOTA.handle();

  WiFiClient client = wifiServer.available();

  if (client) {
    Serial.print("Client (");
    Serial.print(client.remoteIP());
    Serial.println(") Connected.");
    client.print("Connected to ESP8266 (");
    client.print(WiFi.localIP());
    client.println(").");
    while (client.connected()) {
      String input = "";
      while (client.available() > 0) {
        char c = client.read();
        input.concat(c);
      }
      if (client.available() == 0 && input != "") {
        if (input.substring(0, 4) == "gpio") {
          char c = input[4];
          String status_change = input.substring(6, input.length() - 1);
          
          if (status_change.substring(0,4) == "show") {
            if (c == '0') {
              client.print("GPIO0 = ");
              client.println(gpio0_status);
            } else if (c == '2') {
              client.print("GPIO2 = ");
              client.println(gpio2_status);
            }
          } else {
            switch (c) {
              case '0':
                if (status_change.substring(0, 2) == "on") {
                  digitalWrite(0, HIGH);
                  gpio0_status = 1;
                } else if (status_change.substring(0, 2) == "of") {
                  digitalWrite(0, LOW);
                  gpio0_status = 0;
                }
                client.print("ok. GPIO0 = ");
                client.println(gpio0_status);
                Serial.print("GPIO0 = ");
                Serial.println(gpio0_status);
                break;
              case '2':
                if (status_change.substring(0, 2) == "on") {
                  digitalWrite(2, HIGH);
                  gpio2_status = 1;
                } else if (status_change.substring(0, 2) == "of") {
                  digitalWrite(2, LOW);
                  gpio2_status = 0;
                }
                client.print("ok. GPIO2 = ");
                client.println(gpio2_status);
                Serial.print("GPIO2 = ");
                Serial.println(gpio2_status);
                break;
              default:
                break;
            }
          }
        } else {
          client.println("error");
        }
      }
      delay(10);
    }

    client.stop();
    Serial.print("Client Disconnected.");
  }
}
