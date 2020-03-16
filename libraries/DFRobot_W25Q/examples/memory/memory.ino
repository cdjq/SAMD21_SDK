/*!
 * @file memory.ino
 * @brief 擦除、读取、写入等内存操作
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2019-03-5
 * @get from https://www.dfrobot.com
 * @url https://github.com/cdjq/W25Q
 */

#include <W25Q.h>

uint32_t addr = 0xFFFFFE;
uint32_t secFirstAddr, blockFirstAddr,block32FirstAddr;
uint16_t secNumber, blockNumber;
void setup() {
  SerialUSB.begin(115200);
  SerialUSB.print("Initializating W25Q device...");
  while(!W25Q.begin()){
    SerialUSB.println("initialization failed!");
  }
  while(!SerialUSB.available());
  SerialUSB.read();SerialUSB.read();SerialUSB.read();
  SerialUSB.println("initialization done!");
  secFirstAddr = W25Q.getSectorFirstAddr(addr, &secNumber);
  blockFirstAddr = W25Q.getBlock64FirstAddr(addr, &blockNumber);
  block32FirstAddr = W25Q.getBlock64FirstAddr(addr);
  SerialUSB.print("The first Address of the sector of 0xFFFFE: ");
  SerialUSB.println(secFirstAddr,HEX);
  SerialUSB.print("Sector Number：");
  SerialUSB.println(secNumber);
  SerialUSB.print("The first Address of the block of 0xFFFFE: ");
  SerialUSB.println(blockFirstAddr,HEX);
  SerialUSB.print("Block Number: ");
  SerialUSB.println(blockNumber);

  String str1 = "hello,world",str2 ="No data!";

  W25Q.eraseSector(secFirstAddr);
  W25Q.readFlash(addr, &str2, str1.length());
  SerialUSB.println(str2,HEX);
  W25Q.writeFlash(addr, &str1, str1.length());
  W25Q.readFlash(addr, &str2, str1.length());
  SerialUSB.println(str2,HEX);
}

void loop() {
  
}