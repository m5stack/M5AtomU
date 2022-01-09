# ATOM Library

English | [中文](README_cn.md)

<img src="https://static-cdn.m5stack.com/resource/docs/static/assets/img/product_pics/core/atom_u/atom_u_01.webp" alt="M5Atom U" width="350" height="350">

## Description

 **ATOM U** is a compact low-power consumption speech recognition IoT development kit. It adopts an ESP32 chipset, equipped with 2 low-power Xtensa 32-bit LX6 microprocessors with the main frequency of up to 240MHz. Built-in USB-A interface, IR emitter, programmable RGB LED. Plug-and-play, easy to upload and download programs. Integrated `Wi-Fi` and `Bluetooth` modes and digital microphone SPM1423(I2S) for the clear sound record. suitable for HMI, Speech-to-Text (STT).

- LOW-CODE DEVELOPMENT: 
   - ATOM U supports UIFlow graphical programming platform, scripting-free, cloud push;
Fully compatible with Arduino, MicroPython, ESP32-IDF, and other mainstream development platforms, to quickly build various applications.

- HIGH INTEGRATION:
   - ATOM U contains a USB-A port for programming/power supply, IR emitter, programmable RGB LED x1, button x1;
Finely tuned RF circuit, providing stable and reliable wireless communication.

- STRONG EXPANDABILITY:
   - ATOM U is easy access to M5Stack's hardware and software system

## Product Features

- ESP32-PICO-D4 (2.4GHz Wi-Fi and Bluetooth dual mode)
- Integrated programmable RGB LED and button
- Compact design
- Built-in IR emitter
- Expandable pinout and GROVE port
- Development platform:
   - [UIFlow](http://flow.m5stack.com)
   - [MicroPython](http://micropython.org/)
   - [Arduino](http://www.arduino.cc)

## Application

- IoT controller
- Voice recording, Speech-to-Text services

## Driver Installation

Connect the device to the PC, open the device manager to install [FTDI driver](https://ftdichip.com/drivers/vcp-drivers/) for the device. Take the win10 environment as an example, download the driver file that matches the operating system, unzip it, and install it through the device manager. (Note: In some system environments, the driver needs to be installed twice for the driver to take effect. The unrecognized device name is usually `M5Stack` or `USB Serial`. Windows recommends using the driver file to install directly in the device manager (custom Update), the executable file installation method may not work properly). [Click here to download FTDI driver](https://ftdichip.com/drivers/vcp-drivers/)

## PinMap

- SPM1423 - I2S

| M5Core  | G5      | G19      | 3.3V | GND |
| ------- | ------- | -------- | ---- | --- |
| SPM1423 | MIC_CLK | MIC_DATA | VCC  | GND |

- IR & SK6812 & BUTTON

| M5Core | G12 | G27 | G39 |
| ------ | --- | --- | --- |
| IR     | TX  | /   | /   |
| SK6812 | /   | DIN | /   |
| BUTTON | /   | /   | SW  |

## 更多信息

**UIFlow Quick Start**: [Click Here](https://docs.m5stack.com/en/quick_start/atom_u/uiflow)

**MicroPyhton API**: [Click Here](https://docs.m5stack.com/en/quick_start/atom_u/mpy)

**Arduino IDE Development**: [Click Here](https://docs.m5stack.com/en/quick_start/atom_u/arduino)