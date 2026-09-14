#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>

const char *wlan_ssid = "MyWlan2";
const char *wlan_password = ",4T$NPD8g9>C";

extern "C" void connect_wifi() {
  WiFi.begin(wlan_ssid, wlan_password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(50);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  if(MDNS.begin("esp")) {
    Serial.println("mDNS started! Now availlable through: http://esp.local");
  }
}