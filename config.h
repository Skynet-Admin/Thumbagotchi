#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <string>
#include <set>
#include <SPI.h>
#include <SD.h>
#include <FS.h>

#define SD_MOSI 15
#define SD_MISO 16
#define SD_SCK  14
#define SD_CS   21

class Config {
public:
  static int level;
  static int exp;
  static int exp_needed;
  
  static int str;
  static int def;
  static int spd;
  static int dex;

  static int wifi_count;
  static int ble_count;

  // Gem Stone Loot Tracker for Rare MAC Finds. How Fun!
  static int total_gems;
  static String current_gem;

  static std::set<String> seen_macs;
  static std::set<String> seen_ble_macs; 

  static std::string last_seen_name;
  static std::string last_seen_mac;

  static SPIClass sdSPI;

  static void initSD();
  static void saveStats();
  static void loadStats();
  static void logNetwork(String ssid, String mac);
  static void logBleDevice(String name, String mac); 
  
  static void awardWiFiEXP(int newNetworksFound);
  static void awardBleEXP(int newBleFound); 
  static void checkLevelUp();

  // Loot Functions
  static String checkRareMAC(String mac);
  static String generateGem();
};

#endif // CONFIG_H