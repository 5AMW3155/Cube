#include "MPU6050.h"
#include "FastLED.h"
#include "Cube.h"

Cube Cube;

MPU6050 accelgyro;

int16_t ax, ay, az;

#define LED_PIN     13
#define NUM_LEDS    384
#define BRIGHTNESS  10
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

// uint8_t n = 8;
int voxel[3] = {0,4,4};
int dVal = 100;

void pixelMove(int ord1, int ord2, int dist1, int dist2) {
  int pix = Cube.deVoxel(voxel);
  leds[pix] = CRGB::Black;
  voxel[ord1] = voxel[ord1] + dist1;
  voxel[ord2] = voxel[ord2] + dist2;
  pix = Cube.deVoxel(voxel);
  leds[pix] = CRGB::White;
  FastLED.show();
  delay(dVal);
}

void setup() {
  delay(2000); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  set_max_power_in_volts_and_milliamps(5,5600);
  Wire.begin();
  accelgyro.initialize();
  int pix = Cube.deVoxel(voxel);
  leds[pix] = CRGB::White;
  FastLED.show();
}

void loop() {
  int aVal = 2000;
  accelgyro.getAcceleration(&ax, &ay, &az);
}
