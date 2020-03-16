/*!
 * @file readID.ino
 * @brief 读取W25Q128的芯片唯一ID/制造商ID/设备ID/设备容量，并通过串口打印出来。，该ID
 * @n 唯一ID：每个芯片都有唯一ID，该ID有64位，用16进制表示有16位，可用来区分不同的物理芯片。
 * @n 制造商ID：该芯片的制造者。
 * @n 设备ID：W25Q系列种类ID，W25Q系列有多少种芯片，就有多少种ID，W25Q同一系列的ID是相同的，
 * @n 不同种类的ID是不同的，比如W25Q128的设备ID是一致的，W25Q128和W25Q16的ID是不同的。
 * @n 设备容量：单位字节
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2019-03-3
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_IIC_Serial
 */

#include <W25Q.h>

void setup() {
  Serial.begin(115200);
  SerialUSB.print("Initializating W25Q device...");
  while(!W25Q.begin()){
    SerialUSB.println("initialization failed!");
  }
  SerialUSB.println("initialization done!");
}

void loop() {
  SerialUSB.print("Unique ID: ");
  String id = W25Q.readUniqueID();
  SerialUSB.println(id);
  SerialUSB.print("Manufacter ID and Device ID: ");
  uint16_t mDevID = W25Q.readManufacterAndDeviceID();
  SerialUSB.println(mDevID,HEX);
  SerialUSB.print("Device ID: ");
  uint8_t devID = W25Q.readDeviceID();
  SerialUSB.println(devID,HEX);
  SerialUSB.print("Capacity: ");
  SerialUSB.print(W25Q.readFlashCapacityByte());
  SerialUSB.println(" Bytes");
  SerialUSB.println();
  delay(5000);
}