/*
  Cube.h - Library for controlling LED's on an 8x8x8 cube of WS2812b's
  Created by Samuel T. Weiss, December 26, 2022.
  Rleased into the public domain.  
*/
#ifndef Cube_h
#define Cube_h

#include "Arduino.h"

class Cube
{
  public:
    Cube();
    int rotate(int arr[8][8], int num);
    int deVoxel(int temp[3]);
	void faceMove(int ord, int dist);
	void edgeMove(int ord1, int ord2, int dist1, int dist2);
  private:
};

#endif