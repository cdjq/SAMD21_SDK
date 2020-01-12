/*!
 * @file setAndGetValue.ino
 * @brief 设置多个键值对并通过key得到value
 * @n 实验现象：串口监视器打印出得到的value值
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

const char key1[] = "one";                        //key必须是字符或字符串，不大于32字节
const char value1[] = "Monday";                   //value不能大于4010字节
char key2[] = "two";                               //key不是const类型也可以
char value2[] = "Tuesday";                         //value不是const类型也可以
char key3[] = "three";
String value3 = "Wednesday";
String key4 = "four";
String value4 = "Thursday";
String key5 = "five";
char value5[] = "Friday";

char key6[] = "six";           
uint8_t value6[10] = {255,128,64,32,16,8,4,2,1,0};    
char key7[] = "seven";
uint32_t value7[2] = {0x01020304,0x0A0B0C0D};
uint32_t valueRec[3];
size_t size;

void setup()
{
  SerialUSB.begin(115200);
  easyflash.begin();                        //初始化easyflash
  easyflash.setValue(key1,value1,7);
  easyflash.setValue(key2,value2);
  easyflash.setValue(key3,value3);
  easyflash.setValue(key4,value4);
  easyflash.setValue(key5,value5);
  easyflash.setValue(key6,value6,10);          //value6不是字符串，必须传入value6的字节数，为10
  easyflash.setValue(key7,value7,8);           //value7不是字符串，必须传入value7字节数，为8
  easyflash.getValue(key7,valueRec,12,size)        //获得key7对应的value值，将它存在valueRec中，第三个参数为valueRec的所占12字节，将value的实际字节数存在size中
  SerialUSB.print("Value for key7 is: ");          //value和valueRec中所占字节数少的那个，占的字节数个数据将被得到
  for(i=0;i<3;i++) {                               //这里value7占8字节，valueRec占12字节，只获得8字节
    SerialUSB.print(valueRec[i],HEX);
    SerialUSB.print(" ");
  }
  SerialUSB.println();
  SerialUSB.print("The value's length is: ");
  SerialUSB.println(size);
}

void loop() {

}
