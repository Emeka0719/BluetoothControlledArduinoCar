# 🚗 Bluetooth Controlled Arduino Car

A fully functional 2-wheel-drive Arduino car controlled over Bluetooth with variable speed via PWM.

## 📦 Components

| Component | Qty |
|---|---|
| Arduino Uno / Nano | 1 |
| L298N Motor Driver | 1 |
| HC-05 or HC-06 Bluetooth Module | 1 |
| DC Gear Motors (TT motors) | 2 |
| Wheels + chassis | 1 set |
| 7.4V Li-Po or 4×AA battery pack | 1 |
| Jumper wires | — |

## 🔌 Wiring

```
L298N  IN1  →  Arduino Pin 2
L298N  IN2  →  Arduino Pin 3
L298N  IN3  →  Arduino Pin 4
L298N  IN4  →  Arduino Pin 5
L298N  ENA  →  Arduino Pin 9   (PWM)
L298N  ENB  →  Arduino Pin 10  (PWM)
L298N  GND  →  Arduino GND
L298N  +5V  →  Arduino 5V
L298N  +12V →  Battery (+)

HC-05 TX    →  Arduino Pin 11  (SoftwareSerial RX)
HC-05 RX    →  Arduino Pin 12  via voltage divider (1kΩ / 2kΩ)
HC-05 VCC   →  Arduino 5V
HC-05 GND   →  Arduino GND
```

> ⚠️ **Voltage Divider on HC-05 RX**: The HC-05 RX pin is 3.3 V tolerant. Use a 1 kΩ + 2 kΩ divider between Arduino TX (pin 12) and HC-05 RX to step 5 V → ~3.3 V.

## 📲 Bluetooth Commands

Send these characters from any Bluetooth serial terminal app (e.g. **Serial Bluetooth Terminal** on Android):

| Command | Action |
|---|---|
| `F` | Move Forward |
| `B` | Move Backward |
| `L` | Turn Left (pivot) |
| `R` | Turn Right (pivot) |
| `S` | Stop |
| `1` | Speed: Low (40%) |
| `2` | Speed: Medium (70%) — *default* |
| `3` | Speed: High (100%) |

Commands are **case-insensitive** (`f` = `F`).

## 🛠️ Setup

1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Open `bluetooth_car/bluetooth_car.ino`.
3. Select your board (**Tools → Board → Arduino Uno**).
4. Select the correct COM port.
5. Upload the sketch.
6. Pair your phone to the HC-05/HC-06 (default PIN: `1234` or `0000`).
7. Open a Bluetooth serial app and connect to the module.
8. Send commands and drive!

## 📁 Project Structure

```
arduino-bt-car/
├── bluetooth_car/
│   └── bluetooth_car.ino   # Main sketch
└── README.md
```

## ⚙️ Customising Speed

Speed presets are defined at the top of the sketch:

```cpp
const uint8_t SPEED_LOW  = 102;   // ~40%
const uint8_t SPEED_MED  = 178;   // ~70%
const uint8_t SPEED_HIGH = 255;   // 100%
```

Adjust these values (0–255) to suit your motors and battery voltage.

## 📜 License

MIT — free to use, modify, and share.
