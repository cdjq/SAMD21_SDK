/* Arduino Sd2Card Library
 * Copyright (C) 2009 by William Greiman
 *
 * This file is part of the Arduino Sd2Card Library
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
 * along with the Arduino Sd2Card Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#define USE_SPI_LIB
#include <Arduino.h>
#include "2Card.h"
//------------------------------------------------------------------------------


#define SDCARD_SPI SPI
#include <SPI.h>
#include <W25QSPIFlash.h>
static SPISettings settings;

// functions for hardware SPI
/** Send a byte to the card */
static void spiSend(uint8_t b) {
  SDCARD_SPI.transfer(b);
}
/** Receive a byte from the card */
static  uint8_t spiRec(void) {
  return SDCARD_SPI.transfer(0xFF);
}

//------------------------------------------------------------------------------
// send command and return error code.  Return zero for OK
uint8_t Sd2Card::cardCommand(uint8_t cmd, uint32_t arg) {
  // end read if in partialBlockRead mode
  readEnd();

  // select card
  chipSelectLow();

  // wait up to 300 ms if busy
  waitNotBusy(300);

  // send command
  spiSend(cmd | 0x40);

  // send argument
  for (int8_t s = 24; s >= 0; s -= 8) spiSend(arg >> s);

  // send CRC
  uint8_t crc = 0XFF;
  if (cmd == CMD0) crc = 0X95;  // correct crc for CMD0 with arg 0
  if (cmd == CMD8) crc = 0X87;  // correct crc for CMD8 with arg 0X1AA
  spiSend(crc);

  // wait for response
  for (uint8_t i = 0; ((status_ = spiRec()) & 0X80) && i != 0XFF; i++)
    ;
  return status_;
}
//------------------------------------------------------------------------------
/**
 * Determine the size of an SD flash memory card.
 *
 * \return The number of 512 byte data blocks in the card
 *         or zero if an error occurs.
 */
uint32_t Sd2Card::cardSize(void) {
  csd_t csd;
  if (!readCSD(&csd)) return 0;
  if (csd.v1.csd_ver == 0) {
    uint8_t read_bl_len = csd.v1.read_bl_len;
    uint16_t c_size = (csd.v1.c_size_high << 10)
                      | (csd.v1.c_size_mid << 2) | csd.v1.c_size_low;
    uint8_t c_size_mult = (csd.v1.c_size_mult_high << 1)
                          | csd.v1.c_size_mult_low;
    return (uint32_t)(c_size + 1) << (c_size_mult + read_bl_len - 7);
  } else if (csd.v2.csd_ver == 1) {
    uint32_t c_size = ((uint32_t)csd.v2.c_size_high << 16)
                      | (csd.v2.c_size_mid << 8) | csd.v2.c_size_low;
    return (c_size + 1) << 10;
  } else {
    error(SD_CARD_ERROR_BAD_CSD);
    return 0;
  }
}
//------------------------------------------------------------------------------
static uint8_t chip_select_asserted = 0;

void Sd2Card::chipSelectHigh(void) {
  digitalWrite(chipSelectPin_, HIGH);
/*#ifdef USE_SPI_LIB
  if (chip_select_asserted) {
    chip_select_asserted = 0;
    SDCARD_SPI.endTransaction();
  }
#endif*/
}
//------------------------------------------------------------------------------
void Sd2Card::chipSelectLow(void) {
/*#ifdef USE_SPI_LIB
  if (!chip_select_asserted) {
    chip_select_asserted = 1;
    SDCARD_SPI.beginTransaction(settings);
  }
#endif*/
  digitalWrite(chipSelectPin_, LOW);
}
//------------------------------------------------------------------------------
/** Erase a range of blocks.
 *
 * \param[in] firstBlock The address of the first block in the range.
 * \param[in] lastBlock The address of the last block in the range.
 *
 * \note This function requests the SD card to do a flash erase for a
 * range of blocks.  The data on the card after an erase operation is
 * either 0 or 1, depends on the card vendor.  The card must support
 * single block erase.
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 */
 
uint8_t Sd2Card::flashtemp[4096] = {0};

uint8_t Sd2Card::erase(uint32_t firstBlock, uint32_t lastBlock) {
 // memset(flashtemp,0xFF,4096);
  uint32_t firstS = (firstBlock/8)*8;
  uint32_t lastS = (lastBlock/8)*8;
  while(lastS >= firstS) {
	if(lastS == firstS) { 
		if (firstBlock-firstS) 
		  SPIFlash.w25QRead(firstS<<9, flashtemp, (firstBlock-firstS)<<9);
	    if ((firstS+8)-(lastBlock+1))
	      SPIFlash.w25QRead((lastBlock+1)<<9, flashtemp+((firstBlock-firstS)<<9), ((firstS+8)-(lastBlock+1))<<9);
	} else {
		if (firstBlock-firstS)
	      SPIFlash.w25QRead(firstS<<9, flashtemp, (firstBlock-firstS)<<9);
    }	  
	SPIFlash.eraseSector(firstS<<9);
	if (lastS == firstS) { 
		if (firstBlock-firstS) 
	      SPIFlash.w25QWrite(firstS<<9, flashtemp, (firstBlock-firstS)<<9);	    
	    if ((firstS+8)-(lastBlock+1)) 
	      SPIFlash.w25QRead((lastBlock+1)<<9, flashtemp+((firstBlock-firstS)<<9), ((firstS+8)-(lastBlock+1))<<9);		
	} else {
		if (firstBlock-firstS)
	      SPIFlash.w25QRead(firstS<<9, flashtemp, (firstBlock-firstS)<<9);
	}  
	firstS += 8;
	firstBlock = firstS;
  }
  return true;
}
//------------------------------------------------------------------------------
/** Determine if card supports single block erase.
 *
 * \return The value one, true, is returned if single block erase is supported.
 * The value zero, false, is returned if single block erase is not supported.
 */
uint8_t Sd2Card::eraseSingleBlockEnable(void) {
 // csd_t csd;
  return 1;//readCSD(&csd) ? csd.v1.erase_blk_en : 0;
}
//------------------------------------------------------------------------------
/**
 * Initialize an SD flash memory card.
 *
 * \param[in] sckRateID SPI clock rate selector. See setSckRate().
 * \param[in] chipSelectPin SD chip select pin number.
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.  The reason for failure
 * can be determined by calling errorCode() and errorData().
 */
uint8_t Sd2Card::init(uint8_t sckRateID, uint8_t chipSelectPin) {
  errorCode_ = inBlock_ = partialBlockRead_ = type_ = 0;
  chipSelectPin_ = chipSelectPin;
  pinMode(chipSelectPin_, OUTPUT);
  digitalWrite(chipSelectPin_, HIGH);
  SDCARD_SPI.begin();
  type_ = 1;
  return true;
}
//------------------------------------------------------------------------------
/**
 * Enable or disable partial block reads.
 *
 * Enabling partial block reads improves performance by allowing a block
 * to be read over the SPI bus as several sub-blocks.  Errors may occur
 * if the time between reads is too long since the SD card may timeout.
 * The SPI SS line will be held low until the entire block is read or
 * readEnd() is called.
 *
 * Use this for applications like the Adafruit Wave Shield.
 *
 * \param[in] value The value TRUE (non-zero) or FALSE (zero).)
 */
void Sd2Card::partialBlockRead(uint8_t value) {
//  readEnd();
  partialBlockRead_ = value;
}
//------------------------------------------------------------------------------
/**
 * Read a 512 byte block from an SD card device.
 *
 * \param[in] block Logical block to be read.
 * \param[out] dst Pointer to the location that will receive the data.

 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 */
uint8_t Sd2Card::readBlock(uint32_t block, uint8_t* dst) {
  return readData(block, 0, 512, dst);
}
//------------------------------------------------------------------------------
/**
 * Read part of a 512 byte block from an SD card.
 *
 * \param[in] block Logical block to be read.
 * \param[in] offset Number of bytes to skip at start of block
 * \param[out] dst Pointer to the location that will receive the data.
 * \param[in] count Number of bytes to read
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 */
uint8_t Sd2Card::readData(uint32_t block,
        uint16_t offset, uint16_t count, uint8_t* dst) {
  if (count == 0) return true;
  if ((count + offset) > 512) {
    goto fail;
  }
  if (!inBlock_ || block != block_ || offset < offset_) {
    block_ = block;
    offset_ = 0;
    inBlock_ = 1;
  }
   SPIFlash.w25QRead((block<<9)+offset, dst, count);
  offset_ += count;
  return true;

 fail:
  chipSelectHigh();
  return false;
}

uint8_t Sd2Card::readRegister(uint8_t cmd, void* buf) {

  return true;

}

#ifdef USE_SPI_LIB
//------------------------------------------------------------------------------
// set the SPI clock frequency
uint8_t Sd2Card::setSpiClock(uint32_t clock)
{
  settings = SPISettings(clock, MSBFIRST, SPI_MODE0);
  return true;
}
#endif
//------------------------------------------------------------------------------
// wait for card to go not busy
uint8_t Sd2Card::waitNotBusy(unsigned int timeoutMillis) {
  unsigned int t0 = millis();
  unsigned int d;
  do {
    if( SPIFlash.w25QBusy() == 0) return true;
    d = millis() - t0;
  }
  while (d < timeoutMillis);
  return false;
}
//------------------------------------------------------------------------------
/** Wait for start block token */
uint8_t Sd2Card::waitStartBlock(void) {
  return true;
}
//------------------------------------------------------------------------------
/**
 * Writes a 512 byte block to an SD card.
 *
 * \param[in] blockNumber Logical block to be written.
 * \param[in] src Pointer to the location of the data to be written.
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 */
uint8_t Sd2Card::writeBlock(uint32_t blockNumber, const uint8_t* src) {
#if SD_PROTECT_BLOCK_ZERO
  // don't allow write to first block
  if (blockNumber == 0) {
    error(SD_CARD_ERROR_WRITE_BLOCK_ZERO);
    goto fail;
  }
#endif  // SD_PROTECT_BLOCK_ZERO

  SPIFlash.w25QRead(blockNumber<<9,flashtemp,512);
  for(uint16_t i=0;i<512;i++) {
	  if(flashtemp[i]!=0xFF) {
		  SPIFlash.w25QRead((blockNumber/8*8)<<9,flashtemp,4096);
		  memset(flashtemp+((blockNumber%8)<<9),0xFF,512);
		  SPIFlash.eraseSector((blockNumber/8*8)<<9);
		  SPIFlash.w25QWrite((blockNumber/8*8)<<9,flashtemp,4096);
		  break;
	  }
  }
  SPIFlash.w25QWrite(blockNumber<<9, src, 512);
  return true;

 fail:
  return false;
}

//------------------------------------------------------------------------------
// send one block of data for write block or write multiple blocks
uint8_t Sd2Card::writeData(uint8_t token, const uint8_t* src) {

  for (uint16_t i = 0; i < 512; i++) {
    spiSend(src[i]);
  }
  return true;
}
//------------------------------------------------------------------------------
/** Start a write multiple blocks sequence.
 *
 * \param[in] blockNumber Address of first block in sequence.
 * \param[in] eraseCount The number of blocks to be pre-erased.
 *
 * \note This function is used with writeData() and writeStop()
 * for optimized multiple block writes.
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 */
uint8_t Sd2Card::writeStart(uint32_t blockNumber, uint32_t eraseCount) {
#if SD_PROTECT_BLOCK_ZERO
  // don't allow write to first block
  if (blockNumber == 0) {
    error(SD_CARD_ERROR_WRITE_BLOCK_ZERO);
    goto fail;
  }
#endif  // SD_PROTECT_BLOCK_ZERO
  erase(blockNumber,blockNumber+eraseCount-1);
  return true;

 fail:
  chipSelectHigh();
  return false;
}
//------------------------------------------------------------------------------
/** End a write multiple blocks sequence.
 *
* \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.
 */
uint8_t Sd2Card::writeStop(void) {
  return true;

}


