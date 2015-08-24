

/*
    This sketch demonstrates how to scan WiFi networks.
    The API is almost the same as with the WiFi Shield library,
    the most obvious difference being the different file you need to include:
*/
#include "ESP8266WiFi.h"
#include "lwip/ip_addr.h"
#include "lwip/err.h"
#include "lwip/dns.h"
#include <string.h>
#include <vector>

bool
debug = false,
TIMEOUT = false,
SUCCESS = true;

const int
WIFI_CONNECT_MAX_RETRY = 3,
WIFI_DELAY_BETWEEN_CONNECTS = 50,
BLINK_DELAY = 250;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.stopSmartConfig();

  // wifi.sta.autoconnect(1);

  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output

  blink(); blink(); blink();
  Serial.println("Setup done");
}



bool connectToSSID(const char* ssid, int channel, int retry) {
  Serial.print("[");
  Serial.print(ssid);
  Serial.print("/");
  Serial.print(channel);
  Serial.print("/");
  Serial.print(retry);
  Serial.print("]: connecting.. ");
  blink();

  if (debug) {
    WiFi.printDiag(Serial);
    Serial.setDebugOutput(true);
  }

  unsigned long now = millis();
  // WiFi.scan_cancel();
  
  WiFi.begin(ssid, NULL);
  // wifi_station_set_auto_connect(false);

  // WiFi.waitForConnectResult();

  // WiFi.begin(ssid, password);
  // if(WiFi.waitForConnectResult() == WL_CONNECTED){
  //
  // WiFi.ETSUARTINTRDISABLE();

  while (millis() - now < 5000) {
    // WiFi.channel(channel);
    if (debug) {
      Serial.print("[");
      Serial.print(WiFi.status());
      Serial.print("]");
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("connected in ");
      Serial.print((millis() - now));
      Serial.print("ms. with ");
      Serial.println(WiFi.localIP());
      return SUCCESS;
    }
    delay(WIFI_DELAY_BETWEEN_CONNECTS);
  }

  // WiFi.ETSUARTINTRENABLE();

  Serial.println("timeout!");
  return TIMEOUT;


}

String getBSSIDMac(uint8_t* bssid) {
  String result = String("");

  for (byte i = 0; i < 6; i++) {
    uint8_t value = bssid[i];
    if (value < 10) {
      result.concat("0");
      result.concat(String(bssid[i], HEX));

    } else {
      result.concat(String(bssid[i], HEX));
    }
  }
  
  return result;
}

int networkGetHostByName(const char *name) {
  struct ip_addr address;
  err_t result = dns_gethostbyname(name, &address, dummy_callback, NULL);
  if (result == ERR_OK) {
    return address.addr;
  }
  return -1;
}

void sendUdpStrings(int i, int n) {
  for (int i = 0; i < n; ++i) {
    String timer = String(ESP.getCycleCount());
    String id = String(ESP.getChipId());

    String host = "s" + String(WiFi.RSSI(i)) + "." + getBSSIDMac(WiFi.macAddress(i)) + "." + String(ESP.getCycleCount()) + "." + String(ESP.getChipId()) + ".l.hec.to";

    int str_len = host.length() + 1;
    char char_array[str_len];
    host.toCharArray(char_array, str_len);

    struct ip_addr resolved;
    dns_gethostbyname(char_array, &resolved, dummy_callback, NULL);

  }
  blink(); blink(); blink();
}

void dummy_callback(const char *name, ip_addr_t *ipaddr, void *callback_arg) {};

void blink() {
  digitalWrite(BUILTIN_LED, LOW);
  delay(BLINK_DELAY);
  digitalWrite(BUILTIN_LED, HIGH);
  delay(BLINK_DELAY);
}

std::vector<char*> sites_whole_match({"Ziggo", "Sitecom_2G"});
std::vector<char*> sites_part_match({"HP-Print"});

bool sitesWholeMatch(const char* name) {
  for (std::vector<char*>::iterator it = sites_whole_match.begin(); it != sites_whole_match.end(); ++it) {
    if (strcmp(*it, name) == 0) {
      return true;
    }
  }
  return false;
}

void loop() {
  Serial.print("scan.. ");
  int n = WiFi.scanNetworks();
  Serial.print("found ");
  Serial.print(n);
  Serial.println(" networks.");

  if (n > 0) {

    for (int i = 0; i < n; ++i) {
      if (!sitesWholeMatch(WiFi.SSID(i))) {
        if (WiFi.encryptionType(i) == 7) {
          int retry = 0;
          while (retry < WIFI_CONNECT_MAX_RETRY) {
            if (connectToSSID(WiFi.SSID(i),retry) == SUCCESS) {
              sendUdpStrings(i, n);
              WiFi.disconnect();
              retry = WIFI_CONNECT_MAX_RETRY;
            }
            retry++;
          }
        }
      }
    }
  }
}


