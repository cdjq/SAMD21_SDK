/*!
 * @file W25Q.cpp
 * @brief W25Q Master library for Arduino FireBeetle Board-M0. 
 * @n The W25Q series are Serial Flash memory which provides a storage solution for systems
 * with limited space,pins and power.(WQ25系列是串行flash存储器，可以为空间，引脚，功耗有限
 * 制的系统提供一个存储方案) The W25Q series offers flexibility and performance well beyond
 * ordinary Serial Flash devices.(W25Q系列的灵活性和性能比一般的串行flash设备要高)They are
 * ideal for code shadowing to RAM, executing code directly from Dual/Quad SPI and storing
 * voice, text and data.(串行flash可以实现代码映射到RAM，直接通过SPI方式执行代码，存储声音，
 * 文本，数据)
 * @n W25Q库向用户提供了一系列的接口函数，使用户可以直接对W25Q系列的存储器进行读取等操作
 * @n 该库支持的芯片系列: W25Q128
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2019-02-28
 * @
 */
#include "W25Q.h"
#include <Arduino.h>
#include <SPI.h>

bool W25QClass::begin(uint8_t csPin, uint32_t freq){
  if(!_spi) return false;
  _spi->begin();
  if(freq) _spi->beginTransaction(SPISettings(freq, MSBFIRST, SPI_MODE0));
  if(csPin != W25Q_CHIP_CS_INVAILD) _cs = csPin;
  pinMode(_cs, OUTPUT);
  return true;
}

String W25QClass::readUniqueID(){
  uint8_t temp;
  char cBuf[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
  String id = "";
  W25Q_CHIP_CS_LOW;
  _spi->transfer(W25Q_CMD_READ_ID);
  _spi->transfer(0xFF);
  _spi->transfer(0xFF);
  _spi->transfer(0xFF);
  _spi->transfer(0xFF);
  for(uint8_t i = 0; i < 8; i++){
      temp = _spi->transfer(0xFF);
      //DBG(temp,HEX);
      id += cBuf[(temp>>4) & 0x0F];
      id += cBuf[temp & 0x0F];
  }
  W25Q_CHIP_CS_HIGH;
  return id;
}
//返回值如下:
//0XEF17,表示芯片型号为W25Q128
uint16_t W25QClass::readManufacterAndDeviceID(){
  uint16_t val;
  W25Q_CHIP_CS_LOW;
  _spi->transfer(W25Q_CMD_READ_MDEVICEID);
  _spi->transfer(0x00);
  _spi->transfer(0x00);
  _spi->transfer(0x00);
  val = _spi->transfer(0xFF) << 8;
  val |= _spi->transfer(0xFF);
  W25Q_CHIP_CS_HIGH;
  return val;
}

uint8_t W25QClass::readDeviceID(){
  uint8_t val;
  W25Q_CHIP_CS_LOW;
  _spi->transfer(W25Q_CMD_READ_DEVICEID);
  _spi->transfer(0xFF);
  _spi->transfer(0xFF);
  _spi->transfer(0xFF);
  val = _spi->transfer(0xFF);
  W25Q_CHIP_CS_HIGH;
  return val;
}
uint32_t W25QClass::readFlashCapacityByte(){
  uint32_t val;
  W25Q_CHIP_CS_LOW;
  _spi->transfer(W25Q_CMD_READ_JEDEC_ID);
  _spi->transfer(0xFF);
  _spi->transfer(0xFF);
  val =pow(2,_spi->transfer(0xFF));
  W25Q_CHIP_CS_HIGH;
  return val;
}
bool W25QClass::writeEnable(eWEMode_t mode){
  uint8_t val,cmd;
  switch((uint8_t)mode){
      case eNonVolMode:
           cmd = W25Q_CMD_NON_WRITE_ENABLE;
           break;
      case eVolMode:
           cmd = W25Q_CMD_WRITE_ENABLE;
           break;
  }
  W25Q_CHIP_CS_LOW;
  _spi->transfer(cmd);
  W25Q_CHIP_CS_HIGH;
  val = readSR(eStatusReg1);
  if(((sSR1_t *)(&val))->wel) return true;
  else return false;
}

void W25QClass::writeDisable(){
  W25Q_CHIP_CS_HIGH;
  _spi->transfer(W25Q_CMD_WRITE_DISABLE);
  W25Q_CHIP_CS_LOW;
}
uint8_t W25QClass::readSR(eSR_t sr){
  uint8_t command = 0,val = 0;
  switch((uint8_t)sr){
      case eStatusReg1: 
            command = W25Q_CMD_READ_SR1;
            break;
      case eStatusReg2: 
            command = W25Q_CMD_READ_SR2;
            break;
      case eStatusReg3: 
            command = W25Q_CMD_READ_SR3;
            break;
      default:
            command = W25Q_CMD_READ_SR1;
            break;
  }
  W25Q_CHIP_CS_LOW;
  _spi->transfer(command);
  val = _spi->transfer(0xFF);
  //DBG(val);
  W25Q_CHIP_CS_HIGH;
  return val;
}
void W25QClass::writeSR(eSR_t sr, uint8_t srVal, eWEMode_t mode){
  uint8_t command = 0;
  switch((uint8_t)sr){
      case eStatusReg1: 
            command = W25Q_CMD_WRITE_SR1;
            break;
      case eStatusReg2: 
            command = W25Q_CMD_WRITE_SR2;
            break;
      case eStatusReg3: 
            command = W25Q_CMD_WRITE_SR3;
            break;
      default:
            command = W25Q_CMD_WRITE_SR1;
            break;
  }
  while(waitBusy());
  writeEnable(mode);
  W25Q_CHIP_CS_LOW;
  _spi->transfer(command);
  _spi->transfer(srVal);
  W25Q_CHIP_CS_HIGH;
}

void W25QClass::readFlash(uint32_t addr, void *pBuf, uint16_t len){
  if((addr > 0xFFFFFF)||(pBuf == NULL)||(len == 0)) return;
  uint8_t *buf = (uint8_t *)pBuf;
  while(waitBusy());
  W25Q_CHIP_CS_LOW;
  _spi->transfer(W25Q_CMD_READ_FLASH);
  _spi->transfer((uint8_t)(addr>>16));
  _spi->transfer((uint8_t)(addr>>8));
  _spi->transfer((uint8_t)(addr));
  for(uint16_t i = 0; i < len; i++){
      buf[i] = _spi->transfer(0xFF);
  }
  W25Q_CHIP_CS_HIGH;
}
void W25QClass::writeFlash(uint32_t addr, void *pBuf, uint16_t len){
  if((addr > W25Q_BLOCK_ADDR_END)||(pBuf == NULL) || (len == 0)) return;
  uint8_t *buf = (uint8_t *)pBuf;
  uint16_t left = len;
  while(left){
      left > W25Q_PAGE_SIZE ? len = W25Q_PAGE_SIZE : len = left;
      while(waitBusy());
      writeEnable(eNonVolMode);
      W25Q_CHIP_CS_LOW;
      _spi->transfer(W25Q_CMD_PAGE_PROGRAM);
      _spi->transfer((uint8_t)(addr>>16));
      _spi->transfer((uint8_t)(addr>>8));
      _spi->transfer((uint8_t)(addr));
      for(uint16_t i = 0; i < len; i++){
          _spi->transfer(buf[i]);
      }
      W25Q_CHIP_CS_HIGH;
      buf += len;
      left -= len;
      addr += len;
  }
}

void W25QClass::eraseChip(){
  while(waitBusy());
  writeEnable(eNonVolMode);
  W25Q_CHIP_CS_LOW;
  _spi->transfer(W25Q_CMD_CHIP_ERASE);
  W25Q_CHIP_CS_HIGH;
}
void W25QClass::eraseSector(uint32_t addr, bool isFirstAddr){
  if((addr > W25Q_BLOCK_ADDR_END)||(isFirstAddr && (!(addr%W25Q_SECTOR_SIZE)))) 
      return;
  while(waitBusy());
  while(!writeEnable(eNonVolMode));
  W25Q_CHIP_CS_LOW;
  _spi->transfer(W25Q_CMD_SECTOR_ERASE);
  _spi->transfer((uint8_t)(addr>>16));
  _spi->transfer((uint8_t)(addr>>8));
  _spi->transfer((uint8_t)(addr));
  W25Q_CHIP_CS_HIGH;
}
void W25QClass::eraseBlock32(uint32_t addr, bool isFirstAddr){
  if((addr > W25Q_BLOCK_ADDR_END)||(isFirstAddr && (!(addr%(W25Q_BLOCK_SIZE/2))))) 
      return;
  while(waitBusy());
  while(!writeEnable(eNonVolMode));
  W25Q_CHIP_CS_LOW;
  _spi->transfer(W25Q_CMD_BLOCK32_ERASE);
  _spi->transfer((uint8_t)(addr>>16));
  _spi->transfer((uint8_t)(addr>>8));
  _spi->transfer((uint8_t)(addr));
  W25Q_CHIP_CS_HIGH;
}
void W25QClass::eraseBlock64(uint32_t addr, bool isFirstAddr){
  if((addr > W25Q_BLOCK_ADDR_END)||(isFirstAddr && (!(addr%W25Q_BLOCK_SIZE)))) 
      return;
  while(waitBusy());
  while(!writeEnable(eNonVolMode));
  W25Q_CHIP_CS_LOW;
  _spi->transfer(W25Q_CMD_BLOCK64_ERASE);
  _spi->transfer((uint8_t)(addr>>16));
  _spi->transfer((uint8_t)(addr>>8));
  _spi->transfer((uint8_t)(addr));
  W25Q_CHIP_CS_HIGH;
}

void W25QClass::eraseOrProgramSuspend(){
  if((readSR(eStatusReg2)&0x80) || (!waitBusy())) return;
  W25Q_CHIP_CS_LOW;
  _spi->transfer(W25Q_CMD_SUSPEND);
  W25Q_CHIP_CS_HIGH;
  delay(1);
}
void W25QClass::eraseOrProgramResume(){
  if((!(readSR(eStatusReg2)&0x80)) || (waitBusy())) return;
  W25Q_CHIP_CS_LOW;
  _spi->transfer(W25Q_CMD_RESUME);
  W25Q_CHIP_CS_HIGH;
}

void W25QClass::powerDown(){
  W25Q_CHIP_CS_LOW;
  _spi->transfer(W25Q_CMD_POWERDOWN);
  W25Q_CHIP_CS_HIGH;
}

void W25QClass::releasePowerDown(){
  W25Q_CHIP_CS_LOW;
  _spi->transfer(W25Q_CMD_POWERDOWN_RELEASE);
  W25Q_CHIP_CS_HIGH;
}

void W25QClass::reset(){
  while(waitBusy());
  W25Q_CHIP_CS_LOW;
  _spi->transfer(W25Q_CMD_RESET_ENABLE);
  _spi->transfer(W25Q_CMD_RESET);
  W25Q_CHIP_CS_HIGH;
  delay(1);
}
uint32_t W25QClass::getSectorFirstAddr(uint32_t addr, uint16_t *secNum){
  if(addr > W25Q_BLOCK_ADDR_END) return addr;
  uint16_t num;
  num = addr/W25Q_SECTOR_SIZE;
  if(secNum != NULL) *secNum = num;
  return (uint32_t)(num*W25Q_SECTOR_SIZE);
}

uint32_t W25QClass::getBlock32FirstAddr(uint32_t addr, uint16_t *b32Num){
  if(addr > W25Q_BLOCK_ADDR_END) return addr;
  uint16_t num;
  num = addr/(W25Q_BLOCK_SIZE/2);
  if(b32Num != NULL) *b32Num = num;
  return (uint32_t)(num*(W25Q_BLOCK_SIZE/2));
}

uint32_t W25QClass::getBlock64FirstAddr(uint32_t addr, uint16_t *b64Num){
  if(addr > W25Q_BLOCK_ADDR_END) return addr;
  uint16_t num;
  num = addr/W25Q_BLOCK_SIZE;
  if(b64Num != NULL) *b64Num = num;
  return (uint32_t)(num*W25Q_BLOCK_SIZE);
}

bool W25QClass::waitBusy(){
  uint8_t temp;
  W25Q_CHIP_CS_LOW;
  _spi->transfer(W25Q_CMD_READ_SR1);
  temp = _spi->transfer(0xFF);
  W25Q_CHIP_CS_HIGH;
  if(temp & 0x01) return true;
  else return false;
}

W25QClass W25Q;
