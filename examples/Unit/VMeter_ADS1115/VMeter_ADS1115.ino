/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with Atom-Lite/Matrix sample source code
*                          配套  Atom-Lite/Matrix 示例源代码
* Visit the website for more
information：https://docs.m5stack.com/en/unit/ameter
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/unit/ameter
*
* describe: Vmeter_ADS1115.  电流计
* date：2021/8/27
*******************************************************************************
  Please connect to Port,Measure voltage and display in the serial.
  请连接端口,测量电压并显示到串口
  Pay attention: EEPROM (0x53) has built-in calibration parameters when leaving
the factory. Please do not write to the EEPROM, otherwise the calibration data
will be overwritten and the measurement results will be inaccurate. 注意: EEPROM
(0x53)在出厂时具有内置的校准参数。请不要写入EEPROM，否则校准数据会被覆盖，测量结果会不准确。
*/

#include "M5Atom.h"
#include "voltmeter.h"

Voltmeter voltmeter;

float page512_volt  = 5000.0F;
float page4096_volt = 60000.0F;

int16_t volt_raw_list[10];
uint8_t raw_now_ptr = 0;
int16_t adc_raw     = 0;

int16_t hope           = 0.0;
uint8_t voltage_change = 0;

voltmeterGain_t now_gain = PAG_512;

void setup() {
    M5.begin();
    Wire.begin(26, 32);

    voltmeter.setMode(SINGLESHOT);  // | PAG      | Max Input Voltage(V) |
    voltmeter.setRate(RATE_8);      // | PAG_6144 |        128           |
    voltmeter.setGain(PAG_512);     // | PAG_4096 |        64            |
    hope = page512_volt /
           voltmeter.resolution;  // | PAG_2048 |        32            |
                                  // | PAG_512  |        16            |
                                  // | PAG_256  |        8             |
}

void loop(void) {
    M5.update();  // Check the status of the key.  检测按键的状态
    if (M5.Btn.wasPressed()) {
        if (voltage_change == 0) {
            voltmeter.setMode(SINGLESHOT);  // Set the mode.  设置模式
            voltmeter.setRate(RATE_8);      // Set the rate.  设置速率
            voltmeter.setGain(PAG_512);
            now_gain = PAG_512;
            hope     = page512_volt / voltmeter.resolution;

            for (uint8_t i = 0; i < 10; i++) {
                volt_raw_list[i] = 0;
            }
            voltage_change = 1;
        } else if (voltage_change == 1) {
            voltmeter.setMode(SINGLESHOT);
            voltmeter.setRate(RATE_8);
            voltmeter.setGain(PAG_4096);
            now_gain = PAG_4096;
            hope     = page4096_volt / voltmeter.resolution;

            for (uint8_t i = 0; i < 10; i++) {
                volt_raw_list[i] = 0;
            }

            voltage_change = 0;
        }
    }
    voltmeter.getVoltage();

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

    if (now_gain == PAG_512) {
        Serial.printf("Hope volt:");
        Serial.printf("%.2f mv", page512_volt);
    } else {
        Serial.printf("Hope volt:");
        Serial.printf("%.2f mv", page4096_volt);
    }

    Serial.printf("Hope ADC:");
    Serial.printf("%d", hope);

    Serial.printf("Cal volt:");
    Serial.printf("%.2f mv", adc_raw * voltmeter.resolution *
                                 voltmeter.calibration_factor);

    Serial.printf("Cal ADC:");
    Serial.printf("%.0f", adc_raw * voltmeter.calibration_factor);

    Serial.printf("RAW ADC:");
    Serial.printf("%d\n", adc_raw);
}
