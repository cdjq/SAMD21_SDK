/*!
 * @file readStatusRegister.ino
 * @brief 读取W25Q128的状态寄存器1/状态寄存器2/状态寄存器3的值，并制表打印出来。
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
  SerialUSB.println("Status Register1 Config: ");
}

void loop() {
  printSR1();
  printSR2();
  printSR3();
  delay(5000);
}

void printSR1(){
  uint8_t val = W25Q.readSR(W25Q.eStatusReg1);
  W25QClass::sSR1_t *pSR1 = (W25QClass::sSR1_t *)(&val);
  SerialUSB.println("***************************************************");
  SerialUSB.println("*|------------Status Register1 Config------------|*");
  SerialUSB.println("*| s7  | s6  | s5  | s4  | s3  | s2  | s1  | s0  |*");
  SerialUSB.println("*|-----------------------------------------------|*");
  SerialUSB.println("*| (R) | SEC | TB  | BP2 | BP1 | BP0 | WEL | BUSY|*");
  SerialUSB.println("*|-----------------------------------------------|*");
  delay(10);
  SerialUSB.print("*|  ");
  SerialUSB.print(pSR1->reserve);
  SerialUSB.print("  |  ");
  SerialUSB.print(pSR1->sec);
  SerialUSB.print("  |  ");
  SerialUSB.print(pSR1->tb);
  SerialUSB.print("  |  ");
  SerialUSB.print(pSR1->bp2);
  SerialUSB.print("  |  ");
  SerialUSB.print(pSR1->bp1);
  SerialUSB.print("  |  ");
  SerialUSB.print(pSR1->bp0);
  SerialUSB.print("  |  ");
  SerialUSB.print(pSR1->wel);
  SerialUSB.print("  |  ");
  SerialUSB.print(pSR1->busy);
  SerialUSB.println("  |*");
  SerialUSB.println("*************************************************************************************");
  SerialUSB.println("*   |     |     |     |     |     |     |     |_____Erase/Write in Progress         *");
  SerialUSB.println("*   |     |     |     |     |     |     |___________Write Enable Latch              *");
  SerialUSB.println("*   |     |     |     |     |     |                                                 *");
  SerialUSB.println("*   |     |     |     |     |     |                                                 *");
  SerialUSB.println("*   |     |     |     |_____|_____|_________________Block Protect Bits(non-volatile)*");
  SerialUSB.println("*   |     |     |___________________________________Top/Bottom Protect(non-volatile)*");
  SerialUSB.println("*   |     |_________________________________________Sector Protect(non-volatile)    *");
  SerialUSB.println("*   |_______________________________________________Reserved                        *");
  SerialUSB.println("*************************************************************************************");
  SerialUSB.println();
}
void printSR2(){
  uint8_t val = W25Q.readSR(W25Q.eStatusReg2);
  W25QClass::sSR2_t *pSR2 = (W25QClass::sSR2_t *)(&val);
  SerialUSB.println("***************************************************");
  SerialUSB.println("*|------------Status Register2 Config------------|*");
  SerialUSB.println("*| s15 | s14 | s13 | s12 | s11 | s10 | s9  | s8  |*");
  SerialUSB.println("*|-----------------------------------------------|*");
  SerialUSB.println("*| SUS | CMP | LB3 | LB2 | LB1 | (R) | QE  | SRL |*");
  SerialUSB.println("*|-----------------------------------------------|*");
  delay(10);
  SerialUSB.print("*|  ");
  SerialUSB.print(pSR2->srl);
  SerialUSB.print("  |  ");
  SerialUSB.print(pSR2->qe);
  SerialUSB.print("  |  ");
  SerialUSB.print(pSR2->r);
  SerialUSB.print("  |  ");
  SerialUSB.print(pSR2->lb1);
  SerialUSB.print("  |  ");
  SerialUSB.print(pSR2->lb2);
  SerialUSB.print("  |  ");
  SerialUSB.print(pSR2->lb3);
  SerialUSB.print("  |  ");
  SerialUSB.print(pSR2->cmp);
  SerialUSB.print("  |  ");
  SerialUSB.print(pSR2->sus);
  SerialUSB.println("  |*");
  SerialUSB.println("*********************************************************************************************************************");
  SerialUSB.println("*   |     |     |     |     |     |     |     |_____Status Register Lock(Volatile/Non-Volatile Writable)            *");
  SerialUSB.println("*   |     |     |     |     |     |     |____________Quad Enable(Volatile/Non-Volatile Writable)                    *");
  SerialUSB.println("*   |     |     |     |     |     |__________________Reserved                                                       *");
  SerialUSB.println("*   |     |     |     |     |                                                                                       *");
  SerialUSB.println("*   |     |     |     |     |                                                                                       *");
  SerialUSB.println("*   |     |     |_____|_____|________________________Security Register Lock Bits(Volatile/Non-Volatile OTP Writable)*");
  SerialUSB.println("*   |     |_________________________________________Complement Protect(Volatile/Non-Volatile Writable)              *");
  SerialUSB.println("*   |_______________________________________________Suspend Status(Status-only)                                     *");
  SerialUSB.println("*********************************************************************************************************************");
  SerialUSB.println();
}
void printSR3(){
  uint8_t val = W25Q.readSR(W25Q.eStatusReg3);
  W25QClass::sSR3_t *pSR3 = (W25QClass::sSR3_t *)(&val);
  SerialUSB.println("***************************************************");
  SerialUSB.println("*|------------Status Register3 Config------------|*");
  SerialUSB.println("*| s23 | s22 | s21 | s20 | s19 | s18 | s17 | s16 |*");
  SerialUSB.println("*|-----------------------------------------------|*");
  SerialUSB.println("*| (R) | DRV1| DRV2| (R) | (R) | WPS | (R) | (R) |*");
  SerialUSB.println("*|-----------------------------------------------|*");
  delay(10);
  SerialUSB.print("*|  ");
  SerialUSB.print(pSR3->r1);
  SerialUSB.print("  |  ");
  SerialUSB.print(pSR3->drv1);
  SerialUSB.print("  |  ");
  SerialUSB.print(pSR3->drv2);
  SerialUSB.print("  |  ");
  SerialUSB.print(pSR3->r2);
  SerialUSB.print("  |  ");
  SerialUSB.print(pSR3->r3);
  SerialUSB.print("  |  ");
  SerialUSB.print(pSR3->wps);
  SerialUSB.print("  |  ");
  SerialUSB.print(pSR3->r4);
  SerialUSB.print("  |  ");
  SerialUSB.print(pSR3->r5);
  SerialUSB.println("  |*");
  SerialUSB.println("************************************************************************************************************");
  SerialUSB.println("*   |     |     |     |     |     |     |     |_____Reserved                                               *");
  SerialUSB.println("*   |     |     |     |     |     |     |                                                                  *");
  SerialUSB.println("*   |     |     |     |     |     |_____|___________Output Driver Strength(Volatile/Non-Volatile Writable) *");
  SerialUSB.println("*   |     |     |     |     |_______________________Reserved                                               *");
  SerialUSB.println("*   |     |     |     |_____________________________Reserved                                               *");
  SerialUSB.println("*   |     |     |___________________________________Write Protect Selection(Volatile/Non-Volatile Writable)*");
  SerialUSB.println("*   |     |_________________________________________Reserved                                               *");
  SerialUSB.println("*   |_______________________________________________Reserved)                                              *");
  SerialUSB.println("************************************************************************************************************");
  SerialUSB.println();
}