/*!
 * @file deleteData.ino
 * @brief 清除log区保存的数据
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

const char logToWrite[24] ="All data wil be cleaned!";
size_t size;

void setup()
{
  SerialUSB.begin(115200);
  easyflash.begin();                        //初始化easyflash
  easyflash.logWrite(logToWrite,24);        //先写入数据保证接下来读到的字节数不为0
  size = logSize();                         //得到log已经使用的字节数
  SerialUSB.println(size);
  easyflash.logClean();        //注意，这会清除log区所有的数据
  size = logSize();
  SerialUSB.println(size);
}

void loop() {

}
