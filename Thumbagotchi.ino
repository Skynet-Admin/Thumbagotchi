#include "arduino_secrets.h"

/*
 * this is my first project I hope you enjoy it :)
 With PWNagotchi not working in AUS due to the "BOTNET" classification, this is my device to mimic it's intention but in more of a War-Driver way.
 *
 * Inspired by jayofelony's initial build. repo found here:
 https://github.com/jayofelony
 *
 * This build - All Credit to Skynet and B@5710N
 *
 * Hardware: Waveshare ESP32-S3 1.47" LCD, 32Gb Micro-SD Card (Kodak)
 */

#include <WiFi.h>
#include <ArduinoBLE.h> 
#include "config.h"
#include "display.h"

#define BOOT_BTN 0 // The GPIO pin for the BOOT button

volatile bool buttonPressed = false;

void IRAM_ATTR isrButton() {
  buttonPressed = true;
}

void smartDelay(int ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    if (buttonPressed) {
      buttonPressed = false;
      Display::nextPage(); 
      delay(200); 
    }
    delay(10); 
  }
}

void setup() {
  USBSerial.begin(115200);
  delay(2000); 
  
  USBSerial.println("Booting Thumbagotchi OS..."); //Change "Thumbagotchi" to your device name.

  pinMode(BOOT_BTN, INPUT_PULLUP);
  attachInterrupt(BOOT_BTN, isrButton, FALLING);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  if (!BLE.begin()) {
    USBSerial.println("Starting BLE failed!");
  }

  Config::initSD();

  Display::startScreen();
  Display::updateUI("(^o^)", "Waking up...");
  smartDelay(2000);
}

void loop() {
  if (buttonPressed) {
    buttonPressed = false;
    Display::nextPage();
  }

  // ==========================================
  // PHASE 1: WI-FI SCANNING
  // ==========================================
  Display::updateUI("(0_0)", "Scanning WiFi...");
  BLE.stopScan(); 
  
  int wifiCount = WiFi.scanNetworks();

  if (wifiCount > 0) {
    int newWifiFound = 0;
    String topWifiName = "";

    for (int i = 0; i < wifiCount; i++) {
      String currentMAC = WiFi.BSSIDstr(i); 
      String currentSSID = WiFi.SSID(i);
      if (currentSSID == "") currentSSID = "<Hidden>";

      if (Config::seen_macs.find(currentMAC) == Config::seen_macs.end()) {
        Config::seen_macs.insert(currentMAC);
        newWifiFound++;
        Config::logNetwork(currentSSID, currentMAC);
        if (topWifiName == "") topWifiName = currentSSID; 
        
        // --- RARE MAC CHECK ---
        String rareTarget = Config::checkRareMAC(currentMAC);
        if (rareTarget != "") {
          Config::total_gems++;
          Config::current_gem = Config::generateGem();
          Config::saveStats();
          
          Display::updateUI("(*O*)", rareTarget + " ID'd!");
          smartDelay(2500);
          Display::updateUI("(^_-)", "Got: " + Config::current_gem);
          smartDelay(2500);
        }
      }
    }

    if (newWifiFound > 0) {
      Config::awardWiFiEXP(newWifiFound);
      Display::updateUI("(^o^)", topWifiName);
      smartDelay(2000); 
    }
  }
  WiFi.scanDelete(); 

  // ==========================================
  // PHASE 2: BLUETOOTH LE SCANNING
  // ==========================================
  Display::updateUI("(o_o)", "Sniffing BLE...");
  BLE.scan();
  
  int newBleFound = 0;
  String topBleName = "";
  unsigned long startBleScan = millis();

  while (millis() - startBleScan < 3000) {
    if (buttonPressed) {
      buttonPressed = false;
      Display::nextPage();
    }

    BLEDevice peripheral = BLE.available();
    if (peripheral) {
      String currentMAC = peripheral.address();
      currentMAC.toUpperCase(); // Ensure uppercase for check
      
      String currentName = peripheral.hasLocalName() ? peripheral.localName() : "<Unknown BLE>";

      if (Config::seen_ble_macs.find(currentMAC) == Config::seen_ble_macs.end()) {
        Config::seen_ble_macs.insert(currentMAC);
        newBleFound++;
        Config::logBleDevice(currentName, currentMAC);
        if (topBleName == "") topBleName = currentName;
        
        // --- RARE MAC CHECK ---
        String rareTarget = Config::checkRareMAC(currentMAC);
        if (rareTarget != "") {
          Config::total_gems++;
          Config::current_gem = Config::generateGem();
          Config::saveStats();
          
          Display::updateUI("(*O*)", rareTarget + " ID'd!");
          smartDelay(2500);
          Display::updateUI("(^_-)", "Got: " + Config::current_gem);
          smartDelay(2500);
        }
      }
    }
  }
  
  BLE.stopScan(); 

  if (newBleFound > 0) {
    Config::awardBleEXP(newBleFound);
    Display::updateUI("(*_*)", topBleName); 
    smartDelay(2000);
  } else {
    Display::updateUI("(-_-)", "Nothing new...");
  }
  
  // ==========================================
  // PHASE 3: REST
  // ==========================================
  Display::updateUI("(-_-)", "Resting...");
  smartDelay(3000); 
}