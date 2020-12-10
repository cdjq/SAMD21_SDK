/*!
 * @file sdWrite.ino
 * @brief 创建一个abc.csv文件并往里输入数据，最后读出文件内容
 * @n 实验现象：串口监视器打印出abc.csv文件内容
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [LiYue](liyue.wang@dfrobot.com)
 * @version  V1.0
 * @date  2019-2-28
 * @get from https://www.dfrobot.com
 * @url https://github.com/cdjq/DFRobot_CSV
 */

#include <DFRobot_CSV.h>

#define ONBOARD_UD_M0_CS 32 // The cs pin of Firebeetle M0 the onboard U disk is 32
#define NONBOARD_SD_MOUDLE_CS 2//spi 模块的cs引脚连接到M0的数字2引脚


void setup() {
  // Open serial communications and wait for port to open:
  SerialUSB.begin(115200);
  while(!SerialUSB);//等待串口打开
  SerialUSB.print("Initializing FlashDisk...");
  /*如果使用M0板载的U盘，初始化如下*/
  if (!SD.begin(/*csPin = */ONBOARD_UD_M0_CS, /*type = */TYPE_ONBOARD_UD_SAMD)) {
    SerialUSB.println("initialization failed!");
    while(1);
  }
/*如果外部连接是spi SD卡模块，初始化如下*/
  //if (!SD.begin(/*csPin = */NONBOARD_SD_MOUDLE_CS, /*type = */TYPE_NONBOARD_SD_MOUDLE)) {
  //  SerialUSB.println("initialization failed!");
  //  while(1);
  //}
  SerialUSB.println("initialization done.");
  
  File myFile;
  myFile = SD.open("abc.csv", FILE_WRITE);                   //以写的方式打开abc.csv文件，没有的话会创建改文件
  DFRobot_CSV csv(&myFile);
  // if the file opened okay, write to it:
  if(myFile) {
    SerialUSB.println("Writing to test.txt...");
    csv.print("group");csv.print("number");csv.println("mark");        //以csv格式往文件写入数据，用print输入每列的值，用println输入值并换行
    csv.print("\"Jerry\"");csv.print('1');csv.println(3.65);
    csv.print("3,Herny");csv.print("10");csv.println(99.5);
    // close the file:
    myFile.close();
    SerialUSB.println("done.");
  } else {
    // if the file didn't open, print an error:
    SerialUSB.println("error opening abc.csv");
  }

  // re-open the file for reading:
  myFile = SD.open("abc.csv");
  if (myFile) {
    SerialUSB.println("abc.csv:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      SerialUSB.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    SerialUSB.println("error opening abc.csv");
  }
}

void loop() {
  }
