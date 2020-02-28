/*!
 * @file write.ino
 * @brief 打开abc.csv文件，获得总的行数，和指定行的列数，读取指定的行的值，指定位置的值，并打印出来
 * @n 实验现象：串口监视器打印出abc.csv文件指定的内容
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [LiYue](liyue.wang@dfrobot.com)
 * @version  V1.0
 * @date  2019-2-28
 * @get from https://www.dfrobot.com
 * @url https://github.com/cdjq/DFRobot_CSV
 */

#include<DFRobot_CSV.h>

void setup() {
  // Open serial communications and wait for port to open:
  SerialUSB.begin(115200);
  delay(5000);
  SerialUSB.print("Initializing FlashDisk...");
  if (!SD.begin(32)) {
    SerialUSB.println("initialization failed!");
    while (1);
  }
  SerialUSB.println("initialization done.");
  File myFile;
  myFile = SD.open("abc.csv");           //以读的方式打开abc.csv文件，如果该文件不存在，先运行write.ino创建并写入abc.csv
  DFRobot_CSV csv(myFile);
  if (myFile) {
      int row = csv.getRow();
      SerialUSB.print("Number of rows: ");
      SerialUSB.println(row);
      
      int column = csv.getColumn(1);
      SerialUSB.print("Columns in the first row: ")
      SerialUSB.println(column);
      
      String str[column];
      csv.readRow(1,str);
      SerialUSB.print("The first row is: ")
      for(int j=0;j<column;j++) {
          SerialUSB.print(str[j]);SerialUSB.print(' ');
      }    
      SerialUSB.println();
      
      String item = csv.readItem(2,3);
      SerialUSB.print("The field (2,3) is: ");
      SerialUSB.println(item);

    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    SerialUSB.println("error opening test.txt");
  }
}
void loop() {
  }
