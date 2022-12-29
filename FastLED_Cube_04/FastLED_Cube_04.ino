#include "MPU6050.h"
#include "FastLED.h"
#include "Cube.h"

#define LED_PIN     13
#define NUM_LEDS    384
#define BRIGHTNESS  20
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS]; // Initilize LED arrays
Cube Cube; // Initilize Cube lib
MPU6050 accelgyro; // Initilize IMU

int voxel[3] = {4,4,9};
int dVal = 100;

// void Maze() {
//   int wall[8][3] = {{1,1,9},{2,1,9},{3,1,9},{4,1,9},{5,1,9},{6,1,9},{7,1,9},{8,1,9}};
//   for (byte i = 0; i <= 7; i++) {
//     int pix = Cube.deVoxel(wall[i]);
//     leds[pix] = CRGB::Green;
//   }
//   FastLED.show();
//   return(0);
// };

void Move(int ord1, int ord2, int dist1, int dist2) {
  int pix = Cube.deVoxel(voxel);
  voxel[ord1] = voxel[ord1] + dist1;
  voxel[ord2] = voxel[ord2] + dist2;
  int new_pix = Cube.deVoxel(voxel);
  if (leds[new_pix].getAverageLight() != 0) {
    voxel[ord1] = voxel[ord1] - dist1;
    voxel[ord2] = voxel[ord2] - dist2;
  }
  else {
    leds[pix] = CRGB::Black;
    leds[new_pix] = CRGB::White;
    FastLED.show();
    delay(dVal);
  }
  return(0);
}

void setup() {
  delay(2000); // power-up safety delay
  FastLED.addLeds<WS2812, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  set_max_power_in_volts_and_milliamps(5,5600);
  // Maze();
  Wire.begin();
  accelgyro.initialize();
  int pix = Cube.deVoxel(voxel);
  leds[pix] = CRGB::White;
  FastLED.show();
  // Serial.begin(38400);
}

void loop() {
  int aVal = 8000;
  int16_t accels[3];
  accels[0] = accelgyro.getAccelerationX();
  accels[1] = accelgyro.getAccelerationY();
  accels[2] = accelgyro.getAccelerationZ();
  for (byte i = 0; i <= 2; i++) {
    for (byte j = 0; j <=2; j++) {
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