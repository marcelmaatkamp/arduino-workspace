/*
 ESP8266 Blink by Simon Peter
 Blink the blue LED on the ESP-01 module
 This example code is in the public domain
 
 The blue LED on the ESP-01 module is connected to GPIO1 
 (which is also the TXD pin; so we cannot use Serial.print() at the same time)
 
 Note that this sketch uses BUILTIN_LED to find the pin with the internal LED
*/

#include <ESP8266WiFi.h>

const char* ssid     = "";
const char* password = "";

const char* host = "172.16.*.*"; // ifconfig host machine
int port = 8080;

int oldgpio = 0;
WiFiClient client;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output

  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() {

  int gpio = digitalRead(2);

  if(oldgpio != gpio) { 
 
    Serial.print("connecting to ");
    Serial.println(host);
  
    if (!client.connect(host, port)) {
      Serial.println("connection failed");
      return;
    }
  
    String url = "/input/motion?";
    
    url += "espid=";
    url += ESP.getChipId();
    
    url += "&gpio=";
    url += gpio;
  
    Serial.print("Requesting URL: ");
    Serial.println(url);
  
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
    delay(10);
  
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
  
    // Serial.println();
    Serial.println("closing connection");

    oldgpio = gpio;
  }

  delay(250);

}
