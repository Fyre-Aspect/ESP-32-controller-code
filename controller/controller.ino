// controller.ino
// Upload this sketch to the CONTROLLER ESP32.
// Before uploading, replace the receiverMAC[] values below with the MAC address
// printed by mac_address_finder.ino when run on the RECEIVER ESP32.
//
// Example: if the MAC shown was  3C:8A:1F:B2:2D:90
// then set:  uint8_t receiverMAC[] = {0x3C, 0x8A, 0x1F, 0xB2, 0x2D, 0x90};

#include <esp_now.h>
#include <WiFi.h>

// *** REPLACE WITH YOUR RECEIVER ESP32 MAC ADDRESS ***
uint8_t receiverMAC[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

#define JOY_X_PIN   D12   // VRX
#define JOY_Y_PIN   D11   // VRY
#define BTN1_PIN    D10   // RAISE
#define BTN2_PIN    D13   // LOWER

typedef struct ControllerData {
  int   joyX;
  int   joyY;
  bool  btn1;
  bool  btn2;
} ControllerData;

ControllerData myData;
esp_now_peer_info_t peerInfo;

void setup() {
  Serial.begin(9600);
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed!");
    return;
  }

  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer!");
    return;
  }
  Serial.println("Controller ready! Sending data...");
}

void loop() {
  myData.joyX = analogRead(JOY_X_PIN);
  myData.joyY = analogRead(JOY_Y_PIN);
  myData.btn1 = !digitalRead(BTN1_PIN);
  myData.btn2 = !digitalRead(BTN2_PIN);

  esp_now_send(receiverMAC, (uint8_t *) &myData, sizeof(myData));
  delay(20);
}
