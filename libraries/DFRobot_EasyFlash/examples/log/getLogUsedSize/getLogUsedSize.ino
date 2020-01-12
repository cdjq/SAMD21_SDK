/*!
 * @file getSize.ino
 * @brief 写入log值然后读取log总字节数
 * @n 实验现象：串口监视器打印出log的总字节数
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [LiYue](liyue.wang@dfrobot.com)
 * @version  V1.0
 * @date  2019-12-23
 * @get from https://www.dfrobot.com
 * @url https://github.com/cdjq/DFRobot_EasyFlash
 */
 
#include <DFRobot_EasyFlash.h>

DFRobot_EasyFlash easyflash;
size_t size;

void setup() {
  SerialUSB.begin(115200);
  easyflash.begin();
  size = easyflash.logSize();
  SerialUSB.println(size);
}

void loop() {
  // put your main code here, to run repeatedly:

}
