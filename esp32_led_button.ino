#include <FastLED.h>

// ─── PIN CONFIG ──────────────────────────────────────────────
#define LED_PIN       5      // Data pin → WS2812B DIN
#define BUTTON_PIN    4      // Button pin (use INPUT_PULLUP)
#define NUM_LEDS      60     // Change to your strip length
#define LED_TYPE      WS2812B
#define COLOR_ORDER   GRB
#define BRIGHTNESS    150    // 0–255
// ─────────────────────────────────────────────────────────────

CRGB leds[NUM_LEDS];

// Button debounce
int        lastButtonState  = HIGH;
int        buttonState      = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_DELAY = 50;

// Mode tracking
int   currentMode   = 0;
const int NUM_MODES = 5;

// Hue tracker for rainbow
uint8_t hue = 0;

// ─── SETUP ───────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.println("ESP32 LED Strip Ready!");
  Serial.println("Press button to cycle modes.");
}

// ─── MAIN LOOP ───────────────────────────────────────────────
void loop() {
  handleButton();
  runCurrentMode();
  FastLED.show();
  delay(20);
}

// ─── BUTTON HANDLER ──────────────────────────────────────────
void handleButton() {
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != buttonState) {
      buttonState = reading;

      // Trigger on release (LOW → HIGH with INPUT_PULLUP)
      if (buttonState == HIGH) {
        currentMode = (currentMode + 1) % NUM_MODES;
        FastLED.clear();
        Serial.print("Mode → ");
        Serial.println(currentMode);
      }
    }
  }

  lastButtonState = reading;
}

// ─── MODE DISPATCHER ─────────────────────────────────────────
void runCurrentMode() {
  switch (currentMode) {
    case 0: modeSolidWhite();   break;
    case 1: modeRainbow();      break;
    case 2: modeColorWipe();    break;
    case 3: modePulse();        break;
    case 4: modeOff();          break;
  }
}

// ─── MODE 0: SOLID WHITE ─────────────────────────────────────
void modeSolidWhite() {
  fill_solid(leds, NUM_LEDS, CRGB::White);
}

// ─── MODE 1: RAINBOW CYCLE ───────────────────────────────────
void modeRainbow() {
  fill_rainbow(leds, NUM_LEDS, hue, 7);
  hue++;
}

// ─── MODE 2: COLOR WIPE (Red → Green → Blue) ─────────────────
void modeColorWipe() {
  static int   pos         = 0;
  static int   colorIndex  = 0;
  static CRGB  colors[]    = { CRGB::Red, CRGB::Green, CRGB::Blue };

  leds[pos] = colors[colorIndex];
  pos++;

  if (pos >= NUM_LEDS) {
    pos = 0;
    colorIndex = (colorIndex + 1) % 3;
    FastLED.clear();
  }
}

// ─── MODE 3: BREATHING PULSE (blue) ──────────────────────────
void modePulse() {
  static uint8_t val  = 0;
  static int8_t  dir  = 1;

  fill_solid(leds, NUM_LEDS, CHSV(160, 255, val));  // Blue hue
  val += dir * 3;
  if (val >= 255 || val <= 0) dir = -dir;
}

// ─── MODE 4: OFF ─────────────────────────────────────────────
void modeOff() {
  FastLED.clear();
}