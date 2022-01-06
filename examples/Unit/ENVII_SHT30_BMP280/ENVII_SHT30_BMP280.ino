/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with Atom-Lite/Matrix sample source code
*                          配套  Atom-Lite/Matrix 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/products
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/products
*
* describe：ENVII_SHT30_BMP280.  环境传感器
* date：2021/8/11
*******************************************************************************
  Please connect to Port,Read temperature, humidity and atmospheric pressure and
display them on the display Serial
  请连接端口,读取温度、湿度和大气压强并在显示屏上显示
*/

#include <M5Atom.h>
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
#include "UNIT_ENV.h"
SHT3X sht30;
Adafruit_BMP280 bme;

float tmp      = 0.0;
float hum      = 0.0;
float pressure = 0.0;

void setup() {
    M5.begin();          // Init M5Atom.  初始化 M5Atom
    Wire.begin(26, 32);  // Initialize pin 26,32.  初始化26,32引脚
    Serial.println(F("ENV Unit(SHT30 and BMP280) test...\n"));
}

void loop() {
    while (!bme.begin(0x76)) {  //初始化bme传感器.  Init the sensor of bme
        Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    }
    pressure = bme.readPressure();  // Stores the pressure gained by BMP.
                                    // 存储bmp获取到的压强
    sht30.get();           // Obtain the data of shT30.  获取sht30的数据
    tmp = sht30.cTemp;     // Store the temperature obtained from shT30.
                           // 将sht30获取到的温度存储
    hum = sht30.humidity;  // Store the humidity obtained from the SHT30.
                           // 将sht30获取到的湿度存储
    Serial.printf("Temp: %2.1f\nHumi: %2.0f%%\nPressure:%2.0fPa\n\n", tmp, hum,
                  pressure);
    delay(2000);
}
