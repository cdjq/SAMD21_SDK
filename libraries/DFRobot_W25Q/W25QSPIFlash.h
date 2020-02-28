#include <SPI.h>

#ifndef W25QSPIFlash_H
#define W25QSPIFlash_H

class W25QX
{
public:
  W25QX():_csPin(32) {}
  void read(uint32_t address,void *data,uint16_t len);
  void eraseSector(uint32_t address);
  void write(uint32_t address,const void *data,uint16_t left);
  void setCSPin(uint8_t pinNum);
private:
  uint8_t _csPin;
  uint8_t w25QBusy(void);
  void readUniqueID(void *recData);
  void writeEnable(void);
};

extern W25QX SPIFlash;
#endif