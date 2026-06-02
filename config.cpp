#include "config.h"

int Config::level = 1;
int Config::exp = 0;
int Config::exp_needed = 100; 

int Config::str = 1;
int Config::def = 1;
int Config::spd = 1;
int Config::dex = 1;

int Config::wifi_count = 0;
int Config::ble_count = 0;

// Initialize Gem Trackers
int Config::total_gems = 0;
String Config::current_gem = "None";

std::set<String> Config::seen_macs;
std::set<String> Config::seen_ble_macs;

std::string Config::last_seen_name = "None";
std::string Config::last_seen_mac = "00:00:00:00:00:00";

SPIClass Config::sdSPI(HSPI);

void Config::initSD() {
  sdSPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, sdSPI)) {
    USBSerial.println("SD Card Mount Failed!");
    return;
  }
  loadStats(); 
}

void Config::saveStats() {
  File file = SD.open("/siglet_save.txt", FILE_WRITE);
  if(!file) return;
  file.println(level);
  file.println(exp);
  file.println(exp_needed);
  file.println(str);
  file.println(def);
  file.println(spd);
  file.println(dex);
  file.println(wifi_count);
  file.println(ble_count);
  file.println(total_gems);
  file.println(current_gem);
  file.close();
}

void Config::loadStats() {
  File file = SD.open("/Thumbagotchi_save.txt"); //change "Thumbagotchi" to device name.
  if(!file) return; 
  
  level = file.readStringUntil('\n').toInt();
  exp = file.readStringUntil('\n').toInt();
  exp_needed = file.readStringUntil('\n').toInt();
  str = file.readStringUntil('\n').toInt();
  def = file.readStringUntil('\n').toInt();
  spd = file.readStringUntil('\n').toInt();
  dex = file.readStringUntil('\n').toInt();
  wifi_count = file.readStringUntil('\n').toInt();
  ble_count = file.readStringUntil('\n').toInt(); 
  
  total_gems = file.readStringUntil('\n').toInt();
  current_gem = file.readStringUntil('\n');
  current_gem.trim();
  
  file.close();
}

void Config::logNetwork(String ssid, String mac) {
  File file = SD.open("/wifi_log.txt", FILE_APPEND);
  if(!file) return;
  file.print("MAC: "); file.print(mac);
  file.print(" | SSID: "); file.println(ssid);
  file.close();
}

void Config::logBleDevice(String name, String mac) {
  File file = SD.open("/ble_log.txt", FILE_APPEND);
  if(!file) return;
  file.print("MAC: "); file.print(mac);
  file.print(" | NAME: "); file.println(name);
  file.close();
}

void Config::awardWiFiEXP(int newNetworksFound) {
  for(int i = 0; i < newNetworksFound; i++) {
    exp += 15; 
    wifi_count++;
    if (wifi_count % 10 == 0) { str++; spd++; }
  }
  checkLevelUp();
  saveStats(); 
}

void Config::awardBleEXP(int newBleFound) {
  for(int i = 0; i < newBleFound; i++) {
    exp += 10; 
    ble_count++;
    if (ble_count % 20 == 0) { def++; dex++; }
  }
  checkLevelUp();
  saveStats();
}

void Config::checkLevelUp() {
  if (exp >= exp_needed) {
    exp = exp - exp_needed; 
    level++;
    exp_needed = level * 100; 
    str++; def++; spd++; dex++;
  }
}

// --- Gem Stone LOGIC ---

String Config::checkRareMAC(String mac) {
  String oui = mac.substring(0, 8);
  oui.toUpperCase(); // Ensure it matches the database exactly
  
  // Tactical & Drones
  if (oui == "00:25:DF" || oui == "E0:89:9D") return "Axon Device";
  if (oui == "60:60:1F" || oui == "8C:85:90") return "DJI Drone";
  
  // Tech & Gaming
  if (oui == "B8:27:EB" || oui == "DC:A6:32") return "Raspberry Pi";
  if (oui == "98:B6:E9" || oui == "9C:E6:E5") return "Nintendo";
  
  return ""; // Not a rare MAC
}

String Config::generateGem() {
  const char* gems[] = {"Ruby", "Sapphire", "Emerald", "Onyx", "Quartz", "Topaz", "Obsidian"};
  const char* suffixes[] = {"of Fear", "of Power", "of Shadows", "of Logic", "of Stealth", "of Signals", "of The Void"};
  
  int gIndex = random(0, 7);
  int sIndex = random(0, 7);
  
  return String(gems[gIndex]) + " " + String(suffixes[sIndex]);
}