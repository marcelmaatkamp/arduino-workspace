#include <SPI.h>

/*
 *  This sketch demonstrates how to scan WiFi networks. 
 *  The API is almost the same as with the WiFi Shield library, 
 *  the most obvious difference being the different file you need to include:
 */
#include "ESP8266WiFi.h"
#include <Dns.h>
#include <Ethernet.h>




bool debug = false, TIMEOUT = false, SUCCESS = true;

const int WIFI_CONNECT_MAX_RETRY = 3;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
  if(debug) { 
    WiFi.printDiag(Serial);
    Serial.setDebugOutput(true);
    printMacAddress();
  }
  
  Serial.println("Setup done");
}

void printMacAddress() {
  byte mac[6];                     
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
}

void printWifiStatus(int i) { 
  Serial.print("|");
  Serial.print(WiFi.encryptionType(i));
  Serial.print("|");
  Serial.print(WiFi.RSSI(i));
  Serial.print("|");
  Serial.print(WiFi.SSID(i));
  Serial.println("|");
}

bool connectToSSID(const char* ssid) {
  Serial.print("[");
  Serial.print(ssid);
  Serial.print("]: connecting.. ");
  
  WiFi.begin(ssid);
  unsigned long now = millis();
  while (millis() - now < 5000) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("]: connected: ");
      Serial.println(WiFi.localIP());
      return SUCCESS;
    }
    delay(100);
  }
  Serial.println("timeout!");
  return TIMEOUT;

    // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);
  
  // print your MAC address:
  byte mac[6];  
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
  
  // print your subnet mask:
  IPAddress subnet = WiFi.subnetMask();
  Serial.print("NetMask: ");
  Serial.println(subnet);

  // print your gateway address:
  IPAddress gateway = WiFi.gatewayIP();
  Serial.print("Gateway: ");
  Serial.println(gateway);
}

void sendUdpStrings(int i) { 

  DNSClient dns;
  dns.begin(Ethernet.dnsServerIP());
  IPAddress ip1;
  dns.getHostByName("www.google.com", ip1);
  Serial.print("www.google.com: ");
  Serial.println(ip1);
}


void loop() {
  Serial.print("scan.. ");
  int n = WiFi.scanNetworks();
  Serial.print("found ");
  Serial.print(n);
  Serial.println(" networks.");

  if(n>0) {
    for (int i = 0; i < n; ++i) {
      printWifiStatus(i);
    }
    for (int i = 0; i < n; ++i) {
      
      if(WiFi.encryptionType(i) == 7) { 
        int retry = 0;

        while(retry < WIFI_CONNECT_MAX_RETRY) { 
          if(connectToSSID(WiFi.SSID(i)) == SUCCESS) { 
            

            sendUdpStrings(i);
          }
          retry++;
        }
        
      }
    }
  }
  Serial.println("");
}
