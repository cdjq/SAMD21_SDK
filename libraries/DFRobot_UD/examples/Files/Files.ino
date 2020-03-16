/*!
 * @file Files.ino
 * @brief UD disk basic file example.
 * @n This example shows how to create and destroy an UD disk file
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @version  V1.0
 * @date  2019-03-5
 * @get from https://www.dfrobot.com
 */

#include <UD.h>

UDFile myFile;

void setup() {
  // Open serial communications and wait for port to open:
  SerialUSB.begin(115200);
  while (!SerialUSB) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  SerialUSB.print("Initializing UD disk...");

  if (!UD.begin()) {
    SerialUSB.println("initialization failed!");
    while(1);
  }
  Serial.println("initialization done.");

  if (UD.exists("example.txt")) {
    SerialUSB.println("example.txt exists.");
  } else {
    SerialUSB.println("example.txt doesn't exist.");
  }

  // open a new file and immediately close it:
  SerialUSB.println("Creating example.txt...");
  myFile = UD.open("example.txt", FILE_WRITE);
  myFile.close();

  // Check to see if the file exists:
  if (UD.exists("example.txt")) {
    SerialUSB.println("example.txt exists.");
  } else {
    SerialUSB.println("example.txt doesn't exist.");
  }

  // delete the file:
  SerialUSB.println("Removing example.txt...");
  UD.remove("example.txt");

  if (UD.exists("example.txt")) {
    SerialUSB.println("example.txt exists.");
  } else {
    SerialUSB.println("example.txt doesn't exist.");
  }
}

void loop() {
  // nothing happens after setup finishes.
}



