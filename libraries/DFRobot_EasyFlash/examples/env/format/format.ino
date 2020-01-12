/*!
 * @file format.ino
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

char key1[] = "fruit";               
char value1[] = "apple";
String key2 = "vegetable";
String value2 = "cabbage";
char valueRec[20];
size_t size;

void setup()
{
  SerialUSB.begin(115200);
  easyflash.begin();
  easyflash.setValue(key1,value1);
  easyflash.setValue(key2,value2);
  easyflash.format();                            //这会格式化env区
  String string1 = easyflash.getValue(key1);
  String string2 = easyflash.getValue(key2);
  SerialUSB.println(string1);
  SerialUSB.println(string2);
}

void loop() {

}
