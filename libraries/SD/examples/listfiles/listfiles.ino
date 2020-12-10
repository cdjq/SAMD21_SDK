/*!
 * @file listFiles.ino
 * @brief The Listfiles of UD disk. 
 * @n This example shows how print out the files in a directory on a UD disk file
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


File root;

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
/*如果外部连接spi SD卡模块，初始化如下*/
  //if (!SD.begin(/*csPin = */ONBOARD_UD_M0_CS, /*type = */TYPE_NONBOARD_SD_MOUDLE)) {
  //  SerialUSB.println("initialization failed!");
  //  while(1);
  //}

  root = SD.open("/");

  printDirectory(root, 0);

  SerialUSB.println("done!");
}

void loop() {
  // nothing happens after setup finishes.
}

void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry =  dir.openNextFile();
    if (!entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      SerialUSB.print('\t');
    }
    SerialUSB.print(entry.name());
    if (entry.isDirectory()) {
      SerialUSB.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      SerialUSB.print("\t\t");
      SerialUSB.println(entry.size(), DEC);
    }
    entry.close();
  }
}



