/*!
 * @file format.ino
 * @brief 格式化env区，注意这个示例会删除数据库里面所有键值对，慎用！
 * @n 实验现象：串口监视器打印出key对应value
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
  Serial.begin(115200);
  while(!Serial);                                     //等待串口稳定
  easyflash.begin();
  easyflash.setValue(key1,value1);
  easyflash.setValue(key2,value2);
  String string1 = easyflash.getValue(key1);
  String string2 = easyflash.getValue(key2);
  Serial.print("value1:");Serial.println(string1);
  Serial.print("value2:");Serial.println(string2);
  Serial.println("format");
  easyflash.format();                            //这会格式化env区,删除里面所有键值对
  string1 = easyflash.getValue(key1);
  string2 = easyflash.getValue(key2);
  Serial.print("value1:");Serial.println(string1);
  Serial.print("value2:");Serial.println(string2);
}

void loop() {

}