#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <LinkedList.h>

bool debug = false;

class Wifi {
  public: 
   char const* ssid;
   int rssi;
   int encryptionType;

};

class WifiScan {
  public:
    WifiScan( int size )
        : wifis( size ) 
    {}
    std::vector<Wifi*> wifis;

   private:

};

LinkedList<Wifi*> wifis = LinkedList<Wifi*>();

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
  if(debug) { 
    WiFi.printDiag(Serial);
    Serial.setDebugOutput(true);
  }
  
  Serial.println("Setup done");
}

WifiScan* wifiScan = new WifiScan(0);

void loop() {

  Serial.print("scan .. ");

  int n = WiFi.scanNetworks();
  Serial.print(" found ");
  Serial.print(n);
  Serial.println(" networks.");
      
    for (int i = 0; i < n; ++i) {

      Wifi* wifi = new Wifi();
      
      wifi->ssid = WiFi.SSID(i);
      wifi->rssi = WiFi.RSSI(i);
      wifi->encryptionType = WiFi.encryptionType(i);
      
      wifiScan->wifis.push_back(wifi);
    }


    for(std::vector<Wifi*>::iterator it = wifiScan->wifis.begin(); it != wifiScan->wifis.end(); ++it) {
      Wifi* wifi = (*it);

      Serial.print("|");
      Serial.print(wifi->encryptionType);
      Serial.print("|");
      Serial.print(wifi->rssi);
      Serial.print("|");
      Serial.print(wifi->ssid);
      Serial.println("|");

      if(wifi->encryptionType==4) { 
        
        if (WiFi.status() == WL_CONNECTED) {

        
      }

      // TKIP (WPA) = 2
      // WEP = 5
      // CCMP (WPA) = 4
      // NONE = 7
      // AUTO = 8

      // delete: http://stackoverflow.com/questions/4645705/vector-erase-iterator

      delete(wifi);
    }   
    wifiScan->wifis.clear();



    // bool success = wifis.add(wifiScan);
      
 
  Serial.println("");
}
