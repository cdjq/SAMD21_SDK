#include <W25QSPIFlash.h>
#define CS_H  digitalWrite(_csPin, HIGH)
#define CS_L  digitalWrite(_csPin, LOW)
void W25QSPIFlash::readUniqueID(uint8_t *recData)
{
  while(w25QBusy());
  CS_L;
  SPI.transfer(0x4B);
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  for(int i = 0; i < 8; i++){
    *recData = SPI.transfer(0xFF);
    recData++;
  }
  CS_H;
}
uint8_t W25QSPIFlash::w25QBusy(void) //判断W25Q16是否繁忙函数 繁忙则返回1
{
    uint8_t flag;
    CS_L;
    SPI.transfer(0x05);
    flag=SPI.transfer(0xFF);
    CS_H;
    flag&=0x01;
    return flag;
}

void W25QSPIFlash::w25QRead(uint32_t address,uint8_t *data,uint16_t len)//从W25Q16中的address地址上读取 j个字节的数据保存到 以data为首地址的内存中
{
    uint16_t i;
    while(w25QBusy());
    CS_L;
    SPI.transfer(0x03);
    SPI.transfer(address>>16);
    SPI.transfer(address>>8);
    SPI.transfer(address);
    for(i=0;i<len;i++)
    {
        *(data+i)=SPI.transfer(0xFF);
    }
    CS_H;
}
void W25QSPIFlash::writeEnable(void) //写使能函数 对W25Q16进行写操作之前要进行这一步操作
{
    CS_L;
    SPI.transfer(0x06);
    CS_H;
}

void W25QSPIFlash::eraseSector(uint32_t address)
{
    while(w25QBusy());
    writeEnable();                              
    CS_L;                                              //置cs低选中
    SPI.transfer(0x20);
    SPI.transfer(address>>16);
    SPI.transfer(address>>8);
    SPI.transfer(address);
    CS_H;
  
}


void W25QSPIFlash::w25QWrite(uint32_t address,const uint8_t *data,uint16_t left)
{
    uint16_t max;
	while(left) {
      while(w25QBusy());//如果芯片繁忙就等在这里
  //    EraseSector(address);
      writeEnable();//要先写入允许命令
      CS_L;
      SPI.transfer(0x02);
      SPI.transfer(address>>16);
      SPI.transfer(address>>8);
      SPI.transfer(address);
	  if(left>256)
		max = 256;
	  else 
		max = left;
      for(uint16_t i=0;i<max;i++) {
        SPI.transfer(*(data+i));
      }
      CS_H;
	  address += max;
	  data += max;
	  left -= max;
	}  
}


void W25QSPIFlash::setCSPin(uint8_t pinNum)
{
	_csPin = pinNum;
}

W25QSPIFlash SPIFlash;