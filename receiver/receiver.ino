// receiver.ino
// Upload this sketch to the RECEIVER ESP32 AFTER you have:
//   1. Run mac_address_finder.ino on this board and noted the MAC address.
//   2. Pasted that MAC address into controller.ino (receiverMAC[]).
//   3. Uploaded controller.ino to the CONTROLLER ESP32.
// Keep both ESP32s plugged in at the same time once both sketches are uploaded.

#include <esp_now.h>
#include <WiFi.h>

typedef struct ControllerData {
  int   joyX;
  int   joyY;
  bool  btn1;
  bool  btn2;
} ControllerData;

ControllerData incomingData;

#define DEAD_LOW   1200
#define DEAD_HIGH  2900

void onDataReceived(const esp_now_recv_info *recv_info, const uint8_t *data, int len) {
  memcpy(&incomingData, data, sizeof(incomingData));

  String moveCmd = "STOP";
  bool goForward  = (incomingData.joyY < DEAD_LOW);
  bool goBackward = (incomingData.joyY > DEAD_HIGH);
  bool turnLeft   = (incomingData.joyX < DEAD_LOW);
  bool turnRight  = (incomingData.joyX > DEAD_HIGH);

  if (goForward && turnLeft)        moveCmd = "FORWARD-LEFT";
  else if (goForward && turnRight)  moveCmd = "FORWARD-RIGHT";
  else if (goBackward && turnLeft)  moveCmd = "BACK-LEFT";
  else if (goBackward && turnRight) moveCmd = "BACK-RIGHT";
  else if (goForward)               moveCmd = "FORWARD";
  else if (goBackward)              moveCmd = "BACKWARD";
  else if (turnLeft)                moveCmd = "TURN-LEFT";
  else if (turnRight)               moveCmd = "TURN-RIGHT";

  String btnCmd = "NONE";
  if (incomingData.btn1)      btnCmd = "RAISE";
  else if (incomingData.btn2) btnCmd = "LOWER";

  Serial.print("Move: "); Serial.print(moveCmd);
  Serial.print("  |  Action: "); Serial.print(btnCmd);
  Serial.print("  |  X="); Serial.print(incomingData.joyX);
  Serial.print("  Y="); Serial.println(incomingData.joyY);
}

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed!");
    return;
  }
  esp_now_register_recv_cb(onDataReceived);
  Serial.println("Receiver ready. Waiting for controller...");
}

void loop() {
  delay(10);
}
