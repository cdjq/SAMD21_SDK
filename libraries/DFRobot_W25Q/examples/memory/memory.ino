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
  Serial.begin(115200);
  Serial.print("Initializating W25Q device...");
  while(!W25Q.begin()){
    Serial.println("initialization failed!");
  }
  while(!Serial.available());
  Serial.read();Serial.read();Serial.read();
  Serial.println("initialization done!");
  secFirstAddr = W25Q.getSectorFirstAddr(addr, &secNumber);
  blockFirstAddr = W25Q.getBlock64FirstAddr(addr, &blockNumber);
  block32FirstAddr = W25Q.getBlock64FirstAddr(addr);
  Serial.print("The first Address of the sector of 0xFFFFE: ");
  Serial.println(secFirstAddr,HEX);
  Serial.print("Sector Number：");
  Serial.println(secNumber);
  Serial.print("The first Address of the block of 0xFFFFE: ");
  Serial.println(blockFirstAddr,HEX);
  Serial.print("Block Number: ");
  Serial.println(blockNumber);

  String str1 = "hello,world",str2 ="No data!";

  W25Q.eraseSector(secFirstAddr);
  W25Q.readFlash(addr, &str2, str1.length());
  Serial.println(str2,HEX);
  W25Q.writeFlash(addr, &str1, str1.length());
  W25Q.readFlash(addr, &str2, str1.length());
  Serial.println(str2,HEX);
}

void loop() {
  
}