# 🏮 ESP32 LED Lantern

A portable, USB-powered LED lantern built with an ESP32 microcontroller and a WS2812B addressable LED strip. Cycle through vibrant lighting modes with a single button press — perfect for camping, desk ambiance, or just showing off your soldering skills.

![Status](https://img.shields.io/badge/status-active-brightgreen) ![Platform](https://img.shields.io/badge/platform-ESP32-blue) ![LEDs](https://img.shields.io/badge/LEDs-WS2812B-orange) ![License](https://img.shields.io/badge/license-MIT-lightgrey)

---

## 📑 Table of Contents

- [Features](#-features)
- [Components](#-components)
- [Wiring Diagram](#-wiring-diagram)
- [Software Setup](#-software-setup)
- [Uploading the Code](#-uploading-the-code)
- [Usage](#-usage)
- [Troubleshooting](#-troubleshooting)
- [Power Considerations](#-power-considerations)
- [License](#-license)

---

## ✨ Features

- **60 individually addressable RGB LEDs** — full color control on every pixel
- **Single-button mode cycling** — tap to switch between lighting effects
- **USB power bank powered** — fully portable, no wall outlet needed
- **Compact wiring** — only three signal connections required
- **Arduino IDE compatible** — easy to flash and customize

---

## 🧰 Components

| Component | Quantity | Notes |
|---|---|---|
| ESP32 DevKitC | 1 | Any ESP32 dev board with accessible GPIO will work |
| WS2812B LED Strip (5 V) | 1 | 60 LEDs; IP30 (bare) or IP65 (silicone-coated) |
| Momentary Push Button | 1 | Normally open (NO) |
| USB Power Bank | 1 | 5 V output, ≥ 2 A recommended |
| Micro-USB or USB-C Cable | 1 | To power the ESP32 from the power bank |
| USB Breakout / Splitter Cable | 1 | To tap 5 V/GND from USB for the LED strip |
| Dupont / Jumper Wires | Several | Male-to-female or as needed for your enclosure |
| (Optional) 470 Ω Resistor | 1 | Inline on the data line for signal protection |
| (Optional) 1000 µF Capacitor | 1 | Across LED strip 5 V/GND to smooth inrush current |

---

## 🔌 Wiring Diagram

### Connections Overview

```
USB Power Bank (5 V)
  │
  ├──── 5 V ──────┬──────────────────── WS2812B VCC (Red)
  │               │
  │             (ESP32 powered
  │              via its own
  │              USB cable)
  │
  └──── GND ─────┬──────────────────── WS2812B GND (White/Black)
                  │
                  └──────────────────── ESP32 GND pin
                  │
                  └──────────────────── Button Pin 2


ESP32 GPIO 5  ────── (470 Ω) ────────── WS2812B Data In (Green)

ESP32 GPIO 15 ────────────────────────── Button Pin 1
```

### Pin-by-Pin Wiring

#### WS2812B LED Strip → ESP32 & Power

| LED Strip Wire | Connects To | Notes |
|---|---|---|
| **VCC** (Red) | USB 5 V | Powers the LEDs directly from the power bank |
| **GND** (White or Black) | Common GND bus | Shared with ESP32 GND |
| **Data In** (Green) | ESP32 **GPIO 5** | Optional: place a 470 Ω resistor inline for signal integrity |

#### Push Button → ESP32

| Button Pin | Connects To | Notes |
|---|---|---|
| Pin 1 | ESP32 **GPIO 15** | Uses the ESP32's internal pull-up resistor — no external resistor needed |
| Pin 2 | **GND** (common bus) | Pressing the button pulls GPIO 15 LOW |

#### Ground Bus (Critical!)

All three ground references **must** be connected together:

1. **USB power bank GND**
2. **ESP32 GND pin**
3. **WS2812B GND wire**

> ⚠️ **A shared ground is essential.** Without it, the data signal will be unreliable and LEDs may flicker, show wrong colors, or not respond at all.

---

## 💻 Software Setup

### 1. Install Arduino IDE

Download and install the [Arduino IDE](https://www.arduino.cc/en/software) (v2.x recommended).

### 2. Add ESP32 Board Support

1. Open **File → Preferences**.
2. In **Additional Board Manager URLs**, add:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Go to **Tools → Board → Boards Manager**, search for **esp32**, and install **esp32 by Espressif Systems**.

### 3. Select Your Board

- **Tools → Board** → `ESP32 Dev Module`
- **Tools → Port** → Select the COM port your ESP32 appears on

### 4. Install Required Libraries

Open **Sketch → Include Library → Manage Libraries** and install:

| Library | Author | Purpose |
|---|---|---|
| **Adafruit NeoPixel** | Adafruit | Drives the WS2812B LED strip |

> 💡 Alternatively, you can use **FastLED** — just adjust the initialization code accordingly.

---

## 🚀 Uploading the Code

### Example Sketch

Create a new sketch and paste the following starter code:

```cpp
#include <Adafruit_NeoPixel.h>

// ── Pin Definitions ──
#define LED_PIN     5      // GPIO 5  → WS2812B Data In
#define BUTTON_PIN  15     // GPIO 15 → Momentary push button
#define NUM_LEDS    60     // Total LEDs on the strip

// ── NeoPixel Object ──
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// ── State ──
int mode = 0;
const int NUM_MODES = 5;
bool lastButtonState = HIGH;
unsigned long lastDebounce = 0;
const unsigned long DEBOUNCE_MS = 200;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Internal pull-up enabled
  strip.begin();
  strip.setBrightness(80);            // 0-255; keep moderate for battery life
  strip.show();                       // Initialize all pixels to off
}

void loop() {
  checkButton();

  switch (mode) {
    case 0: solidWarmWhite();   break;
    case 1: rainbowCycle();     break;
    case 2: breathingGlow();    break;
    case 3: campfireFlicker();  break;
    case 4: solidOff();         break;
  }
}

// ── Button Handling ──
void checkButton() {
  bool reading = digitalRead(BUTTON_PIN);
  if (reading == LOW && lastButtonState == HIGH
      && (millis() - lastDebounce > DEBOUNCE_MS)) {
    mode = (mode + 1) % NUM_MODES;
    lastDebounce = millis();
  }
  lastButtonState = reading;
}

// ── Mode 0: Warm White ──
void solidWarmWhite() {
  for (int i = 0; i < NUM_LEDS; i++)
    strip.setPixelColor(i, strip.Color(255, 180, 80));
  strip.show();
}

// ── Mode 1: Rainbow Cycle ──
void rainbowCycle() {
  static uint16_t hue = 0;
  for (int i = 0; i < NUM_LEDS; i++)
    strip.setPixelColor(i, strip.ColorHSV(hue + (i * 65536L / NUM_LEDS)));
  strip.show();
  hue += 256;
  delay(20);
}

// ── Mode 2: Breathing Glow ──
void breathingGlow() {
  float val = (exp(sin(millis() / 2000.0 * PI)) - 0.368) * 108.0;
  strip.setBrightness((uint8_t)val);
  for (int i = 0; i < NUM_LEDS; i++)
    strip.setPixelColor(i, strip.Color(255, 140, 60));
  strip.show();
  strip.setBrightness(80);  // Reset default
}

// ── Mode 3: Campfire Flicker ──
void campfireFlicker() {
  for (int i = 0; i < NUM_LEDS; i++) {
    int flicker = random(100, 255);
    strip.setPixelColor(i, strip.Color(flicker, flicker / 3, 0));
  }
  strip.show();
  delay(random(50, 150));
}

// ── Mode 4: Off ──
void solidOff() {
  strip.clear();
  strip.show();
}
```

### Flash It

1. Connect the ESP32 to your computer via USB.
2. Click **Upload** (→) in the Arduino IDE.
3. Wait for `Done uploading` — the lantern is now running!

---

## 🎮 Usage

| Action | Result |
|---|---|
| **Power on** | Plug the USB power bank in. The lantern starts in Warm White mode. |
| **Single press** | Cycles to the next mode: Warm White → Rainbow → Breathing → Campfire → Off → (repeat) |
| **Power off** | Disconnect the USB power bank. |

### Lighting Modes

| # | Mode | Description |
|---|---|---|
| 0 | Warm White | Steady, cozy warm-white glow |
| 1 | Rainbow Cycle | Smooth rainbow that flows across all 60 LEDs |
| 2 | Breathing Glow | Gentle pulsing warm light (inhale / exhale rhythm) |
| 3 | Campfire Flicker | Randomized orange-red flicker simulating a flame |
| 4 | Off | All LEDs off (power-saving standby) |

> 💡 **Tip:** Customize modes by editing the `switch` block in `loop()`. Add your own effects!

---

## 🔧 Troubleshooting

| Symptom | Likely Cause | Fix |
|---|---|---|
| LEDs don't turn on at all | No power to strip | Check USB 5 V → LED VCC connection |
| First LED lights, rest are dark | Bad data connection | Verify GPIO 5 → Data In wiring; try a 470 Ω resistor inline |
| LEDs flicker or show random colors | Missing shared ground | Connect all GND lines together |
| Button doesn't cycle modes | Wrong GPIO or wiring | Confirm button wires: GPIO 15 ↔ Pin 1, GND ↔ Pin 2 |
| Upload fails in Arduino IDE | Wrong board/port selected | Set board to `ESP32 Dev Module`; check COM port |
| LEDs are dim | Brightness set too low | Increase `strip.setBrightness()` value (max 255) |
| Power bank shuts off by itself | Draw too low for bank's auto-off | Increase brightness, or use a power bank without auto-off |

---

## ⚡ Power Considerations

- Each WS2812B LED draws up to **60 mA at full white** (20 mA per channel × 3).
- 60 LEDs at full brightness = **3.6 A max** (theoretical peak).
- In practice, at `brightness(80)` with colorful patterns, expect **0.5–1.2 A** total draw.
- A **2 A USB power bank** is sufficient for all included modes at moderate brightness.
- For extended battery life, keep brightness ≤ 100 and favor single-color modes.

---

## 📁 Project Structure

```
esp32-led-lantern/
├── README.md              ← You are here
├── led_lantern.ino        ← Main Arduino sketch
└── assets/
    └── wiring_diagram.png ← (Optional) Photo or Fritzing diagram
```

---

## 📝 License

This project is open source under the [MIT License](https://opensource.org/licenses/MIT). Build it, mod it, share it.

---

*Built with an ESP32, a strip of WS2812B LEDs, and a love for glowing things.* 🏮

This README is AI generated because I just had a massive LA paragraph, so I'm braindead, and don't feel like doing this. Thank Copilot for the README, and Me for the .ino code :)