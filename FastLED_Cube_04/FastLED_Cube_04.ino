#include "MPU6050.h"
#include "FastLED.h"
#include "Cube.h"

Cube Cube;

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

#define LED_PIN     13
#define NUM_LEDS    384
#define BRIGHTNESS  20
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

int voxel[3] = {9,4,4};
int dVal = 100;

void Move(int ord1, int ord2, int dist1, int dist2) {
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
  // Serial.begin(38400);
}

void loop() {
  int aVal = 10000;
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  int accels[3];
  accels[0] = ax;
  accels[1] = ay;
  accels[2] = az;
  for (int i = 0; i <= 2; i++) {
    for (int j = 0; j <=2; j++) {
      if ((i != j) && (accels[i] > aVal) && (voxel[i] > 1) && ((voxel[j] == 0) || (voxel[j] == 9))) {
        Move(i,0,-1,0);
      }
      else if ((i != j) && (accels[i] < -aVal) && (voxel[i] < 8) && ((voxel[j] == 0) || (voxel[j] == 9))) {
        Move(i,0,1,0);
      }
      else if ((i != j) && (accels[i] > aVal) && (voxel[i] == 1) && (voxel[j] == 0)) {
        Move(i,j,-1,1);
      }
      else if ((i != j) && (accels[i] > aVal) && (voxel[i] == 1) && (voxel[j] == 9)) {
        Move(i,j,-1,-1);
      }
      else if ((i != j) && (accels[i] < -aVal) && (voxel[i] == 8) && (voxel[j] == 0)) {
        Move(i,j,1,1);
      }
      else if ((i != j) && (accels[i] < -aVal) && (voxel[i] == 8) && (voxel[j] == 9)) {
        Move(i,j,1,-1);
      }
    }
  }
}