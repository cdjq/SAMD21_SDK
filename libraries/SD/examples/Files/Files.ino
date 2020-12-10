/*!
 * @file Files.ino
 * @brief UD disk basic file example.
 * @n This example shows how to create and destroy an UD disk file.
 * SPI flash hardware conneted
 * SPI CS------->32 pin
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @version  V1.0
 * @date  2019-03-5
 * @get from https://www.dfrobot.com
 */

#include <SD.h>

#define ONBOARD_UD_M0_CS 32 // The cs pin of Firebeetle M0 the onboard U disk is 32
#define NONBOARD_SD_MOUDLE_CS 2//spi 模块的cs引脚连接到M0的数字2引脚

File myFile;

void setup() {
  // Open serial communications and wait for port to open:
  SerialUSB.begin(115200);
  while (!SerialUSB) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  SerialUSB.print("Initializing UD disk...");
/*如果使用M0板载的U盘，初始化如下*/
  if (!SD.begin(/*csPin = */ONBOARD_UD_M0_CS, /*type = */TYPE_ONBOARD_UD_SAMD)) {
    SerialUSB.println("initialization failed!");
    while(1);
  }
/*如果外部连接是spi SD卡模块，初始化如下*/
  //if (!SD.begin(/*csPin = */ONBOARD_UD_M0_CS, /*type = */TYPE_NONBOARD_SD_MOUDLE)) {
  //  SerialUSB.println("initialization failed!");
  //  while(1);
  //}
 
  Serial.println("initialization done.");

  if (SD.exists("example.txt")) {
    SerialUSB.println("example.txt exists.");
  } else {
    SerialUSB.println("example.txt doesn't exist.");
  }

  // open a new file and immediately close it:
  SerialUSB.println("Creating example.txt...");
  myFile = SD.open("example.txt", FILE_WRITE);
  myFile.close();

  // Check to see if the file exists:
  if (SD.exists("example.txt")) {
    SerialUSB.println("example.txt exists.");
  } else {
    SerialUSB.println("example.txt doesn't exist.");
  }

  // delete the file:
  SerialUSB.println("Removing example.txt...");
  SD.remove("example.txt");

  if (SD.exists("example.txt")) {
    SerialUSB.println("example.txt exists.");
  } else {
    SerialUSB.println("example.txt doesn't exist.");
  }
}

void loop() {
  // nothing happens after setup finishes.
}