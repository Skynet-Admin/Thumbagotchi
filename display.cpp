#include "display.h"

Adafruit_ST7789 Display::tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

int Display::currentPage = 1;
bool Display::forceRedraw = true;

String Display::currentFace = "";
String Display::currentStatus = "";

void Display::startScreen() {
  pinMode(TFT_BL, OUTPUT);
  toggleScreen(true);

  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS); 
  tft.init(172, 320); 
  tft.setRotation(2); 
  tft.fillScreen(ST77XX_BLACK);
}

void Display::toggleScreen(bool isOn) {
  digitalWrite(TFT_BL, isOn ? HIGH : LOW);
}

void Display::nextPage() {
  currentPage++;
  if (currentPage > 3) currentPage = 1; 
  
  forceRedraw = true;

  if (currentPage == 3) {
    toggleScreen(false); // STEALTH MODE
  } else {
    toggleScreen(true);  
    tft.fillScreen(ST77XX_BLACK); 
    
    if (currentPage == 2) updatePage2();
    if (currentPage == 1) updateUI(currentFace, currentStatus);
  }
}

void Display::updatePage2() {
  if (currentPage != 2) return;

  tft.fillScreen(ST77XX_BLACK);
  
  // Header
  tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setCursor(5, 10);
  tft.println("- DATA BANK -");
  tft.drawLine(0, 35, 172, 35, ST77XX_WHITE);

  // Network Totals
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 50);
  tft.print("Wi-Fi: "); tft.println(Config::wifi_count);
  tft.setCursor(10, 70);
  tft.print("BLE:   "); tft.println(Config::ble_count);
  
  tft.drawLine(0, 95, 172, 95, ST77XX_WHITE);

  // RPG Level & EXP
  tft.setTextColor(ST77XX_CYAN, ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 105);
  tft.print("LVL: "); tft.println(Config::level);
  
  tft.setTextSize(1);
  tft.setCursor(10, 125);
  tft.print("EXP: "); tft.print(Config::exp); tft.print(" / "); tft.println(Config::exp_needed);

  tft.drawLine(0, 140, 172, 140, ST77XX_WHITE);

  // --- GEM SECTION ---
  tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 150);
  tft.print("TOTAL GEMS: "); tft.println(Config::total_gems);
  tft.setCursor(10, 165);
  tft.println("LATEST GEM:");
  tft.setCursor(10, 180);
  tft.println(Config::current_gem);

  tft.drawLine(0, 195, 172, 195, ST77XX_WHITE);

  // RPG Base Stats
  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 205);
  tft.print("STR: "); tft.println(Config::str);
  tft.setCursor(10, 230);
  tft.print("DEF: "); tft.println(Config::def);
  tft.setCursor(90, 205);
  tft.print("SPD: "); tft.println(Config::spd);
  tft.setCursor(90, 230);
  tft.print("DEX: "); tft.println(Config::dex);

  forceRedraw = false;
}

void Display::updateUI(String face, String statusText) {
  // Check if things changed
  bool faceChanged = (face != currentFace);
  bool statusChanged = (statusText != currentStatus);

  // Update background trackers
  currentFace = face;
  currentStatus = statusText;

  // Only draw if we are on Page 1
  if (currentPage != 1) return;

  if (faceChanged || statusChanged || forceRedraw) {
    forceRedraw = false;
    tft.fillScreen(ST77XX_BLACK); 
    
    // Header
    tft.fillRect(0, 0, 172, 45, ST77XX_BLACK);
    tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
    tft.setTextSize(2);
    tft.setCursor(5, 5);
    tft.print("SIGLET  LVL:");
    tft.println(Config::level);
    
    // EXP Bar
    tft.drawRect(5, 27, 162, 12, ST77XX_WHITE);
    if (Config::exp_needed > 0) { 
      int fillWidth = (Config::exp * 160) / Config::exp_needed;
      if (fillWidth > 160) fillWidth = 160; 
      tft.fillRect(6, 28, fillWidth, 10, ST77XX_GREEN);
    }
    tft.drawLine(0, 45, 172, 45, ST77XX_WHITE);

    // Face & Status
    tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
    tft.setTextSize(4);
    int faceX = (172 - (face.length() * 24)) / 2; 
    if (faceX < 0) faceX = 0;
    tft.setCursor(faceX, 90);
    tft.println(face);

    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setTextSize(1);
    tft.setCursor(5, 150);
    tft.print("Last: ");
    tft.println(statusText);
    tft.drawLine(0, 220, 172, 220, ST77XX_WHITE);

    // Bottom Stats
    tft.setTextColor(ST77XX_CYAN, ST77XX_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 240);
    tft.print("STR:"); tft.println(Config::str);
    tft.setCursor(10, 280);
    tft.print("DEF:"); tft.println(Config::def);
    tft.setCursor(90, 240);
    tft.print("SPD:"); tft.println(Config::spd);
    tft.setCursor(90, 280);
    tft.print("DEX:"); tft.println(Config::dex);
  }
}