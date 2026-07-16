# lora_communication
LoRa Based Communication System with Gateway Intelligence and Real-Time Anti-Jamming Control.

# 📡 LoRa Anti-Jamming Gateway

A robust, multi-node wireless communication system utilizing **LoRa technology** for long-range communication with **real-time AES encryption**, **dynamic anti-jamming channel switching**, and **gateway-based network intelligence**. The system ensures secure and reliable data transmission between distributed sensor nodes and a centralized monitoring dashboard.

---

## 📋 Table of Contents

- [Overview](#overview)
- [System Architecture](#system-architecture)
- [Hardware Requirements](#hardware-requirements)
- [Key Features](#key-features)
- [Pin Configuration & Wiring](#pin-configuration--wiring)
- [Software & Firmware Setup](#software--firmware-setup)
- [Project Directory Structure](#project-directory-structure)
- [Project Artifacts](#project-artifacts)
- [Future Enhancements](#future-enhancements)
- [License](#license)

---

# Overview

This project demonstrates a **secure LoRa-based wireless communication network** consisting of multiple transmitter nodes and a centralized ESP32 gateway. The gateway continuously monitors communication quality, detects signal interference (jamming), and dynamically switches communication channels to maintain uninterrupted data transmission.

Each transmitted packet is encrypted using **AES cryptography**, ensuring confidentiality even over long-range wireless links. The gateway also uploads communication statistics such as RSSI, SNR, and received messages to an IoT dashboard for real-time monitoring.

---

# 🏗️ System Architecture

The overall system consists of three major components:

1. **End Device (Sender Node)**
   - ESP32 + LoRa
   - Encrypts messages using AES
   - Sends packets over LoRa

2. **Gateway Node**
   - ESP32 + LoRa
   - Detects interference
   - Performs automatic channel switching
   - Decrypts received packets
   - Uploads telemetry to cloud dashboard

3. **Cloud Dashboard**
   - Displays received messages
   - RSSI
   - SNR
   - Communication status
   - Active communication channel

> **Add your architecture diagram here**

```markdown
![System Architecture](assets/system_block_diagram.png)
```

Example folder:

```
LoRa-AntiJamming-Gateway/
│
├── assets/
│     └── system_block_diagram.png
│
├── src/
├── README.md
```

---

# 🛠️ Hardware Requirements

## 📡 Gateway Node

| Component | Description |
|-----------|-------------|
| ESP32 Development Board | Main controller |
| SX1278 / SX1276 LoRa Module | Long-range wireless communication |
| OLED Display (SSD1306) *(Optional)* | Communication status |
| Wi-Fi | Cloud connectivity |

---

## 🌿 Sender Node

| Component | Description |
|-----------|-------------|
| ESP32 Development Board | Controller |
| SX1278 / SX1276 LoRa Module | LoRa Transceiver |
| 4×4 Keypad *(Optional)* | Message input |
| LCD/OLED Display *(Optional)* | Local display |

---

## ☁️ Dashboard

- Adafruit IO Dashboard
- Wi-Fi Network
- MQTT / HTTP Communication

---

# ⚡ Key Features

## 🔐 AES Encrypted Communication

- End-to-end AES encryption
- Secure wireless packet transmission
- Prevents unauthorized message access

---

## 📶 Long-Range LoRa Communication

- Low power consumption
- Long communication range
- Reliable packet delivery

---

## 🛰️ Intelligent Gateway

- Receives encrypted packets
- Decrypts messages
- Maintains network statistics
- Forwards telemetry to cloud

---

## 🚨 Real-Time Anti-Jamming

- Continuously monitors RSSI and SNR
- Detects interference automatically
- Performs dynamic channel switching
- Restores communication without manual intervention

---

## ☁️ IoT Dashboard Integration

Displays

- Received Messages
- RSSI
- SNR
- Communication Status
- Current LoRa Channel

---

# 📌 Pin Configuration & Wiring

## ESP32 ↔ SX1278/SX1276 LoRa Module

| LoRa Module Pin | ESP32 Pin | Description |
|-----------------|-----------|-------------|
| VCC | 3.3V | Power Supply *(Never connect to 5V)* |
| GND | GND | Ground |
| MISO | GPIO19 | SPI MISO |
| MOSI | GPIO23 | SPI MOSI |
| SCK | GPIO18 | SPI Clock |
| NSS / CS | GPIO5 | Chip Select |
| RST | GPIO14 | Reset |
| DIO0 | GPIO2 | Interrupt |

---

# 💻 Software & Firmware Setup

## Development Environment

- Arduino IDE
- PlatformIO (Optional)

---

## Required Libraries

```
SPI.h
LoRa.h
WiFi.h
Wire.h
Adafruit_GFX.h
Adafruit_SSD1306.h
AESLib.h
AdafruitIO_WiFi.h
```

Install all libraries using the Arduino Library Manager.

---

## Flashing the Sender Node

1. Open

```
src/Sender/
```

2. Configure

- AES Key
- LoRa Frequency
- Node ID

3. Upload to ESP32.

---

## Flashing the Gateway

Open

```
src/Gateway/
```

Update

```
WiFi SSID
WiFi Password
Adafruit IO Username
Adafruit IO Key
LoRa Frequency
```

Upload the firmware to the Gateway ESP32.

---

# 📂 Project Directory Structure

```
LoRa-AntiJamming-Gateway
│
├── assets/
│   ├── system_block_diagram.png
│   ├── gateway.jpg
│   └── dashboard.png
│
├── src/
│   ├── Sender/
│   ├── Gateway/
│   └── Receiver/
│
├── docs/
│   └── lora_paper.pdf
│
├── README.md
└── LICENSE
```

---

# 📄 Project Artifacts

## 📘 Research Paper

The complete technical methodology, mathematical modeling, implementation details, experimental analysis, and anti-jamming algorithm are documented in:

```
docs/lora_paper.pdf
```

The paper includes:

- System Design
- AES Encryption Methodology
- LoRa Communication Protocol
- Anti-Jamming Strategy
- Experimental Results
- Performance Analysis
- Future Scope

---

# 🚀 Future Enhancements

- Machine Learning-based Jamming Detection
- Adaptive Frequency Hopping (FHSS)
- Multi-Gateway Architecture
- GPS-Based Node Localization
- OTA Firmware Updates
- Web Dashboard
- LoRaWAN Integration
- Blockchain-Based Authentication


## Hardware Setup

![Gateway](assets/gateway.jpg)

## Dashboard

![Dashboard](assets/dashboard.png)
```

---

# 👨‍💻 Author

**Shuhash SK**

Electrical and Computer Engineering Student

Specialization:
- Embedded Systems
- IoT
- Wireless Communication
- Cyber Physical Systems
- Deep Learning

---

# 📜 License

This project is licensed under the **MIT License**.

Feel free to use, modify, and contribute to this project.
