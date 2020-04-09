/*!
 * @file DFRobot_Flash_Font.h
 * @brief Flash内置字体库
 * @n 这是一个Flash内置字体使用库
 * @n 可调用Flash内置的字体，显示中英日韩文字
 * @copyright	Copyright (c) 2020 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [guo](xinyu.guo@dfrobot.com)
 * @version  V1.0
 * @date  2020-02-24
 * @https://github.com/ghjklzx/DFRobot_Flash_Font
 */
#ifndef __DFRobot_Flash_Font_H
#define __DFRobot_Flash_Font_H

#include "Arduino.h"

//#define ENABLE_DBG
#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif

#define HEAD_ADDRESS 0xc00000//SPIflash首地址

#define CS 32//SPIflash片选引脚
#define FONT_INFO_BYTES                18//字模信息数据长度
#define CHAR_ADDRESS_AND_BYTES_LEN     6//字模信息数据长度
#define CHAR_WIGTH_AND_BYTE_PER_LINE   4//字模信息数据长度
#endif

class DFRobot_Flash_Font
{
public:
  typedef struct {
    char title[4];//
    uint16_t height;//字体高度
    uint16_t width;//字体宽度
    uint16_t baseline;//数据长度
    uint16_t xHeight;//字模x轴
    uint16_t yHeight;//字模y轴
    uint16_t firstChar;//0
    uint16_t lastChar; //65535
    } __attribute__ ((packed)) uniInfo_t;//所有字符的信息

  typedef struct {
    uint32_t ptrCharData;
    uint16_t len;//字模数据长度
    } __attribute__ ((packed)) charInfo_t;//字符信息

  typedef struct {
    uint16_t width;
    uint16_t bytePerLine;
    } __attribute__ ((packed)) charSpec_t;//字符信息
	
  typedef struct {
    uint32_t ucode;
    uint8_t charBufer[32];
    uint8_t width;
    uint8_t lenth;
    uint8_t bytePerLine;
    } charData_t; //用户使用的字模信息

  charData_t charData; //提供给用户字模数据
  uniInfo_t uniInfo;//字模的info信息
  uint32_t transSize = 0;

public:
  /**
   * @brief 构造函数
   * @param 
   */
  DFRobot_Flash_Font();
  
  /**
   * @brief 析构函数
   * @param 释放_utf8字符数据
   */
  ~DFRobot_Flash_Font();

  /**
   * @brief 初始化
   * @param 设置flash，并读取字模头部信息
   */
  void begin(void);

  /**
   * @brief 缓存字符信息
   * @param 
   */
  void cache(String &s);

  /**
   * @brief 判断字模是否输出完成
   * @param  
   */
  bool avaible(void);
  
  /**
   * @brief 读取unicode
   * @param 
   */
  void readUni();
  
  /**
   * @brief 获得字模信息
   * @param 
   */
  bool getFont();

  /**
   * @brief 在打印窗口打印字模
   * @param 
   */
  void drawStringMap();

  /**
   * @brief 擦除字模空间
   * @param 
   */
  bool eraseSpace();


private: 
  uint8_t *_utf8 =NULL;
  uint16_t _index=0;
  uint8_t _len=0;
};
