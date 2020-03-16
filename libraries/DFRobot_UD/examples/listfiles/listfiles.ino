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
#include <UD.h>

UDFile root;

void setup() {
  // Open serial communications and wait for port to open:
  SerialUSB.begin(115200);
  while (!SerialUSB) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  SerialUSB.print("Initializing UD disk...");

  if (!UD.begin()) {
    SerialUSB.println("initialization failed!");
    while (1);
  }
  SerialUSB.println("initialization done.");

  root = UD.open("/");

  printDirectory(root, 0);

  SerialUSB.println("done!");
}

void loop() {
  // nothing happens after setup finishes.
}

void printDirectory(UDFile dir, int numTabs) {
  while (true) {
    UDFile entry =  dir.openNextFile();
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



