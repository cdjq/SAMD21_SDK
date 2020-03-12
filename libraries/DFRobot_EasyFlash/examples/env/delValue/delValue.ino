/*!
 * @file delValue.ino
 * @brief 设置一个键值对并通过key删除它
 * @n 实验现象：串口监视器打印出key对应value的长度
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [LiYue](Yue.Li@dfrobot.com)
 * @version  V1.0
 * @date  2019-12-23
 * @get from https://www.dfrobot.com
 * @url https://github.com/cdjq/DFRobot_EasyFlash
 */

#include <DFRobot_EasyFlash.h>
DFRobot_EasyFlash easyflash;

char key[] = "key";               
char value[] = "value";
char valueRec[20];
size_t valueLen;
size_t getLen;
void setup()
{
  SerialUSB.begin(115200);
  easyflash.begin();
  easyflash.setValue(key,value);
  getLen = easyflash.getValue(key,valueRec,20,valueLen);   //getValue()返回获得的key的长度，valueLen记录value的实际长度（获得长度小于等于用来接受的buffer长，所以可能小于实际长度）。
  Serial.print("The length of the value is :");Serial.println(getLen);
  easyflash.delValue(key);                        //根据key删除键值对
  getLen = easyflash.getValue(key,valueRec,20,valueLen);  //getValue()返回实际获得的长度，注意这里如果value已经删除，valueLen无法获得value的实际长度，所以valueLen的值不会变，传进来是多少函数运行完还是多少
  Serial.print("After delete the key&value.The length of the value is :");Serial.println(getLen);
}

void loop() {

}
