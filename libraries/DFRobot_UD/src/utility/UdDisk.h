/* Arduino UdDisk Library
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
 * along with the Arduino UdDiskLibrary.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#ifndef __UDDISK_H
#define __UDDISK_H
#ifdef ARDUINO_SAM_ZERO
#include <variant.h>
#endif

#define SPI_FLASH_SS_PIN 32
#define UD_PROTECT_BLOCK_ZERO 1
//UD Chip Select Pin
namespace UDLib{
uint8_t const  UD_CHIP_SELECT_PIN = SPI_FLASH_SS_PIN;

class UdDisk {
 public:
  /**
   * @brief Construct an instance of UdDisk
   */
  UdDisk(void) :inBlock_(0){}
  /**
   * @brief get size of USB Disk,unit Byte.
   * @return Successfully returns a non-zero integer.
   */
  uint32_t diskSizeByte(void);
  /**
   * @brief Erase a range of blocks. The size of blocks is 512 Bytes.
   * @param firstBlock The address of the first block in the range.
   * @param lastBlock The address of the last block in the range.
   * @return Successfully returns a non-zero integer.
   */
  uint8_t erase(uint32_t firstBlock, uint32_t lastBlock);
  /**
   * @brief Initialize a USB Disk with the default UD chip select pin.
   * @return Successfully returns a non-zero integer.
   */
  uint8_t init(void) {
    return init(UD_CHIP_SELECT_PIN);
  }
  /**
   * Initialize an SD flash memory card with the selected SPI clock rate
   * and the default SD chip select pin.
   * See sd2Card::init(uint8_t sckRateID, uint8_t chipSelectPin).
   */
  /**
   * @brief Initialize a USB Disk with the selected SPI clock rate
   * @n and the default UD chip select pin.
   * @param chipSelectPin UD chip select pin number.
   * @param freq SPI clock rate
   * @return Successfully returns a non-zero integer.
   */
  uint8_t init(uint8_t chipSelectPin, uint32_t freq = 0);
  /**
   * @brief Read part of a 512 byte block from a USB Disk.
   * @param block Logical block number.
   * @param dst Pointer to the location that will receive the data.
   * @return Successfully returns a non-zero integer.
   */
  uint8_t readBlock(uint32_t block, uint8_t* dst);
  /**
   * @brief Read part of a 512 byte block from a USB Disk.
   * @param block block Logical block to be read.
   * @param offset Number of bytes to skip at start of block.
   * @param count Number of bytes to read.
   * @param dst Pointer to the location that will receive the data.
   * @return Successfully returns a non-zero integer.
   */
  uint8_t readData(uint32_t block, uint16_t offset, uint16_t count, uint8_t* dst);
  /**
   * @brief Writes a 512 byte block to a USB Disk.
   * @param blockNumber Logical block to be written.
   * @param src Pointer to the location of the data to be written.
   * @return Successfully returns a non-zero integer.
   */
  uint8_t writeBlock(uint32_t blockNumber, const uint8_t* src);

 private:
  static uint8_t flashtemp[4096]; 
  uint32_t block_;
  uint8_t inBlock_;
  uint16_t offset_;
  /**
   * @brief wait for UD to go not busy.
   * @param timeoutMillis Delay time 
   * @return Returns true if UD is busy, otherwise returns false
   */
  uint8_t waitNotBusy(unsigned int timeoutMillis);
};
};
#endif
