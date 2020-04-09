/*!
 * @file font_display.ino
 * @brief 调用M0内置的SPIFlash显示字模
 * @n 实验现象：通过串口模拟屏幕显示自定义的str变量
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [guo](xinyu.guo@dfrobot.com)
 * @version  V1.0
 * @date  2020-02-24
 * @https:https://github.com/ghjklzx/DFRobot_Flash_Font
 */
 
#include <DFRobot_Flash_Font.h>
#include <W25Q.h>
#include <SPI.h>

DFRobot_Flash_Font font;

void drawStringMap(uint8_t *charBuf,uint8_t wid, uint8_t len,uint8_t bytePerLine ) {
  int count = 0;
  int w = 0;
  for (int i = 0; i < len; i++) {
    uint8_t mask = charBuf[i];
    int n = 8;
    if ((w + 8) > wid)
      n =wid - w;
    for (int p = 0; p < n; p++) {
      if (mask & 0x80) Serial.print("■");
      else Serial.print("□");
      mask <<= 1;
    }
    w += 8;
    count++;
    if (count == bytePerLine) {
      count = 0;
      w = 0;
      Serial.println();
    }
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  SPI.begin();
  delay(300);
  font.begin();              //读取字模初始化信息
  delay(10);
  while (!Serial);        // 等待串口打开
}

void loop() {
  //String str1= "你好世界";      
  String str1= "こんにちは世界";
  //String str1= "하이 월드";      //设置字符，可设置中英日韩文
  font.cache(str1);           //缓存str信息
  while(font.avaible()){     //判断是否打印完所有字符，未打印则继续打印下一个字符
    font.readUni();          //读取单个字符unicode
    font.getFont();          //读取单个字符的字体信息
    /*
    用户读取字模相关信息后可通过库内置的结构体进行调用
    font.charData.ucode          每个字符的unicode信息
    font.charData.charBufer[32]  每个字符的数据buffer
    font.charData.width          每个字符的宽度信息
    font.charData.lenth          每个字符的长度信息
    font.charData.bytePerLine    每个字符的数据长度信息
    */
    drawStringMap(font.charData.charBufer,font.charData.width,font.charData.lenth,font.charData.bytePerLine );
    //传入字符信息，对字符进行绘制
  }
  while(1);
}
