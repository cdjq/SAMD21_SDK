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
uint8_t valueRec[12];
uint8_t valueRec1[12];
size_t size;
size_t getLen;

void setup()
{
  Serial.begin(115200);
  delay(5000);                                     //等待串口稳定
  easyflash.begin();                        //初始化easyflash
  easyflash.setValue(key1,value1,7);
  easyflash.setValue(key2,value2);
  easyflash.setValue(key3,value3);
  easyflash.setValue(key4,value4);
  easyflash.setValue(key5,value5);
  easyflash.setValue(key6,value6,10);          //value6不是字符串，必须传入value6的字节数，为10
  easyflash.setValue(key7,value7,8);           //value7不是字符串，必须传入value7字节数，value7是uint32_t型数组，所以长度是8
  
  getLen = easyflash.getValue(key7,valueRec,12,size);       //获得key7对应的value值，将它存在valueRec中，第三个参数为valueRec的长度，将value的真正长度存在size中，函数返回实际获得的字节数（实际获得可能小于value真正长度，取决于valueRec长度）
                                                           //有一点需要注意，如果键值对不存在，则调用这个函数时，无法获得value的真实长度，这种情况下第四个参数size值不会被这个函数改变
  Serial.print("value7: ");
  for(uint8_t i=0;i<getLen;i++) {                               //这里value7占8字节，valueRec占12字节，只获得8字节
    Serial.print(valueRec[i],HEX);
    Serial.print(" ");
  }
  Serial.print("The value's length is: ");
  Serial.println(size);

  getLen = easyflash.getValue(key7,valueRec1,12);     //也可以不传第4个参数，这样不会获得value实际长度，函数返回实际获得长度
  Serial.print("value7: ");
    for(uint8_t i=0;i<getLen;i++) {                               //这里value7占8字节，valueRec占12字节，只获得8字节
    Serial.print(valueRec[i],HEX);
    Serial.print(" ");
  }
  Serial.println();


  String str = easyflash.getValue(key5);                        //当传一个参数时，函数将value储存在string对象返回
                                   //  使用含一个参数getValue的前提是，key对应的value所有字节必须是字符型数据，key5对应的value5是字符串所以可以用，但key7对应的value7是数字所以不能用这个函数
  Serial.print("value5: ");
  Serial.println(str);
}

void loop() {

}