#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include "config.h"

// CORRECT Waveshare 1.47" ESP32-S3 Pins
#define TFT_MOSI 45
#define TFT_SCLK 40
#define TFT_CS   42
#define TFT_DC   41
#define TFT_RST  39
#define TFT_BL   48 

class Display {
public:
  static int currentPage; // Tracks the menu page (1, 2, or 3)
  
  static void startScreen();
  static void updateUI(String face, String statusText);
  static void updatePage2(); // Deep Stats Page
  
  static void toggleScreen(bool isOn);
  static void nextPage();

private:
  static Adafruit_ST7789 tft;
  static String currentFace;
  static String currentStatus;
  static bool forceRedraw; // Tells the screen to refresh when changing pages
};

#endif // DISPLAY_H