#include <SPI.h>

#ifndef W25QSPIFlash_H
#define W25QSPIFlash_H
class Sd2Card;
class DFRobot_CSV;

class W25QSPIFlash
{
public:
  W25QSPIFlash():_csPin(32) {}
private:
  uint8_t _csPin;
  void readUniqueID(uint8_t *recData);
  uint8_t w25QBusy(void);
  void w25QRead(uint32_t address,uint8_t *data,uint16_t len);
  void writeEnable(void);
  void eraseSector(uint32_t address);
  void w25QWrite(uint32_t address,const uint8_t *data,uint16_t left);
  void setCSPin(uint8_t pinNum);
  friend class Sd2Card;
  friend class DFRobot_CSV; 
};

extern W25QSPIFlash SPIFlash;
#endif