/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with Atom-Lite/Matrix sample source code
*                          配套  Atom-Lite/Matrix 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/unit/ir
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/unit/ir
*
* describe: ir.
* date：2021/8/27
*******************************************************************************
  Please connect to Port B,Use IR Unit to receive and test infrared receiving
and transmitting 请连接端口B,使用红外单元接收和测试红外接收和发射.
*/

#include <M5Atom.h>

int ir_recv_pin = 32;  // set the input pin.  设置引脚
int ir_send_pin = 26;

int last_recv_value = 0;
int cur_recv_value  = 0;

void setup() {
    M5.begin();
    Serial.begin(115200);
    pinMode(ir_recv_pin, INPUT);
    pinMode(ir_send_pin, OUTPUT);
    // send infrared light.  发送红外线
    // now, you can see the infrared light through mobile phone camera.
    // 现在，你可以通过手机摄像头看到红外光
    digitalWrite(ir_send_pin, 1);
    Serial.println("Test for IR receiver: ");
}

void loop() {
    // now, once you press the button on a remote controller to send infrared
    // light.  现在，一旦你按下遥控器上的按钮发送红外线 the Serial will display
    // "detected!"  串口将显示“检测到!”
    cur_recv_value = digitalRead(ir_recv_pin);
    if (last_recv_value != cur_recv_value) {
        if (cur_recv_value ==
            0) {  // 0: detected 1: not detected,  0检测到,1没有检测到
            Serial.println("detected!");
        }
        last_recv_value = cur_recv_value;
    }
}
