/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with Atom-Lite/Matrix sample source code
*                          配套  Atom-Lite/Matrix 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/vmeter
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/unit/vmeter
*
* Product:  Vmeter_ADS1115.
* Date: 2022/7/11
*******************************************************************************
  Please connect to Port,Measure voltage and display in the serial.
  请连接端口,测量电压并显示到串口
  Pay attention: EEPROM (0x53) has built-in calibration parameters when leaving
  the factory. Please do not write to the EEPROM, otherwise the calibration data
  will be overwritten and the measurement results will be inaccurate.
  注意:EEPROM在出厂时具有内置的校准参数。请不要写入EEPROM，否则校准数据会被覆盖，测量结果会不准确。
*/

#include "M5Atom.h"
#include <Wire.h>
#include "M5_ADS1115.h"
#include "title.h"
#include "shut.h"

ADS1115 voltmeter;

float pgae512_volt  = 5000.0F;
float pgae4096_volt = 60000.0F;

int16_t volt_raw_list[10];
uint8_t raw_now_ptr = 0;
int16_t adc_raw     = 0;

int16_t hope           = 0.0;
uint8_t voltage_change = 0;

ADS1115Gain_t now_gain = PGA_512;

int x     = 0;
int xt    = 0;
int value = 0;

int bright[4] = {30, 60, 100, 200};
int b         = 1;
bool d        = 0;

void setup() {
    M5.begin();
    Wire.begin(26, 32);

    voltmeter.setMode(SINGLESHOT);
    voltmeter.setRate(RATE_128);
    voltmeter.setGain(PGA_512);
}

void loop(void) {
    M5.update();

    voltmeter.getValue();

    volt_raw_list[raw_now_ptr] = voltmeter.adc_raw;
    raw_now_ptr                = (raw_now_ptr == 9) ? 0 : (raw_now_ptr + 1);

    int count = 0;
    int total = 0;

    for (uint8_t i = 0; i < 10; i++) {
        if (volt_raw_list[i] == 0) {
            continue;
        }
        total += volt_raw_list[i];
        count += 1;
    }

    if (count == 0) {
        adc_raw = 0;
    } else {
        adc_raw = total / count;
    }

    value = adc_raw * voltmeter.resolution * voltmeter.calibration_factor;

    if (d == 0)
        Serial.printf("%.2f   \r\n", (

                                         adc_raw * voltmeter.resolution *
                                         voltmeter.calibration_factor) /
                                         1000);
    if (d == 1)
        Serial.printf("%.3f  \r\n", (adc_raw * voltmeter.resolution *
                                     voltmeter.calibration_factor) /
                                        1000);

    Serial.print(
        String(adc_raw * voltmeter.resolution * voltmeter.calibration_factor));
    Serial.printf("ADC:%s", String(adc_raw));

    if (value < 0) value = value * -1;

    x = map(value, 0, 32000, 16, 304);

    if (M5.Btn.wasPressed()) d = !d;
}