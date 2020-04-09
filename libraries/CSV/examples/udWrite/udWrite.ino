/*!
 * @file udWrite.ino
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

void setup() {
  // Open serial communications and wait for port to open:
  SerialUSB.begin(115200);
  while(!SerialUSB);//等待串口打开
  SerialUSB.print("Initializing FlashDisk...");
  if (!UD.begin()) {
    SerialUSB.println("initialization failed!");
    while(1);
  }
  SerialUSB.println("initialization done.");
  
  UDFile myFile;
  myFile = UD.open("abc.csv", FILE_WRITE);                   //以写的方式打开abc.csv文件，没有的话会创建改文件
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
    SerialUSB.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = UD.open("abc.csv");
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
    SerialUSB.println("error opening test.txt");
  }
}

void loop() {
  }
