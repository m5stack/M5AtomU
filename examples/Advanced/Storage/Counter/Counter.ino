/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with Atom-Lite/Matrix sample source code
*                          配套  Atom-Lite/Matrix 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/products
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/products
*
* describe：counter.  计数器
* date：2021/8/3
*******************************************************************************
*/

#include <M5Atom.h>
#include <Preferences.h>

Preferences preferences;

void setup() {
    M5.begin();  // Init Atom.  初始化 ATOM
    preferences.begin(
        "my-app",
        false);  // We will open storage in RW-mode (second parameter has to be
                 // false).
                 // 在perferences中创建叫my-app的空间,并以rw模式打开存储(第二个参数必须为false)

    // preferences.clear();  // Remove all preferences under the opened
    // namespace.清除preferences中所有的空间

    // preferences.remove("counter");  // Or remove the counter key only.
    // 只清除counter中的值

    unsigned int counter = preferences.getUInt(
        "counter",
        0);  // Get the counter value in current sapce, if the key does not
             // exist, return a default value of 0.
             // 在当前空间中读取counter的值(若不存在为0),并赋值给counter
    counter++;  // Increase counter by 1.  使计数器的值加一
    Serial.printf(
        "Current counter value: %u\n",
        counter);  // Print the counter to Serial Monitor.  串口输出计数器的值
    preferences.putUInt(
        "counter",
        counter);  // Store the counter to the Preferences.  存储计数器的值
    preferences.end();  // Close the Preferences.  关闭Preferences
    Serial.println("Restarting in 10 seconds...");
    delay(10000);   // delay 10.  延迟10s
    ESP.restart();  // Restart.  重启
}
void loop() {
}
