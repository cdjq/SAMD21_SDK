/*!
 * @file writeAndRead.ino
 * @brief 写入log然后读出这次写入的log
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

const char logToWrite[20] ="hello world!";
char logRec[20];
size_t size;

void setup()
{
  SerialUSB.begin(115200);
  easyflash.begin();                        //使用前先初始化easyflash
  size = logSize();                         //得到log已经使用的字节数，log写入是接着log结束字节继续写入的，读取下次写入内容时size就是读取的起始位置
  easyflash.logWrite(logToWrite,12);        //写log，写入的字节数必须是4的倍数
  easyflash.logRead(size,logRec,12);           //读取log，读取的字节数必须是4的倍数，索引值是之前读的size，也就是相对log起始地址的偏移量
  for(uint8_t i=0;i<12;i++)
    SerialUSB.print(logRec[i]);
  SerialUSB.println();
}

void loop() {

}
