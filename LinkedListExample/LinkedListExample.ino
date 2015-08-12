#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <LinkedList.h>

class Wifi {
  public:
   
   char const* ssid;
   int rssi;
   int encryptionType;
   
   // virtual void f() { x=1; }
};

LinkedList<Wifi> wifis = LinkedList<Wifi>();


void setup() {
  
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");
}

void loop() {
  // put your main code here, to run repeatedly:

   Serial.println("scan start");

  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      
      Wifi* wifi = new Wifi();
      
      wifi->ssid = WiFi.SSID(i);
      wifi->rssi = WiFi.RSSI(i);
      wifi->encryptionType = WiFi.encryptionType(i);

      wifis.add(wifi);
      
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
    }
  }
  Serial.println("");
}
