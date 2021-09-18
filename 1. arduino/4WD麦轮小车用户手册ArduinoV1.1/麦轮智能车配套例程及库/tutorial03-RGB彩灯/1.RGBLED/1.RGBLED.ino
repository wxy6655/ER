/*
  RGB LED - 彩色LED

  This example code is in the public domain.
  www.yfrobot.com
*/

#include <Adafruit_NeoPixel.h>

#define PIN 13
#define NUMPIXELS 6
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.setBrightness(50);
}

void loop()
{
  // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
  for (int i = 0; i < 6; i++) {
    pixels.setPixelColor(i, pixels.Color(150, 0, 0));
  }
  pixels.show();   // Send the updated pixel colors to the hardware.
  delay(500);
  for (int i = 0; i < 6; i++)
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
  pixels.show();   // Send the updated pixel colors to the hardware.
  delay(500);
  for (int i = 0; i < 6; i++)
    pixels.setPixelColor(i, pixels.Color(0, 0, 150));
  pixels.show();   // Send the updated pixel colors to the hardware.
  delay(500);
}
