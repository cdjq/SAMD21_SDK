/*!
 * @file WriteReadTwice.ino
 * @brief 写入2次log值然后读log
 * @n 实验现象：串口监视器打印出log的值
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
const char logToWrite[20] ="The first time";
const char nextLog[20] = "The second time";
char logRec[40];
size_t size;

void setup() {
  SerialUSB.begin(115200);
  easyflash.begin();
  size = logSize();
  easyflash.logWrite(logToWrite,20);
  easyflash.logRead(size,logRec,20);
  for(uint8_t i=0;i<20;i++)
    SerialUSB.print(logRec[i]);
  SerialUSB.println();
  easyflash.logWrite(nextLog,20);
  easyflash.logRead(size,logRec,40);
  for(uint8_t i=0;i<40;i++)
    SerialUSB.print(logRec[i]);
  SerialUSB.println();
}

void loop() {
  // put your main code here, to run repeatedly:

}
