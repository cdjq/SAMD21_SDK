/*!
 * @file delValue.ino
 * @brief 设置一个键值对并通过key删除它
 * @n 实验现象：串口监视器打印出key对应value的长度
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

char key[] = "How are you?";               
char value[] = "I'm fine,thank you.";
char valueRec[20];
size_t size;

void setup()
{
  SerialUSB.begin(115200);
  easyflash.begin();
  easyflash.setValue(key,value);
  getValue(key,valueRec,20，size);                //获取key对应value的长度
  println(size);
  easyflash.delValue(key);                        //根据key删除键值对
  getValue(key,valueRec,20,size);                 //再次获取key对应的value的长度
  println(size);
}

void loop() {

}
