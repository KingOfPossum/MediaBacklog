#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "esp32_http.h"

char *esp32_make_igdb_request(char *url, char* query) {
  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  http.begin(url);
  
  http.addHeader("Client-ID", "");
  http.addHeader("Authorization", "Bearer ");
  http.addHeader("Accept", "application/json");

  int httpCode = http.POST((uint8_t*)query, strlen(query));
  char *result = NULL;

  if(httpCode > 0) {
    String payload = http.getString();
    result = strdup(payload.c_str());
  } 
  else {
    Serial.printf("ESP32 HTTP Error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  return result;
}