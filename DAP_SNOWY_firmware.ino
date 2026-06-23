
/*
 * MINI DAP SNOWY - Firmware v1.0
 * ESP32-C3 based Digital Audio Player
 * 
 * Pin Assignment:
 *   OLED  → SCL: IO4,  SDA: IO5
 *   SD    → MOSI: IO7, MISO: IO2, SCK: IO6, CS: IO10
 *   DAC   → DIN: IO8,  BCK: IO9,  LCK: IO18
 *   BTN   → SW1 (Play/Pause): IO3, SW2 (Next): IO1, SW3 (Prev): IO0
 * 
 * Libraries needed (install via Arduino Library Manager):
 *   - ESP8266Audio by Earle F. Philhower
 *   - U8g2 by oliver
 *   - SD (built-in ESP32)
 */

#include "Arduino.h"
#include "SD.h"
#include "SPI.h"
#include "Wire.h"
#include "U8g2lib.h"
#include "AudioFileSourceSD.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

// ─── PIN DEFINITIONS ──────────────────────────────────────────
#define I2C_SCL     4
#define I2C_SDA     5

#define SD_MOSI     7
#define SD_MISO     2
#define SD_SCK      6
#define SD_CS       10

#define I2S_DIN     8
#define I2S_BCK     9
#define I2S_LCK     18

#define BTN_PLAY    3   // SW1 - Play / Pause
#define BTN_NEXT    1   // SW2 - Next track
#define BTN_PREV    0   // SW3 - Previous track

// ─── OBJECTS ──────────────────────────────────────────────────
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE, I2C_SCL, I2C_SDA);

AudioGeneratorMP3 *mp3;
AudioFileSourceSD *file;
AudioOutputI2S    *out;

// ─── TRACK LIST ───────────────────────────────────────────────
#define MAX_TRACKS 50
char trackList[MAX_TRACKS][64];
int  trackCount   = 0;
int  currentTrack = 0;
bool isPlaying    = false;

// ─── BUTTON DEBOUNCE ──────────────────────────────────────────
unsigned long lastBtnPress[3] = {0, 0, 0};
#define DEBOUNCE_MS 200

// ─── HELPERS ──────────────────────────────────────────────────

// Scan SD root for .mp3 files
void scanTracks() {
  File root = SD.open("/");
  trackCount = 0;
  while (true) {
    File entry = root.openNextFile();
    if (!entry || trackCount >= MAX_TRACKS) break;
    String name = entry.name();
    if (!entry.isDirectory() && name.endsWith(".mp3")) {
      name = "/" + name;
      name.toCharArray(trackList[trackCount], 64);
      trackCount++;
    }
    entry.close();
  }
  root.close();
}

// Draw now-playing screen on OLED
void updateOLED() {
  oled.clearBuffer();
  oled.setFont(u8g2_font_6x10_tf);

  if (trackCount == 0) {
    oled.drawStr(0, 12, "No MP3 found!");
    oled.drawStr(0, 26, "Put files on SD");
  } else {
    // Track name (strip leading slash)
    String name = String(trackList[currentTrack]);
    if (name.startsWith("/")) name = name.substring(1);
    // Truncate if too long
    if (name.length() > 20) name = name.substring(0, 17) + "...";

    oled.drawStr(0, 10, isPlaying ? ">> NOW PLAYING" : "|| PAUSED");
    oled.drawStr(0, 22, name.c_str());

    // Track counter e.g. "2 / 5"
    String counter = String(currentTrack + 1) + " / " + String(trackCount);
    oled.drawStr(0, 32, counter.c_str());
  }
  oled.sendBuffer();
}

// Start playing currentTrack
void startTrack() {
  if (trackCount == 0) return;

  // Stop previous if running
  if (mp3 && mp3->isRunning()) {
    mp3->stop();
    delete mp3;
    delete file;
    mp3  = nullptr;
    file = nullptr;
  }

  file = new AudioFileSourceSD(trackList[currentTrack]);
  mp3  = new AudioGeneratorMP3();
  mp3->begin(file, out);
  isPlaying = true;
  updateOLED();
  Serial.printf("Playing: %s\n", trackList[currentTrack]);
}

void stopTrack() {
  if (mp3 && mp3->isRunning()) mp3->stop();
  isPlaying = false;
  updateOLED();
}

// ─── SETUP ────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  Serial.println("\n== MINI DAP SNOWY ==");

  // Buttons (INPUT_PULLUP → pressed = LOW)
  pinMode(BTN_PLAY, INPUT_PULLUP);
  pinMode(BTN_NEXT,  INPUT_PULLUP);
  pinMode(BTN_PREV,  INPUT_PULLUP);

  // OLED
  Wire.begin(I2C_SDA, I2C_SCL);
  oled.begin();
  oled.clearBuffer();
  oled.setFont(u8g2_font_6x10_tf);
  oled.drawStr(0, 16, "DAP SNOWY");
  oled.drawStr(0, 28, "Booting...");
  oled.sendBuffer();

  // SD Card
  SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS)) {
    Serial.println("SD Card FAILED!");
    oled.clearBuffer();
    oled.drawStr(0, 16, "SD Card Error!");
    oled.sendBuffer();
    while (true) delay(1000);
  }
  Serial.println("SD Card OK");

  // Scan tracks
  scanTracks();
  Serial.printf("Found %d tracks\n", trackCount);

  // I2S output → PCM5102
  out = new AudioOutputI2S();
  out->SetPinout(I2S_BCK, I2S_LCK, I2S_DIN);
  out->SetGain(0.8);  // 0.0 - 1.0 volume

  mp3  = nullptr;
  file = nullptr;

  updateOLED();

  // Auto-play first track
  if (trackCount > 0) startTrack();
}

// ─── LOOP ─────────────────────────────────────────────────────
void loop() {
  unsigned long now = millis();

  // ── Button: Play / Pause (SW1) ──
  if (digitalRead(BTN_PLAY) == LOW && now - lastBtnPress[0] > DEBOUNCE_MS) {
    lastBtnPress[0] = now;
    if (isPlaying) {
      stopTrack();
    } else {
      if (mp3 && !mp3->isRunning()) {
        mp3->begin(file, out);
        isPlaying = true;
        updateOLED();
      } else {
        startTrack();
      }
    }
  }

  // ── Button: Next (SW2) ──
  if (digitalRead(BTN_NEXT) == LOW && now - lastBtnPress[1] > DEBOUNCE_MS) {
    lastBtnPress[1] = now;
    currentTrack = (currentTrack + 1) % trackCount;
    startTrack();
  }

  // ── Button: Prev (SW3) ──
  if (digitalRead(BTN_PREV) == LOW && now - lastBtnPress[2] > DEBOUNCE_MS) {
    lastBtnPress[2] = now;
    currentTrack = (currentTrack - 1 + trackCount) % trackCount;
    startTrack();
  }

  // ── Feed audio decoder ──
  if (mp3 && mp3->isRunning()) {
    if (!mp3->loop()) {
      // Track finished → auto-next
      Serial.println("Track ended, playing next...");
      currentTrack = (currentTrack + 1) % trackCount;
      startTrack();
    }
  }
}
