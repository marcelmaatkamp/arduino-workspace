/*
    This sketch demonstrates how to scan WiFi networks.
    The API is almost the same as with the WiFi Shield library,
    the most obvious difference being the different file you need to include:
*/
#include "ESP8266WiFi.h"
#include "lwip/ip_addr.h"
#include "lwip/err.h"
#include "lwip/dns.h"

extern "C" void esp_yield();
extern "C" void esp_schedule();

String LOC = ".ad.uk.ms";

bool
debug = false,
TIMEOUT = false,
SUCCESS = true;

const int
WIFI_CONNECT_MAX_RETRY = 3,
WIFI_DELAY_BETWEEN_CONNECTS = 50,
BLINK_DELAY = 250,
LED_RED = 15, LED_GREEN = 12, LED_BLUE = 13, LED_MIN = 0, LED_MAX = PWMRANGE;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.stopSmartConfig();

  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output

  blink(); blink(); blink();
  Serial.println("Setup done");
}


void led_off() { 
  analogWrite(LED_RED, LED_MIN);
  analogWrite(LED_GREEN, LED_MIN);
  analogWrite(LED_BLUE, LED_MIN);
}
void led_red() { 
  analogWrite(LED_RED, LED_MAX);
  analogWrite(LED_GREEN, LED_MIN);
  analogWrite(LED_BLUE, LED_MIN);
}
void led_green() { 
  analogWrite(LED_RED, LED_MIN);
  analogWrite(LED_GREEN, LED_MAX);
  analogWrite(LED_BLUE, LED_MIN);
}
void led_blue() { 
  analogWrite(LED_RED, LED_MIN);
  analogWrite(LED_GREEN, LED_MIN);
  analogWrite(LED_BLUE, LED_MAX);
}
void led_white() { 
  analogWrite(LED_RED, LED_MAX);
  analogWrite(LED_GREEN, LED_MAX);
  analogWrite(LED_BLUE, LED_MAX);
}

String getBSSIDMac(int index) {
  String result = String("");

  uint8_t* bssid = WiFi.BSSID(index);
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

bool connectToSSID(String ssid, int channel, uint8_t bssid[6], String bssidStr, int retry) {
  Serial.print("[");
  Serial.print(ssid);
  Serial.print("/");
  Serial.print(bssidStr);
  Serial.print("/");
  Serial.print(channel);
  Serial.print("/");
  Serial.print(retry);
  Serial.print("]: connecting.. ");

  if (debug) {
    WiFi.printDiag(Serial);
    Serial.setDebugOutput(true);
  }

  unsigned long now = millis();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), NULL);

  while (millis() - now < 5000) {
    if (debug) {
      Serial.print("[");
      Serial.print(WiFi.status());
      Serial.print("/");
      Serial.print(WiFi.channel());
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

  Serial.println("timeout!");
  return TIMEOUT;
}

void sendUdpStrings(int n) {
  String timer = String(ESP.getCycleCount());
  String id = String(ESP.getChipId());

  for (int i = 0; i < n; i++) {
    String host = "s" + String(WiFi.RSSI(i)) + "." + getBSSIDMac(i) + "." + timer + "." + id + "." + String(n - i - 1) + LOC;

    int str_len = host.length() + 1;
    char char_array[str_len];
    host.toCharArray(char_array, str_len);

    struct ip_addr resolved;
    err_t err = dns_gethostbyname(char_array, &resolved, dummy_callback, NULL);
    if(err == ERR_OK) {
      led_green();
    }
    if (err == ERR_INPROGRESS) {
      esp_yield();
      led_green();
    }
  }
}

void dummy_callback(const char *name, ip_addr_t *ipaddr, void *callback_arg) {
  esp_schedule(); // resume the hostByName function
}

void blink() {
  led_white();
  digitalWrite(BUILTIN_LED, HIGH);
  delay(BLINK_DELAY);
  
  digitalWrite(BUILTIN_LED, LOW);
  led_off();
}

// Dont's use these known AP's for DNS tunneling

String ignoreSSIDs[] = {"Ziggo", "Sitecom_2G", ""};

bool sitesWholeMatch(String name) {
  bool ret = false;
  int i = 0;
  while (ignoreSSIDs[i] != "" && !ret) {
    if (ignoreSSIDs[i] == name)
      ret = true;
    i++;
  }
  return ret;
}

void loop() {
  Serial.print("scan.. ");
  int n = WiFi.scanNetworks(false);
  Serial.print("found ");
  Serial.print(n);
  Serial.println(" networks.");

  if (n > 0) {
    for (int i = 0; i < n; i++) {
      if (!sitesWholeMatch(WiFi.SSID(i))) {
        if (WiFi.encryptionType(i) == 7) {
          int retry = 0;
          while (retry < WIFI_CONNECT_MAX_RETRY) {
            led_red();
            if (connectToSSID(WiFi.SSID(i), WiFi.channel(i), WiFi.BSSID(i), WiFi.BSSIDstr(i), retry) == SUCCESS) {
              sendUdpStrings(n);
              WiFi.disconnect(false);
              retry = WIFI_CONNECT_MAX_RETRY;
            }
            led_off();
            retry++;
          }
        }
      }
    }
  }
  led_off();
}
