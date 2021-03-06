/*!
 * @file read.ino
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

#define ONBOARD_UD_M0_CS 32 // The cs pin of Firebeetle M0 the onboard U disk is 32
#define NONBOARD_SD_MOUDLE_CS 2//spi 模块的cs引脚连接到M0的数字2引脚

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while(!Serial)
  Serial.print("Initializing FlashDisk...");

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
  Serial.println("initialization done.");
  File myFile;
  myFile = SD.open("abc.csv");           //以读的方式打开abc.csv文件，如果该文件不存在，先运行write.ino创建并写入abc.csv
  DFRobot_CSV csv(&myFile);
  if (myFile) {
      int row = csv.getRow();
      Serial.print("Number of rows: ");
      Serial.println(row);
      int column = csv.getColumn();
      Serial.print("Columns in the first row: ");
      Serial.println(column);
      
      String str[column];
      csv.readRow(1,str);
      Serial.print("The first row is: ");
      for(int j=0;j<column;j++) {
          Serial.print(str[j]);Serial.print(' ');
      }    
      Serial.println();
      
      String item = csv.readItem(2,3);
      Serial.print("The field (2,3) is: ");
      Serial.println(item);

    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening abc.csv");
  }
}
void loop() {
  }
