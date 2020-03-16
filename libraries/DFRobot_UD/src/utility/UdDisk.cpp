/* Arduino UDisk Library
 * Copyright (C) 2009 by William Greiman
 *
 * This file is part of the Arduino UdDisk Library
 *
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Arduino UdDisk Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include <Arduino.h>
#include "UdDisk.h"
#include <W25Q.h>

namespace UDLib{
uint32_t UdDisk::diskSizeByte(void) {
  return 12*1024*1024;
}
static uint8_t chip_select_asserted = 0;

uint8_t UdDisk::flashtemp[4096] = {0};

uint8_t UdDisk::erase(uint32_t firstBlock, uint32_t lastBlock){
  uint32_t firstS = (firstBlock/8)*8;
  uint32_t lastS = (lastBlock/8)*8;
  while(lastS >= firstS) {
    if(lastS == firstS) { 
        if (firstBlock-firstS) 
          W25Q.readFlash(firstS<<9, flashtemp, (firstBlock-firstS)<<9);
        if ((firstS+8)-(lastBlock+1))
          W25Q.readFlash((lastBlock+1)<<9, flashtemp+((firstBlock-firstS)<<9), ((firstS+8)-(lastBlock+1))<<9);
    } else {
        if (firstBlock-firstS)
          W25Q.readFlash(firstS<<9, flashtemp, (firstBlock-firstS)<<9);
    }     
    W25Q.eraseSector(firstS<<9,false);
    if (lastS == firstS) { 
        if (firstBlock-firstS) 
          W25Q.writeFlash(firstS<<9, flashtemp, (firstBlock-firstS)<<9);       
        if ((firstS+8)-(lastBlock+1)) 
          W25Q.writeFlash((lastBlock+1)<<9, flashtemp+((firstBlock-firstS)<<9), ((firstS+8)-(lastBlock+1))<<9);        
    } else {
        if (firstBlock-firstS)
          W25Q.writeFlash(firstS<<9, flashtemp, (firstBlock-firstS)<<9);
    }  
    firstS += 8;
    firstBlock = firstS;
  }
  return true;
}

uint8_t UdDisk::init(uint8_t chipSelectPin,uint32_t freq) {
  inBlock_ = 0;
  return W25Q.begin(chipSelectPin,freq);
}

uint8_t UdDisk::readBlock(uint32_t block, uint8_t* dst) {
  return readData(block, 0, 512, dst);
}

uint8_t UdDisk::readData(uint32_t block,
        uint16_t offset, uint16_t count, uint8_t* dst) {
  if (count == 0) return true;
  if ((count + offset) > 512) {
    return false;
  }
  if (!inBlock_ || block != block_ || offset < offset_) {
    block_ = block;
    offset_ = 0;
    inBlock_ = 1;
  }
  W25Q.readFlash((block<<9)+offset, (uint8_t *)dst, count);
  offset_ += count;
  return true;
}

uint8_t UdDisk::waitNotBusy(unsigned int timeoutMillis) {
  unsigned int t0 = millis();
  unsigned int d;
  do {
    if(!W25Q.waitBusy()) return true;
    d = millis() - t0;
  }while (d < timeoutMillis);
  return false;
}

uint8_t UdDisk::writeBlock(uint32_t blockNumber, const uint8_t* src) {
#if UD_PROTECT_BLOCK_ZERO
  if (blockNumber == 0) {
    return false;
  }
#endif
  W25Q.readFlash(blockNumber<<9, flashtemp, 512);
  for(uint16_t i=0;i<512;i++) {
      if(flashtemp[i]!=0xFF) {
          W25Q.readFlash((blockNumber/8*8)<<9,flashtemp,4096);
          memset(flashtemp+((blockNumber%8)<<9),0xFF,512);
          W25Q.eraseSector((blockNumber/8*8)<<9,false);
          W25Q.writeFlash((blockNumber/8*8)<<9,flashtemp,4096);
          break;
      }
  }
  W25Q.writeFlash(blockNumber<<9, (uint8_t *)src, 512);
  return true;
}
};
