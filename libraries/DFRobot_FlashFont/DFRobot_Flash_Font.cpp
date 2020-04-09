/*!
 * @file DFRobot_Flash_Font.h
 * @brief DFRobot_Flash_Font类的基础结构，基础方法的实现
 * @copyright	Copyright (c) 2020 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [guo](xinyu.guo@dfrobot.com)
 * @version  V1.0
 * @date  2020-02-24
 * @https:https://github.com/ghjklzx/DFRobot_Flash_Font
 */
#include <DFRobot_Flash_Font.h>
#include <W25Q.h>

DFRobot_Flash_Font::DFRobot_Flash_Font() {
}

DFRobot_Flash_Font::~DFRobot_Flash_Font(void) {
  free (_utf8);
}

void DFRobot_Flash_Font::begin(void){
  pinMode(CS, OUTPUT); //设置SPI FLASH的片选引脚为输出模式
  W25Q.readFlash( HEAD_ADDRESS, &uniInfo , FONT_INFO_BYTES );//读取字模文件头部信息
  DBG("uninfo=");
  DBG(uniInfo.lastChar);
}

void DFRobot_Flash_Font::cache(String &s){
  if (_utf8!=NULL){
    free (_utf8);
	_utf8=NULL;
  }
  _len=s.length();
  _utf8 = (uint8_t *)malloc(_len+1);
  if(_utf8==NULL){
  DBG("no memory");
   return;
  }
  for(int i=0;i<_len;i++){
    _utf8[i]= s[i];
  }
  DBG("_len=");
  DBG(_len);
}

void DFRobot_Flash_Font::readUni(){
  uint32_t uni=0;
  if ( _utf8[_index] == 0 || _utf8[_index] == '\n' )  /* '\n' terminates the string to support the string list procedures */{
    uni=0xff;
    _index++;
    DBG("index")DBG(_index);
    DBG("uni");DBG(uni);
    charData.ucode=uni;	
  } 
  
  uint8_t utf8State = 0;
  if(_utf8[_index] >= 0xfc){
      utf8State = 5;
      uni = _utf8[_index]&1;
      _index++;
      for(uint8_t i=1;i<=5;i++){
        uni <<= 6;
        uni |= (_utf8[_index]&0x3f);
        utf8State--;
        _index++;
      }
      DBG("index")DBG(_index);
      DBG("uni");DBG(uni);
      charData.ucode=uni;
    }else if(_utf8[_index] >= 0xf8){
      utf8State = 4;
      uni = _utf8[_index]&3;
      _index++;
      for(uint8_t i=1;i<=4;i++){
        uni <<= 6;
        uni |= (_utf8[_index]& 0x03f);
        utf8State--;
        _index++;
      }
      DBG("index")DBG(_index);
      DBG("uni");DBG(uni);
      charData.ucode=uni;
      }else if(_utf8[_index] >= 0xf0){
        utf8State = 3;
        uni = _utf8[_index]&7;
        _index++;
        for(uint8_t i=1;i<=3;i++){
        uni <<= 6;
        uni |= (_utf8[_index]& 0x03f);
        utf8State--;
        _index++;
        DBG("index")DBG(_index);
        DBG("uni");DBG(uni);
      }
      DBG(_index);DBG(uni);
      charData.ucode=uni;
	  }else if(_utf8[_index] >= 0xe0){
        utf8State = 2;
        DBG("index=");DBG(_index);
        uni = _utf8[_index]&15;
        _index++;
        DBG("uni=");DBG(uni);
        for(uint8_t i=1;i<=2;i++){
        uni <<= 6;
        uni |= (_utf8[_index]&0x03f);
        utf8State--;
        _index++;
        DBG("uni=");DBG(uni);
        DBG("index=");DBG(_index);
      }
        DBG(_index);DBG(uni);
        charData.ucode=uni;	
      }else if(_utf8[_index] >= 0xc0){
        utf8State = 1;
        uni = _utf8[_index]&0x1f;
        _index++;
        for(uint8_t i=1;i<=1;i++){
        uni <<= 6;
        uni |= (_utf8[_index]& 0x03f);
        utf8State--;
        _index++;
        DBG("index")DBG(_index);
        DBG("uni");DBG(uni);
      }
      DBG(_index);DBG(uni);
      charData.ucode=uni;
      }else if(_utf8[_index] <=0x80){
        uni |= (_utf8[_index]&0x7f);
        _index++;
        DBG("index")DBG(_index);
        DBG("uni");DBG(uni);
        charData.ucode=uni;
      }
}

bool DFRobot_Flash_Font::avaible(){
  DBG("_len=");DBG(_len);
  DBG("_index=");DBG(_index);
  DBG("_utf8[_index]=");DBG(_utf8[_index]);
  return !(_len ==_index);
}

bool DFRobot_Flash_Font::getFont(){
  DBG("getfont");
  uint32_t address;
  if (charData.ucode > (uniInfo.lastChar) || charData.ucode < (uniInfo.firstChar))
  return false;
  address = HEAD_ADDRESS + FONT_INFO_BYTES + charData.ucode * 6;
  DBG();DBG(address);
  charInfo_t charInfo;
  W25Q.readFlash(address, &charInfo, CHAR_ADDRESS_AND_BYTES_LEN);
  DBG(charInfo.ptrCharData);
  delay(10);
  if (charInfo.ptrCharData == 0 || charInfo.len == 0)
  return false;
  charSpec_t charSpec;
  address = (charInfo.ptrCharData) + HEAD_ADDRESS;
  W25Q.readFlash(address, &charSpec, CHAR_WIGTH_AND_BYTE_PER_LINE);
  delay(10);
  uint8_t charBufLen = (charInfo.len) - CHAR_WIGTH_AND_BYTE_PER_LINE;
  address = address + CHAR_WIGTH_AND_BYTE_PER_LINE;
  W25Q.readFlash(address, &charData.charBufer, charBufLen);
  //判断是否成功赋值
  DBG(charSpec.width);
  charData.width = charSpec.width;
  charData.lenth = charBufLen;
  charData.bytePerLine = charSpec.bytePerLine;
  return true;
}

void DFRobot_Flash_Font::drawStringMap() {
  int count = 0;
  int w = 0;
  for (int i = 0; i < charData.lenth; i++) {
    uint8_t mask = charData.charBufer[i];
    int n = 8;
    if ((w + 8) > charData.width)
    n =charData.width - w;
    for (int p = 0; p < n; p++) {
      if (mask & 0x80) Serial.print("■");
      else
        Serial.print("□");
        mask <<= 1;
    }
      w += 8;
      count++;
      if (count == charData.bytePerLine){
        count = 0;
        w = 0;
        Serial.println();
      }
  }
  Serial.println();
}

bool DFRobot_Flash_Font::eraseSpace(){
  uint32_t address = HEAD_ADDRESS;
  int count = 0;
  for (; count < 32; count++) {
    W25Q.eraseBlock64(address);
    address += 4096;
  }
  return count == 32;
}

