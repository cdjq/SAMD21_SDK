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
  Serial.print("Initializating W25Q device...");
  while(!W25Q.begin()){
    Serial.println("initialization failed!");
  }
  Serial.println("Status Register1 Config: ");
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
  Serial.println("***************************************************");
  Serial.println("*|------------Status Register1 Config------------|*");
  Serial.println("*| s7  | s6  | s5  | s4  | s3  | s2  | s1  | s0  |*");
  Serial.println("*|-----------------------------------------------|*");
  Serial.println("*| (R) | SEC | TB  | BP2 | BP1 | BP0 | WEL | BUSY|*");
  Serial.println("*|-----------------------------------------------|*");
  delay(10);
  Serial.print("*|  ");
  Serial.print(pSR1->reserve);
  Serial.print("  |  ");
  Serial.print(pSR1->sec);
  Serial.print("  |  ");
  Serial.print(pSR1->tb);
  Serial.print("  |  ");
  Serial.print(pSR1->bp2);
  Serial.print("  |  ");
  Serial.print(pSR1->bp1);
  Serial.print("  |  ");
  Serial.print(pSR1->bp0);
  Serial.print("  |  ");
  Serial.print(pSR1->wel);
  Serial.print("  |  ");
  Serial.print(pSR1->busy);
  Serial.println("  |*");
  Serial.println("*************************************************************************************");
  Serial.println("*   |     |     |     |     |     |     |     |_____Erase/Write in Progress         *");
  Serial.println("*   |     |     |     |     |     |     |___________Write Enable Latch              *");
  Serial.println("*   |     |     |     |     |     |                                                 *");
  Serial.println("*   |     |     |     |     |     |                                                 *");
  Serial.println("*   |     |     |     |_____|_____|_________________Block Protect Bits(non-volatile)*");
  Serial.println("*   |     |     |___________________________________Top/Bottom Protect(non-volatile)*");
  Serial.println("*   |     |_________________________________________Sector Protect(non-volatile)    *");
  Serial.println("*   |_______________________________________________Reserved                        *");
  Serial.println("*************************************************************************************");
  Serial.println();
}
void printSR2(){
  uint8_t val = W25Q.readSR(W25Q.eStatusReg2);
  W25QClass::sSR2_t *pSR2 = (W25QClass::sSR2_t *)(&val);
  Serial.println("***************************************************");
  Serial.println("*|------------Status Register2 Config------------|*");
  Serial.println("*| s15 | s14 | s13 | s12 | s11 | s10 | s9  | s8  |*");
  Serial.println("*|-----------------------------------------------|*");
  Serial.println("*| SUS | CMP | LB3 | LB2 | LB1 | (R) | QE  | SRL |*");
  Serial.println("*|-----------------------------------------------|*");
  delay(10);
  Serial.print("*|  ");
  Serial.print(pSR2->srl);
  Serial.print("  |  ");
  Serial.print(pSR2->qe);
  Serial.print("  |  ");
  Serial.print(pSR2->r);
  Serial.print("  |  ");
  Serial.print(pSR2->lb1);
  Serial.print("  |  ");
  Serial.print(pSR2->lb2);
  Serial.print("  |  ");
  Serial.print(pSR2->lb3);
  Serial.print("  |  ");
  Serial.print(pSR2->cmp);
  Serial.print("  |  ");
  Serial.print(pSR2->sus);
  Serial.println("  |*");
  Serial.println("*********************************************************************************************************************");
  Serial.println("*   |     |     |     |     |     |     |     |_____Status Register Lock(Volatile/Non-Volatile Writable)            *");
  Serial.println("*   |     |     |     |     |     |     |____________Quad Enable(Volatile/Non-Volatile Writable)                    *");
  Serial.println("*   |     |     |     |     |     |__________________Reserved                                                       *");
  Serial.println("*   |     |     |     |     |                                                                                       *");
  Serial.println("*   |     |     |     |     |                                                                                       *");
  Serial.println("*   |     |     |_____|_____|________________________Security Register Lock Bits(Volatile/Non-Volatile OTP Writable)*");
  Serial.println("*   |     |_________________________________________Complement Protect(Volatile/Non-Volatile Writable)              *");
  Serial.println("*   |_______________________________________________Suspend Status(Status-only)                                     *");
  Serial.println("*********************************************************************************************************************");
  Serial.println();
}
void printSR3(){
  uint8_t val = W25Q.readSR(W25Q.eStatusReg3);
  W25QClass::sSR3_t *pSR3 = (W25QClass::sSR3_t *)(&val);
  Serial.println("***************************************************");
  Serial.println("*|------------Status Register3 Config------------|*");
  Serial.println("*| s23 | s22 | s21 | s20 | s19 | s18 | s17 | s16 |*");
  Serial.println("*|-----------------------------------------------|*");
  Serial.println("*| (R) | DRV1| DRV2| (R) | (R) | WPS | (R) | (R) |*");
  Serial.println("*|-----------------------------------------------|*");
  delay(10);
  Serial.print("*|  ");
  Serial.print(pSR3->r1);
  Serial.print("  |  ");
  Serial.print(pSR3->drv1);
  Serial.print("  |  ");
  Serial.print(pSR3->drv2);
  Serial.print("  |  ");
  Serial.print(pSR3->r2);
  Serial.print("  |  ");
  Serial.print(pSR3->r3);
  Serial.print("  |  ");
  Serial.print(pSR3->wps);
  Serial.print("  |  ");
  Serial.print(pSR3->r4);
  Serial.print("  |  ");
  Serial.print(pSR3->r5);
  Serial.println("  |*");
  Serial.println("************************************************************************************************************");
  Serial.println("*   |     |     |     |     |     |     |     |_____Reserved                                               *");
  Serial.println("*   |     |     |     |     |     |     |                                                                  *");
  Serial.println("*   |     |     |     |     |     |_____|___________Output Driver Strength(Volatile/Non-Volatile Writable) *");
  Serial.println("*   |     |     |     |     |_______________________Reserved                                               *");
  Serial.println("*   |     |     |     |_____________________________Reserved                                               *");
  Serial.println("*   |     |     |___________________________________Write Protect Selection(Volatile/Non-Volatile Writable)*");
  Serial.println("*   |     |_________________________________________Reserved                                               *");
  Serial.println("*   |_______________________________________________Reserved)                                              *");
  Serial.println("************************************************************************************************************");
  Serial.println();
}