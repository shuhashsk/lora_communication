#include <WiFi.h>
#include <SPI.h>
#include <LoRa.h>
#include "mbedtls/aes.h"
#include "AdafruitIO_WiFi.h"

// ---------------- LoRa Pins ----------------
#define SS   5
#define RST  15
#define DIO0 4

// ---------------- Channels ----------------
const long channels[] = {433000000, 434000000, 435000000};
int currentChannel = 0;

// ---------------- WiFi ----------------
#define WIFI_SSID "Vignesvaran"
#define WIFI_PASS "kav12345"

// ---------------- Adafruit IO ----------------
#define IO_USERNAME "shuhash_sk"
#define IO_KEY "aio_GXLw50pR4cN9w8U3C29vitG9hSGB"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

AdafruitIO_Feed *message_feed = io.feed("lora_message");
AdafruitIO_Feed *status_feed  = io.feed("lora_status");
AdafruitIO_Feed *rssi_feed    = io.feed("rssi");
AdafruitIO_Feed *snr_feed     = io.feed("snr");

// ---------------- AES KEY ----------------
unsigned char aes_key[16] = {
0x01,0x02,0x03,0x04,
0x05,0x06,0x07,0x08,
0x09,0x0A,0x0B,0x0C,
0x0D,0x0E,0x0F,0x10
};

unsigned long lastSwitch = 0;

// ---------------- AES Decryption ----------------
void decrypt(unsigned char *cipherText, unsigned char *key, unsigned char *outputBuffer) {
  mbedtls_aes_context aes;
  mbedtls_aes_init(&aes);
  mbedtls_aes_setkey_dec(&aes, key, 128);
  mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT, cipherText, outputBuffer);
  mbedtls_aes_free(&aes);
}

// ---------------- Channel Switching ----------------
void switchChannel() {

  currentChannel = (currentChannel + 1) % 3;

  LoRa.end();
  delay(300);

  LoRa.begin(channels[currentChannel]);
  LoRa.setSyncWord(0xF1);
  LoRa.setSpreadingFactor(7);

  Serial.print("Switched To Channel: ");
  Serial.println(channels[currentChannel]);

  // Inform sender
  LoRa.beginPacket();
  LoRa.print("CH");
  LoRa.write(currentChannel);
  LoRa.endPacket();
}

// ---------------- Interference Check ----------------
void checkInterference(int rssi, float snr) {

  if ((snr < 10 || rssi < -60) && millis() - lastSwitch > 5000) {

    Serial.println("⚠ Interference Detected!");

    // Send interference status to dashboard
    message_feed->save("Interference Detected");
    status_feed->save("Channel Switching");

    switchChannel();

    lastSwitch = millis();
  }
}

void setup() {

  Serial.begin(115200);
  delay(1000);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  io.connect();
  while(io.status() < AIO_CONNECTED) {
    delay(500);
  }

  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(channels[currentChannel])) {
    Serial.println("LoRa Init Failed!");
    while (1);
  }

  LoRa.setSyncWord(0xF1);
  LoRa.setSpreadingFactor(7);

  Serial.println("---- GATEWAY READY ----");
}

void loop() {

  io.run();

  int packetSize = LoRa.parsePacket();

  if (packetSize) {

    unsigned char incoming[16] = {0};
    int i = 0;

    while (LoRa.available() && i < 16) {
      incoming[i++] = LoRa.read();
    }

    if (packetSize == 4) {

      char statusMsg[5];
      for(int j = 0; j < 4; j++)
        statusMsg[j] = (char)incoming[j];

      statusMsg[4] = '\0';

      status_feed->save(statusMsg);
    }

    else if (packetSize == 16) {

      unsigned char decrypted[16] = {0};
      decrypt(incoming, aes_key, decrypted);

      char message[17];
      for(int k = 0; k < 16; k++)
        message[k] = (char)decrypted[k];

      message[16] = '\0';

      Serial.println("Secure Message:");
      Serial.println(message);

      message_feed->save(message);
    }

    int rssi = LoRa.packetRssi();
    float snr = LoRa.packetSnr();

    Serial.printf("RSSI: %d dBm | SNR: %.2f dB\n", rssi, snr);

    rssi_feed->save(rssi);
    snr_feed->save(snr);

    checkInterference(rssi, snr);

    Serial.println("-----------------------------");
  }
}