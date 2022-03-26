# ATOM Library

中文 | [English](README_cn.md)

<img src="https://static-cdn.m5stack.com/resource/docs/static/assets/img/product_pics/core/atom_u/atom_u_01.webp" alt="M5Atom U" width="350" height="350">

## 描述

**ATOM U** 是一款非常小巧灵活的物联网语音识别开发板, 采用乐鑫`ESP32`主控芯片，搭载2个低功耗 `Xtensa® 32-bit LX6` 微处理器，主频高达`240MHz`。集成USB-A接口，IR发射管， 可编程LED灯等外设，即插即用，便于程序上传下载调试。集成`Wi-Fi`和`蓝牙`模块, 搭配内置的数字麦克风SPM1423(I2S), 可实现清晰音频录制, 适用于各种物联网人机交互，语音输入识别场景(STT)。

- `低代码开发`:
   - 支持UIFlow图形化编程平台、脚本化、免编译、云推送
   - 全面兼容Arduino、ESP32-IDF等主流开发平台
- `高集成度`：
   - USB-A编程/供电接口，IR发射管，可编程RGB灯x1、按键x1
   - 经过专业调制的射频电路, 提供稳定可靠的无线通信质量。
- `强拓展性`：
   - 轻松接入M5Stack的软硬件体系

## 产品特性

- ESP32-PICO-D4(2.4GHz Wi-Fi 和蓝牙双模)
- 集成可编程RGB LED与按键
- 机身小巧
- 内置红外发射器
- 可扩展的引脚与GROVE接口
- 开发平台:
   - [UIFlow](http://flow.m5stack.com)
   - [MicroPython](http://micropython.org/)
   - [Arduino](http://www.arduino.cc)

## 应用

- 物联网控制器
- 语音录制、云端STT识别

## 驱动安装

将设备连接至PC，打开设备管理器为设备安装[FTDI驱动](https://ftdichip.com/drivers/vcp-drivers/)。以win10环境为例,下载匹配操作系统的驱动文件, 并解压,通过设备管理器进行安装。（注：某些系统环境下,需要安装两次,驱动才会生效,未识别的设备名通常为`M5Stack`或`USB Serial`, Windows推荐使用驱动文件在设备管理器直接进行安装(自定义更新), 可执行文件安装方式可能无法正常工作）。[点击此处，前往下载FTDI驱动](https://ftdichip.com/drivers/vcp-drivers/)

## 管脚映射

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

**UIFlow 快速上手**: [点击这里](https://docs.m5stack.com/zh_CN/quick_start/atom_u/uiflow)

**MicroPython API**: [点击这里](https://docs.m5stack.com/zh_CN/quick_start/atom_u/mpy)

**Arduino IDE 环境搭建**: [点击这里](https://docs.m5stack.com/zh_CN/quick_start/atom_u/arduino)