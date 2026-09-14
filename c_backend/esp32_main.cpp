#include <Arduino.h>
#include <LittleFS.h>

#include <stdlib.h>

#include <esp32_wifi.h>
#include <databaseManager.h>
#include <server.h>

void setup() {
  Serial.begin(115200);
  
  Serial.printf("----------------WiFi--------------\n");

  connect_wifi();

  Serial.printf("----------------------------------\n\n");

  if(!LittleFS.begin(true)) {
    Serial.println("Error while mounting filesystem");
    return;
  }

  Serial.printf("--------------Database------------\n");

  init_database("/littlefs/database.db");

  Serial.printf("----------------------------------\n\n");

  Serial.printf("--------------Server--------------\n");

  server();

  Serial.printf("----------------------------------\n\n");

  close_database();
}

void loop() {
  delay(10);
}