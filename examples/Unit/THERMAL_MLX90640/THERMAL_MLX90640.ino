/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with Atom-Lite/Matrix sample source code
*                          配套  Atom-Lite/Matrix 示例源代码
* Visit the website for more
information：https://docs.m5stack.com/en/unit/thermal
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/unit/thermal
*
* describe: thermal.
* date：2021/8/31
*******************************************************************************
  Please connect to Port, Read the THERMAL Unit (MLX90640 IR array) temperature
pixels and display it on the Serial. 请连接端口,
读取热单元(MLX90640红外阵列)温度像素，并显示在串口上。
*/
#include <M5Atom.h>

#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"

const byte MLX90640_address = 0x33;  // Default 7-bit unshifted address of the
                                     // MLX90640.  MLX90640的默认7位未移位地址
#define TA_SHIFT \
    8  // Default shift for MLX90640 in open air.  MLX90640在户外的默认移位

#define COLS   32
#define ROWS   24
#define COLS_2 (COLS * 2)
#define ROWS_2 (ROWS * 2)

float pixelsArraySize = COLS * ROWS;
float pixels[COLS * ROWS];
float pixels_2[COLS_2 * ROWS_2];
float reversePixels[COLS * ROWS];

byte speed_setting = 2;  // High is 1 , Low is 2
bool reverseSerial = false;

#define INTERPOLATED_COLS 32
#define INTERPOLATED_ROWS 32

static float mlx90640To[COLS * ROWS];
paramsMLX90640 mlx90640;
float signedMag12ToFloat(uint16_t val);

// low range of the sensor (this will be blue on the Serial).
// 传感器的低量程(串口上显示为蓝色)
int MINTEMP   = 24;   // For color mapping.  颜色映射
int min_v     = 24;   // Value of current min temp.  当前最小温度的值
int min_cam_v = -40;  // Spec in datasheet.  规范的数据表

// high range of the sensor (this will be red on the Serial).
// 传感器的高量程(串口上显示为红色)
int MAXTEMP      = 35;   // For color mapping.  颜色映射
int max_v        = 35;   // Value of current max temp.  当前最大温度值
int max_cam_v    = 300;  // Spec in datasheet.  规范的数据表
int resetMaxTemp = 45;

// the colors we will be using.  我们将要使用的颜色
const uint16_t camColors[] = {
    0x480F, 0x400F, 0x400F, 0x400F, 0x4010, 0x3810, 0x3810, 0x3810, 0x3810,
    0x3010, 0x3010, 0x3010, 0x2810, 0x2810, 0x2810, 0x2810, 0x2010, 0x2010,
    0x2010, 0x1810, 0x1810, 0x1811, 0x1811, 0x1011, 0x1011, 0x1011, 0x0811,
    0x0811, 0x0811, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0031, 0x0031,
    0x0051, 0x0072, 0x0072, 0x0092, 0x00B2, 0x00B2, 0x00D2, 0x00F2, 0x00F2,
    0x0112, 0x0132, 0x0152, 0x0152, 0x0172, 0x0192, 0x0192, 0x01B2, 0x01D2,
    0x01F3, 0x01F3, 0x0213, 0x0233, 0x0253, 0x0253, 0x0273, 0x0293, 0x02B3,
    0x02D3, 0x02D3, 0x02F3, 0x0313, 0x0333, 0x0333, 0x0353, 0x0373, 0x0394,
    0x03B4, 0x03D4, 0x03D4, 0x03F4, 0x0414, 0x0434, 0x0454, 0x0474, 0x0474,
    0x0494, 0x04B4, 0x04D4, 0x04F4, 0x0514, 0x0534, 0x0534, 0x0554, 0x0554,
    0x0574, 0x0574, 0x0573, 0x0573, 0x0573, 0x0572, 0x0572, 0x0572, 0x0571,
    0x0591, 0x0591, 0x0590, 0x0590, 0x058F, 0x058F, 0x058F, 0x058E, 0x05AE,
    0x05AE, 0x05AD, 0x05AD, 0x05AD, 0x05AC, 0x05AC, 0x05AB, 0x05CB, 0x05CB,
    0x05CA, 0x05CA, 0x05CA, 0x05C9, 0x05C9, 0x05C8, 0x05E8, 0x05E8, 0x05E7,
    0x05E7, 0x05E6, 0x05E6, 0x05E6, 0x05E5, 0x05E5, 0x0604, 0x0604, 0x0604,
    0x0603, 0x0603, 0x0602, 0x0602, 0x0601, 0x0621, 0x0621, 0x0620, 0x0620,
    0x0620, 0x0620, 0x0E20, 0x0E20, 0x0E40, 0x1640, 0x1640, 0x1E40, 0x1E40,
    0x2640, 0x2640, 0x2E40, 0x2E60, 0x3660, 0x3660, 0x3E60, 0x3E60, 0x3E60,
    0x4660, 0x4660, 0x4E60, 0x4E80, 0x5680, 0x5680, 0x5E80, 0x5E80, 0x6680,
    0x6680, 0x6E80, 0x6EA0, 0x76A0, 0x76A0, 0x7EA0, 0x7EA0, 0x86A0, 0x86A0,
    0x8EA0, 0x8EC0, 0x96C0, 0x96C0, 0x9EC0, 0x9EC0, 0xA6C0, 0xAEC0, 0xAEC0,
    0xB6E0, 0xB6E0, 0xBEE0, 0xBEE0, 0xC6E0, 0xC6E0, 0xCEE0, 0xCEE0, 0xD6E0,
    0xD700, 0xDF00, 0xDEE0, 0xDEC0, 0xDEA0, 0xDE80, 0xDE80, 0xE660, 0xE640,
    0xE620, 0xE600, 0xE5E0, 0xE5C0, 0xE5A0, 0xE580, 0xE560, 0xE540, 0xE520,
    0xE500, 0xE4E0, 0xE4C0, 0xE4A0, 0xE480, 0xE460, 0xEC40, 0xEC20, 0xEC00,
    0xEBE0, 0xEBC0, 0xEBA0, 0xEB80, 0xEB60, 0xEB40, 0xEB20, 0xEB00, 0xEAE0,
    0xEAC0, 0xEAA0, 0xEA80, 0xEA60, 0xEA40, 0xF220, 0xF200, 0xF1E0, 0xF1C0,
    0xF1A0, 0xF180, 0xF160, 0xF140, 0xF100, 0xF0E0, 0xF0C0, 0xF0A0, 0xF080,
    0xF060, 0xF040, 0xF020, 0xF800,
};

float get_point(float *p, uint8_t rows, uint8_t cols, int8_t x, int8_t y);
void set_point(float *p, uint8_t rows, uint8_t cols, int8_t x, int8_t y,
               float f);
void get_adjacents_1d(float *src, float *dest, uint8_t rows, uint8_t cols,
                      int8_t x, int8_t y);
void get_adjacents_2d(float *src, float *dest, uint8_t rows, uint8_t cols,
                      int8_t x, int8_t y);
float cubicInterpolate(float p[], float x);
float bicubicInterpolate(float p[], float x, float y);
void interpolate_image(float *src, uint8_t src_rows, uint8_t src_cols,
                       float *dest, uint8_t dest_rows, uint8_t dest_cols);

long loopTime, startTime, endTime, fps;

void setup() {
    M5.begin();
    Wire.begin(26, 32);
    Wire.setClock(
        450000);  // Increase I2C clock speed to 400kHz. 增加I2C时钟速度到400kHz

    while (!Serial)
        ;  // Wait for user to open terminal.  等待用户打开终端
    Serial.println("M5Stack MLX90640 IR Camera");

    // Get device parameters - We only have to do this once.
    // 获取设备参数——我们只需要做一次
    int status;
    uint16_t eeMLX90640[832];  // 32 * 24 = 768
    status = MLX90640_DumpEE(MLX90640_address, eeMLX90640);
    if (status != 0) Serial.println("Failed to load system parameters");

    status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
    if (status != 0) Serial.println("Parameter extraction failed");

    int SetRefreshRate;
    // Setting MLX90640 device at slave address 0x33 to work with 16Hz refresh
    // rate: 设置从地址0x33的MLX90640设备以16Hz刷新率工作:
    // 0x00 – 0.5Hz
    // 0x01 – 1Hz
    // 0x02 – 2Hz
    // 0x03 – 4Hz
    // 0x04 – 8Hz // OK
    // 0x05 – 16Hz // OK
    // 0x06 – 32Hz // Fail
    // 0x07 – 64Hz
    SetRefreshRate = MLX90640_SetRefreshRate(0x33, 0x05);
    // Once params are extracted, we can release eeMLX90640 array.
    // 一旦提取了参数，我们就可以释放eeMLX90640数组
}

void loop() {
    M5.update();
    loopTime  = millis();
    startTime = loopTime;
    // Set Min Value - LongPress.  长按设置最小值
    if (M5.Btn.pressedFor(1000)) {
        if (MINTEMP <= 5) {
            MINTEMP = MAXTEMP - 5;
        } else {
            MINTEMP = MINTEMP - 5;
        }
        // infodisplay();
    }
    // Set Min Value - SortPress.  短按设置最小值
    if (M5.Btn.wasPressed()) {
        if (MINTEMP <= 0) {
            MINTEMP = MAXTEMP - 1;
        } else {
            MINTEMP--;
        }
        // infodisplay();
    }

    for (byte x = 0; x < speed_setting; x++)  // x < 2 Read both subpages
    {
        uint16_t mlx90640Frame[834];
        int status = MLX90640_GetFrameData(MLX90640_address, mlx90640Frame);
        if (status < 0) {
            Serial.print("GetFrame Error: ");
            Serial.println(status);
        }

        float vdd = MLX90640_GetVdd(mlx90640Frame, &mlx90640);
        float Ta  = MLX90640_GetTa(mlx90640Frame, &mlx90640);
        float tr =
            Ta - TA_SHIFT;  // Reflected temperature based on the sensor ambient
                            // temperature.  根据传感器环境温度反射温度
        float emissivity = 0.95;
        MLX90640_CalculateTo(mlx90640Frame, &mlx90640, emissivity, tr,
                             pixels);  // save pixels temp to array (pixels).
                                       // 保存像素temp到数组(像素)
        int mode_ = MLX90640_GetCurMode(MLX90640_address);
        // amendment.  修正案
        MLX90640_BadPixelsCorrection((&mlx90640)->brokenPixels, pixels, mode_,
                                     &mlx90640);
    }

    // Reverse image (order of Integer array).  反向图像(整数数组的顺序)
    if (reverseSerial == 1) {
        for (int x = 0; x < pixelsArraySize; x++) {
            if (x % COLS == 0)  // 32 values wide.  32宽值
            {
                for (int j = 0 + x, k = (COLS - 1) + x; j < COLS + x;
                     j++, k--) {
                    reversePixels[j] = pixels[k];
                }
            }
        }
    }

    float dest_2d[INTERPOLATED_ROWS * INTERPOLATED_COLS];
    int ROWS_i, COLS_j;

    if (reverseSerial == 1) {
        // ** reversePixels  反向像素
        interpolate_image(reversePixels, ROWS, COLS, dest_2d, INTERPOLATED_ROWS,
                          INTERPOLATED_COLS);
    } else {
        interpolate_image(pixels, ROWS, COLS, dest_2d, INTERPOLATED_ROWS,
                          INTERPOLATED_COLS);
        // 32 * 24 = 768
        // 63 * 48 = 3072
        // pixels_2
        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                // 原始数据
                pixels_2[(((y * 2) * (COLS * 2)) + (x * 2))] =
                    pixels[y * COLS + x];
                if (x != 31)
                    pixels_2[(((y * 2) * (COLS * 2)) + (x * 2) + 1)] =
                        (pixels_2[(((y * 2) * (COLS * 2)) + (x * 2))] +
                         pixels_2[(((y * 2) * (COLS * 2)) + (x * 2) + 2)]) /
                        2;
                else
                    pixels_2[(((y * 2) * (COLS * 2)) + (x * 2) + 1)] =
                        (pixels_2[(((y * 2) * (COLS * 2)) + (x * 2))]);
            }
        }
        // 计算y间隔插入数据
        for (int y = 0; y < ROWS; y++)  // 24
        {
            for (int x = 0; x < COLS_2; x++)  // 64
            {
                if (y != 23)
                    pixels_2[(((y * 2) + 1) * (COLS_2)) + x] =
                        (pixels_2[(((y * 2) * COLS_2) + x)] +
                         pixels_2[((((y * 2) + 2) * COLS_2) + x)]) /
                        2;
                else
                    pixels_2[(((y * 2) + 1) * (COLS_2)) + x] =
                        (pixels_2[(((y * 2) * COLS_2) + x)] +
                         pixels_2[(((y * 2) * COLS_2) + x)]) /
                        2;
            }
        }
        //*/

        max_v      = MINTEMP;
        min_v      = MAXTEMP;
        int spot_v = pixels[360];
        spot_v     = pixels[768 / 2];
        // while(1);

        for (int itemp = 0; itemp < sizeof(pixels) / sizeof(pixels[0]);
             itemp++) {
            if (pixels[itemp] > max_v) {
                max_v = pixels[itemp];
            }
            if (pixels[itemp] < min_v) {
                min_v = pixels[itemp];
            }
        }

        if (max_v > max_cam_v | max_v < min_cam_v) {
            Serial.print("Error\n");
        } else {
            Serial.printf("Min:%d C  Max:%d C Center:%d C ", min_v, max_v,
                          spot_v);
        }
        loopTime = millis();
        endTime  = loopTime;
        fps      = 1000 / (endTime - startTime);
        Serial.print("fps:" + String(fps));
        Serial.println();
    }
}

// Returns true if the MLX90640 is detected on the I2C bus.
// 如果在I2C总线上检测到MLX90640，则返回true
boolean isConnected() {
    Wire.beginTransmission((uint8_t)MLX90640_address);
    if (Wire.endTransmission() != 0) return (false);  // Sensor did not ACK.
    return (true);
}
