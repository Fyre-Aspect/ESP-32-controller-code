# ESP-32-controller-code

ESP-NOW wireless controller system for ESP32. One ESP32 acts as the **controller** (joystick + buttons) and another acts as the **receiver** (outputs movement/action commands over Serial).

---

## ⚠️ Hey SAI — read this before uploading anything

Follow these steps **in order** every time you set up a new receiver ESP32.

---

## Step 1 — Get the MAC address of the Receiver ESP32

1. Plug the **receiver** ESP32 into your computer.
2. Open `mac_address_finder/mac_address_finder.ino` in the Arduino IDE.
3. Upload it to the receiver ESP32.
4. Open the **Serial Monitor** (baud rate **9600**).
5. You will see a line like:

   ```
   Receiver ESP32 MAC Address: 3C:8A:1F:B2:2D:90
   ```

6. **Copy that MAC address** — you need it in Step 2.

---

## Step 2 — Paste the MAC address into the Controller sketch

1. Open `controller/controller.ino`.
2. Find this line near the top:

   ```cpp
   uint8_t receiverMAC[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
   ```

3. Replace the placeholder bytes with the MAC address you copied.  
   For example, `3C:8A:1F:B2:2D:90` becomes:

   ```cpp
   uint8_t receiverMAC[] = {0x3C, 0x8A, 0x1F, 0xB2, 0x2D, 0x90};
   ```

4. Save the file.
5. Plug in the **controller** ESP32 and upload `controller/controller.ino` to it.

---

## Step 3 — Upload the Receiver sketch

1. **Keep the controller ESP32 plugged in.**
2. Switch back to the **receiver** ESP32 (same board you used in Step 1).
3. Open `receiver/receiver.ino` in the Arduino IDE.
4. Upload it to the receiver ESP32.

---

## Step 4 — Run both at the same time

1. Keep **both** ESP32s plugged in and powered.
2. Open the Serial Monitor on the receiver ESP32 (baud rate **9600**).
3. Move the joystick / press buttons on the controller — you should see output like:

   ```
   Receiver ready. Waiting for controller...
   Move: FORWARD  |  Action: NONE  |  X=2048  Y=500
   Move: TURN-LEFT  |  Action: RAISE  |  X=900  Y=2048
   ```

---

## File overview

| File | Purpose |
|------|---------|
| `mac_address_finder/mac_address_finder.ino` | Upload to receiver ESP32 first — prints its MAC address |
| `controller/controller.ino` | Upload to controller ESP32 — reads joystick & buttons, sends data via ESP-NOW |
| `receiver/receiver.ino` | Upload to receiver ESP32 after Step 2 — receives data and prints movement commands |

---

## Pin mapping (Controller ESP32)

| Pin | Connected to |
|-----|-------------|
| D12 | Joystick VRX |
| D11 | Joystick VRY |
| D10 | Button 1 (RAISE) |
| D13 | Button 2 (LOWER) |

Buttons use `INPUT_PULLUP` — connect one side to the pin, the other side to GND.

---

## Deadzone values (Receiver)

The receiver ignores small joystick movements using a deadzone:

| Constant | Value | Meaning |
|----------|-------|---------|
| `DEAD_LOW` | 1200 | Below this → axis is fully deflected low |
| `DEAD_HIGH` | 2900 | Above this → axis is fully deflected high |

Adjust these in `receiver/receiver.ino` if your joystick centre sits outside 1200–2900.
