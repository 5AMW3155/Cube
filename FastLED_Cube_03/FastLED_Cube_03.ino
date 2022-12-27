#include "MPU6050.h"
#include "FastLED.h"

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;
byte counter;

#define LED_PIN     13
#define NUM_LEDS    384
#define BRIGHTNESS  20
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

void setColorRGB(byte r, byte g, byte b) {
  CRGB color = CRGB(r, g, b);
  fill_solid(leds, NUM_LEDS, color);
  FastLED.show();
}

void setup() {
  delay(2000); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  set_max_power_in_volts_and_milliamps(5,5600);
  Wire.begin();
  accelgyro.initialize();
  FastLED.show();
}

void loop() {
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  ax = abs(ax/125);
  ay = abs(ay/125);
  az = abs(az/125);
  gx = abs(gx);
  gy = abs(gy);
  gz = abs(gz);
  int check = gx + gy + gz;

  byte r = ax;
  byte g = ay;
  byte b = az;

  if (check > 800) {
    setColorRGB(r,g,b);
    counter = 0;
  }
  else if (counter == 100) {
    fadeToBlackBy(leds, NUM_LEDS, 150);
    FastLED.show();
  }
  else {
    counter++;
  }
  delay(50);
}
