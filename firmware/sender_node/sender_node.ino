#include <SPI.h>
#include <LoRa.h>
#include "mbedtls/aes.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------------- LoRa Pins ----------------
#define SS   5
#define RST  15
#define DIO0 26

// ---------------- Channel List ----------------
const long channels[] = {433000000, 434000000, 435000000};
int currentChannel = 0;

// ---------------- AES Key ----------------
unsigned char aes_key[16] = {
  0x01,0x02,0x03,0x04,
  0x05,0x06,0x07,0x08,
  0x09,0x0A,0x0B,0x0C,
  0x0D,0x0E,0x0F,0x10
};

// ---------------- LCD Setup ----------------
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 chars, 2 lines

// ---------------- AES Encrypt Function ----------------
void encrypt(unsigned char * plainText, unsigned char * key, unsigned char * outputBuffer) {
  mbedtls_aes_context aes;
  mbedtls_aes_init(&aes);
  mbedtls_aes_setkey_enc(&aes, key, 128);
  mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, plainText, outputBuffer);
  mbedtls_aes_free(&aes);
}

// ---------------- Channel Update ----------------
void updateChannel(int newChannel) {
  currentChannel = newChannel;
  LoRa.end();
  delay(300);

  if (!LoRa.begin(channels[currentChannel])) {
    Serial.println("LoRa Re-Init Failed!");
    lcd.setCursor(0, 1);
    lcd.print("LoRa Re-Init Err");
    while (1);
  }

  LoRa.setSyncWord(0xF1);
  LoRa.setSpreadingFactor(7);

  Serial.print("Channel Updated To: ");
  Serial.println(channels[currentChannel]);
  lcd.setCursor(0, 1);
  lcd.print("Ch:");
  lcd.print(channels[currentChannel]);
}

// ---------------- Setup ----------------
void setup() {
  Serial.begin(115200);

  // LCD Init
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LoRa Sender Ready");

  // LoRa Init
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(channels[currentChannel])) {
    Serial.println("LoRa Initialization Failed!");
    lcd.setCursor(0, 1);
    lcd.print("LoRa Init Failed!");
    while (1);
  }

  LoRa.setSyncWord(0xF1);
  LoRa.setSpreadingFactor(7);

  Serial.println("---- SENDER READY ----");
}

// ---------------- Loop ----------------
void loop() {

  // -------- Listen for Channel Change Command --------
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String control = "";
    while (LoRa.available()) {
      control += (char)LoRa.read();
    }

    if (control.startsWith("CH") && control.length() >= 3) {
      int newChannel = control[2] - '0';
      if (newChannel >= 0 && newChannel < 3) {
        updateChannel(newChannel);
      }
    }
  }

  // -------- Send Encrypted Message --------
  if (Serial.available() > 0) {
    String msg = Serial.readStringUntil('\n');
    msg.trim();

    if (msg.length() > 0) {
      unsigned char data[16] = {0};
      unsigned char cipher[16] = {0};

      for (int i = 0; i < 16 && i < msg.length(); i++)
        data[i] = msg[i];

      encrypt(data, aes_key, cipher);

      LoRa.beginPacket();
      LoRa.write(cipher, 16);
      LoRa.endPacket();

      Serial.println("Encrypted Message Sent Successfully!");

      // ---------------- Display on LCD ----------------
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Msg:");
      lcd.setCursor(4, 0);
      lcd.print(msg.length() > 12 ? msg.substring(0, 12) : msg); // fit 16 chars

      lcd.setCursor(0, 1);
      lcd.print("Enc:");
      for (int i = 0; i < 4; i++) { // show first 4 bytes for brevity
        lcd.print(cipher[i], HEX);
        lcd.print(" ");
      }
    }
  }

  // -------- Heartbeat Every 10 sec --------
  static unsigned long hb = 0;
  if (millis() - hb > 10000) {
    LoRa.beginPacket();
    LoRa.print("LIVE");
    LoRa.endPacket();
    hb = millis();
  }
}