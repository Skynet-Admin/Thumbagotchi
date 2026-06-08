# 👾 Thumbagotchi

**LEGAL NOTICE:**

Although this device is purely passive in nature, and has been created in a way that -in theory- stays within all laws and regulations; it is not possible to adhere to EVERY regulation and law in real time, over different juristdictions and thus:
This device is intended to be used for educaional purposes only in closed lab setups, or on networks where the admistrators or owners of the network have given EXPLICIT and RECORDED access for use.
The creator of this "Repo" does not hold any responsibility for the end use of the devices mentioned, software created or third party affiliated to this project, or any deviations to this "Repo" that may create a more-than-passive device as an end-state (such as if a future entity expands the code or alters it in order to become any other end-state than the Creator of this Repo intended).

---

**The Passive War-Driving Digital Pet**

Thumbagotchi is an open-source, ESP32-based digital pet that feeds on local Wi-Fi and Bluetooth Low Energy (BLE) signals. It acts as a passive RF scanner (wardriving tool) wrapped in a gamified, Tamagotchi-style interface.
With its small form-factor, 8-Bit style Emotes, RPG style Stats and Loot system; this Device will be your little RF buddy!

This Project was inspired by the popular "PWNagotchi" Project from Jayofelony (REPO HERE: https://github.com/jayofelony/pwnagotchi )

Just plug it into a battery bank, drop it in your pocket, and take it for a walk. As it discovers new access points and smart devices, it earns EXP, levels up, generates RNG loot, and logs everything safely to a Micro-SD card. 

**Zero transmissions. 100% passive sniffing.**

---

### ✨ Features
* 📡 **Passive Sniffing:** Stealthily maps 2.4GHz Wi-Fi beacons and BLE advertisements without ever connecting or broadcasting.
* 📈 **RPG Mechanics:** Earns EXP for every new network found. Leveling up requires exponentially more EXP and boosts your base stats (STR, DEF, SPD, DEX).
* 🧠 **Smart Memory:** Maintains internal MAC address dictionaries to prevent duplicate "farming" of the same networks.
* 💎 **Loot Drops:** Built-in OUI lookup table! Discovering rare MAC addresses (like Drones, Nintendo hardware, or Raspberry Pis) triggers an RNG "Gem Stone" loot drop.
* 💾 **Persistent Data:** Auto-saves your pet's stats and logs every discovered SSID/MAC address to text files on a Micro-SD card.
* 🔋 **Interactive Menu:** Uses hardware interrupts to cycle through three UI pages: **Main Face**, **Deep Stats**, and a **Stealth Mode** (Screen Off) to save battery.

---

### 🛠️ Hardware Requirements
* **Waveshare ESP32-S3 1.47" LCD Board** (Can be adapted to other ESP32 boards by modifying `display.h` pins).
* **Micro-SD Card** (Formatted to FAT32).
* **Portable USB Battery Bank.**

---

### 💻 Software Setup

1. Clone this repository to your local machine.
2. Open the project in the **Arduino IDE** (or Arduino Cloud IDE).
3. Ensure your Board Manager is set to **ESP32S3 Dev Module**.
4. Install the following libraries via the Library Manager:
   * `ArduinoBLE` (by Arduino)
   * `Adafruit GFX Library`
   * `Adafruit ST7735 and ST7789 Library`
5. *(If using the Waveshare board)* Ensure the ESP32 USB mode is set to "Hardware CDC and JTAG" so Native USB works properly. 
6. Flash the code, insert the SD card, and press the `RST` button to boot Thumbagotchi OS!

---

### 🎮 How to Play

1. **Boot Up:** Upon powering on, Thumbagotchi will mount the SD card and load your previous save state. 
2. **Scan Phase:** The face will change to `(0_0)` as it sweeps for Wi-Fi, and `(o_o)` for Bluetooth. 
3. **Discovery:** If it finds a network it hasn't seen before, the face changes to `(^o^)` and displays the SSID. You earn EXP!
4. **The Menu:** Press the physical **BOOT** button on the device to switch pages:
   * **Page 1:** The Face, Current Target, and EXP Bar.
   * **Page 2 (Data Bank):** Total RF counts, RPG Stats, and Loot Inventory.
   * **Page 3 (Stealth Mode):** Turns the TFT backlight off entirely. The device will continue to scan, log, and level up in your pocket while saving massive amounts of battery. 

---

### 📂 SD Card File Structure
If you pop the Micro-SD card into your computer, you will find three files:
* `thumbagotchi_save.txt` - The system save state (Do not edit manually, or you may corrupt your pet!)
* `wifi_log.txt` - A chronological log of every unique Wi-Fi MAC and SSID encountered.
* `ble_log.txt` - A chronological log of every unique Bluetooth device encountered.

---

**Known Issues**
1. Heat - The Device will heat up quiet alot, current work around is short powered on times.
2. LVL Roll Over - Once the Device LVL reaches "9" it rolls over to "1", although the actual level will be displayed in the STATS page.
3. Limited LOOT - This is being expanded on and further LOOT mechanics are being developed in the background, as right now, collection of GEM STONES is arbitrary.

---

### 📜 License
This project is open-source and free to use, modify, and distribute. 

*Happy Hunting!* 📡👾

**Any updates, issues, ideas, or advice, please reach out and we can discuss implimentation**

- Skynet_Static
- B@5710N
