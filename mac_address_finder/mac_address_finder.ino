// mac_address_finder.ino
// Upload this sketch to the RECEIVER ESP32 first.
// Open the Serial Monitor at 9600 baud and copy the MAC address shown.
// Paste that MAC address into controller.ino (receiverMAC[]).
// Then upload receiver.ino to this same ESP32.

#include <WiFi.h>

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Receiver ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  // Nothing to do here
}
