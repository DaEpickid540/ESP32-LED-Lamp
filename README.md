# ESP32 LED Lamp

An ESP32 firmware for controlling a WS2812B LED strip with a single push button. Press the button to cycle through 5 lighting modes.

## Modes

| # | Mode | Description |
|---|------|-------------|
| 0 | Solid White | Full strip, warm white |
| 1 | Rainbow Cycle | Scrolling hue wheel across all LEDs |
| 2 | Color Wipe | Wipes Red → Green → Blue one LED at a time |
| 3 | Breathing Pulse | Blue fade in/out (breathing effect) |
| 4 | Off | Strip off |

## Hardware

- ESP32 dev board
- WS2812B LED strip (default: 60 LEDs)
- Momentary push button
- 5V power supply rated for your strip length

## Wiring

| Component | ESP32 Pin |
|-----------|-----------|
| LED strip DIN | GPIO 5 |
| Button | GPIO 4 (uses internal pull-up) |
| LED strip 5V | External 5V |
| GND | Common GND |

## Dependencies

- [FastLED](https://github.com/FastLED/FastLED) library

Install via Arduino Library Manager: search **FastLED**.

## Configuration

Edit the defines at the top of `esp32_led_button.ino`:

```cpp
#define LED_PIN     5      // Data pin
#define BUTTON_PIN  4      // Button pin
#define NUM_LEDS    60     // Strip length
#define BRIGHTNESS  150    // 0–255
```

## Upload

1. Open `esp32_led_button.ino` in Arduino IDE
2. Select your ESP32 board under **Tools → Board**
3. Install FastLED if not already installed
4. Upload
