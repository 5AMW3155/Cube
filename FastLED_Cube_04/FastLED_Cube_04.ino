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

// int rotate (int arr[8][8], int num) //This function is used to rotate arrays by n turns. Used in deVoxel function
// {
//   for (int k = 0; k < num; k++)
//   {
//     for (int i = 0; i < 8 / 2; i++)
//       {
// 	for (int j = i; j < 8 - i - 1; j++)
// 	  {
// 	    int temp = arr[i][j];
// 	    arr[i][j] = arr[8 - 1 - j][i];
// 	    arr[8 - 1 - j][i] = arr[8 - 1 - i][8 - 1 - j];
// 	    arr[8 - 1 - i][8 - 1 - j] = arr[j][8 - 1 - i];
// 	    arr[j][8 - 1 - i] = temp;
// 	  }
//       }
//     }
//   return arr[8][8];
// };

// int deVoxel (int temp[3]) //This function is used to turn XYZ coordinates into serial pixel addresses.
// {
//   byte x = temp[0];
//   byte y = temp[1];
//   byte z = temp[2];
//   int pix;
//   int arr[8][8] = {
//     {0, 1, 2, 3, 4, 5, 6, 7},
//     {8, 9, 10, 11, 12, 13, 14, 15},
//     {16, 17, 18, 19, 20, 21, 22, 23},
//     {24, 25, 26, 27, 28, 29, 30, 31},
//     {32, 33, 34, 35, 36, 37, 38, 39},
//     {40, 41, 42, 43, 44, 45, 46, 47},
//     {48, 49, 50, 51, 52, 53, 54, 55},
//     {56, 57, 58, 59, 60, 61, 62, 63}
//   };
//   if (x == 9) //Fixed
//     {
//       arr[8][8] = rotate(arr,3);
//       pix = arr[y-1][z-1] + 0*64;
//     };
//   if (x == 0) //Fixed
//     {
//       arr[8][8] = rotate(arr,3);
//       pix = arr[z-1][y-1] + 3*64;
//     };
//   if (y == 9) //Fixed
//     {
//       // arr[8][8] = rotate(arr,0);
//       pix = arr[z-1][x-1] + 5*64;
//     };
//   if (y == 0) //Fixed
//     {
//       arr[8][8] = rotate(arr,2);
//       pix = arr[x-1][z-1] + 2*64;
//     };
//   if (z == 0) //Fixed
//     {
//       // arr[8][8] = rotate(arr,2);
//       pix = arr[y-1][x-1] + 4*64;
//     };
//   if (z == 9) //Fixed
//     {
//       arr[8][8] = rotate(arr,2);
//       pix = arr[x-1][y-1] + 1*64;
//     }
//   return pix;
// };

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