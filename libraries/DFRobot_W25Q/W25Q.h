/*!
 * @file W25Q.h
 * @brief W25Q Master library for Arduino FireBeetle Board-M0. 
 * @n The W25Q series are Serial Flash memory which provides a storage solution for systems
 * with limited space,pins and power.(WQ25系列是串行flash存储器，可以为空间，引脚，功耗有限
 * 制的系统提供一个存储方案) The W25Q series offers flexibility and performance well beyond
 * ordinary Serial Flash devices.(W25Q系列的灵活性和性能比一般的串行flash设备要高)They are
 * ideal for code shadowing to RAM, executing code directly from Dual/Quad SPI and storing
 * voice, text and data.(串行flash可以实现代码映射到RAM，直接通过SPI方式执行代码，存储声音，
 * 文本，数据)
 * @n W25Q库向用户提供了一系列的接口函数，使用户可以直接对W25Q系列的存储器进行读取等操作
 * @n 该库支持的芯片系列: W25Q128
 * -------------------------------------------------------
 * | 芯片类别 | 内存大小 |  块大小  | 扇区大小 | 页大小  |
 * -------------------------------------------------------
 * | W25Q128  |   16MB   |   64KB   |    4KB   |   256B  |
 * -------------------------------------------------------
 (16M)
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2019-02-28
 * @
 */

#ifndef __W25Q_H
#define __W25Q_H

#include <Arduino.h>
#include <SPI.h>

#define DEBUG
#ifdef  DEBUG
#ifdef ARDUINO_SAM_ZERO//M0板子的串口与其他串口使用方式有区别
#define DBG(...) {SerialUSB.print("["); SerialUSB.print(__FUNCTION__); SerialUSB.print("(): "); SerialUSB.print(__LINE__); SerialUSB.print(" ] "); SerialUSB.println(__VA_ARGS__);}
#else
#define DBG(...) {Serial.print("["); Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#endif
#else
#define DBG(...)
#endif

//WQ25系列芯片的ID列表
#define W25Q128  0XEF17

//在FireBeetle Board-M0上，板载的W25Q128芯片的CS引脚被连接到M0板子32引脚
#define W25Q_CHIP_CS_PIN 32
#define W25Q_CHIP_CS_INVAILD 255
#define W25Q_CHIP_CS_HIGH digitalWrite(_cs, HIGH)
#define W25Q_CHIP_CS_LOW digitalWrite(_cs, LOW)

//W25Q系列的页大小为256字节
#define W25Q_PAGE_SIZE 256
//W25Q系列的扇区大小为4KB = 4096Bytes
#define W25Q_SECTOR_SIZE 4096
//W25Q系列的块大小为64KB = 65536Bytes
#define W25Q_BLOCK_SIZE 65536
//W25Q系列的扇区的数量
#define W25Q_SECTOR_NUMBER  4096
//W25Q系列的块的数量
#define W25Q_BLOCK_NUMBER  256
//W25Q内存起始地址
#define W25Q_BLOCK_ADDR_START  0x000000
//W25Q内存结束地址
#define W25Q_BLOCK_ADDR_END    0xFFFFFF


//指令表
#define W25Q_CMD_NON_WRITE_ENABLE   0x06
#define W25Q_CMD_WRITE_ENABLE       0x50
#define W25Q_CMD_WRITE_DISABLE      0x04
#define W25Q_CMD_READ_SR1           0x05
#define W25Q_CMD_READ_SR2           0x35
#define W25Q_CMD_READ_SR3           0x15
#define W25Q_CMD_WRITE_SR1          0x01
#define W25Q_CMD_WRITE_SR2          0x31
#define W25Q_CMD_WRITE_SR3          0x11
#define W25Q_CMD_READ_FLASH         0x03
#define W25Q_CMD_PAGE_PROGRAM       0x02
#define W25Q_CMD_SECTOR_ERASE       0x20
#define W25Q_CMD_BLOCK32_ERASE      0x52
#define W25Q_CMD_BLOCK64_ERASE      0xD8
#define W25Q_CMD_CHIP_ERASE         0xC7
#define W25Q_CMD_POWERDOWN          0XB9
#define W25Q_CMD_POWERDOWN_RELEASE  0xAB
#define W25Q_CMD_READ_DEVICEID      0xAB
#define W25Q_CMD_READ_MDEVICEID     0x90
#define W25Q_CMD_READ_ID            0x4B
#define W25Q_CMD_READ_JEDEC_ID      0x9F
#define W25Q_CMD_SCYREG_ERASE       0x44
#define W25Q_CMD_READ_SCYREG        0x42
#define W25Q_CMD_WRITE_SCYREG       0x48
#define W25Q_CMD_RESET_ENABLE       0x66
#define W25Q_CMD_RESET              0x99
#define W25Q_CMD_SUSPEND            0x75
#define W25Q_CMD_RESUME             0x7A
#define W25Q_CMD_LOCK               0x36
#define W25Q_CMD_UNLOCK             0x39
#define W25Q_CMD_READ_ISLOCK        0x3D
#define W25Q_CMD_GLOBAL_LOCK        0x7E
#define W25Q_CMD_GLOBAL_UNLOCK      0x98


class W25QClass{
public:
  typedef enum{
      eStatusReg1 = 0,/*!< 状态寄存器1*/
      eStatusReg2,/*!< 状态寄存器2*/
      eStatusReg3,/*!< 状态寄存器3*/
  }eSR_t;
  typedef enum{
      eNonVolMode = 0,/*!< 采用它写入的数据，掉电不会丢失*/
      eVolMode,/*!< 采用此方法写入的数据，掉电会丢失*/
  }eWEMode_t;
  /*
   W25Q128状态寄存器1
     * |-------------------------------------------------------------------------
     * |   s7   |   s6   |   s5   |   s4   |   s3   |   s2   |   s1   |   s0   |
     * -------------------------------------------------------------------------
     * |   (R)  |   SEC  |   TB   |   BP2  |   BP1  |   BP0  |   WEL  |  BUSY  |
     * -------------------------------------------------------------------------
  */
  typedef struct{
      uint8_t busy : 1; /*!< Erase/Write in Progress*/
      uint8_t wel : 1; /*!< Write Enable Latch */
      uint8_t bp0 : 1; /*!< Block Protect Bits(non-volatile) */
      uint8_t bp1 : 1; /*!< Block Protect Bits(non-volatile) */
      uint8_t bp2 : 1; /*!< Block Protect Bits(non-volatile) */
      uint8_t tb : 1; /*!< Top/Bottom Protect(non-volatile) */
      uint8_t sec : 1; /*!< Sector Protect(non-volatile) */
      uint8_t reserve : 1; /*!< Reserved */
  } __attribute__ ((packed)) sSR1_t; 
  /*
   W25Q128状态寄存器2
     * -------------------------------------------------------------------------
     * |   s15  |   s14  |   s13  |   s12  |   s11  |   s10  |   s9   |   s8   |
     * -------------------------------------------------------------------------
     * |   SUS  |   CMP  |   LB3  |   LB2  |   LB1  |   (R)  |   QE   |   SRL  |
     * -------------------------------------------------------------------------
  */
  typedef struct{
      uint8_t srl : 1; /*!< Status Register Lock(Volatile/Non-Volatile Writable)*/
      uint8_t qe : 1; /*!< Quad Enable(Volatile/Non-Volatile Writable)  */
      uint8_t r : 1; /*!< Reserved */
      uint8_t lb1 : 1; /*!< Security Register Lock Bits(Volatile/Non-Volatile OTP Writable) */
      uint8_t lb2 : 1; /*!< Security Register Lock Bits(Volatile/Non-Volatile OTP Writable) */
      uint8_t lb3 : 1; /*!< Security Register Lock Bits(Volatile/Non-Volatile OTP Writable) */
      uint8_t cmp : 1; /*!< Complement Protect(Volatile/Non-Volatile Writable) */
      uint8_t sus : 1; /*!< Suspend Status(Status-only) */
  } __attribute__ ((packed)) sSR2_t;
  /*
   W25Q128状态寄存器3
     * -------------------------------------------------------------------------
     * |   s23  |   s22  |   s21  |   s20  |   s19  |   s18  |   s17  |   s16  |
     * -------------------------------------------------------------------------
     * |   (R)  |  DRV1  |  DRV2  |   (R)  |   (R)  |   WPS  |   (R)  |   (R)  |
     * -------------------------------------------------------------------------
  */
  typedef struct{
      uint8_t r1 : 1; /*!< Reserved*/
      uint8_t drv1 : 1; /*!< Output Driver Strength(Volatile/Non-Volatile Writable)  */
      uint8_t drv2 : 1; /*!< Output Driver Strength(Volatile/Non-Volatile Writable) */
      uint8_t r2 : 1; /*!< Reserved */
      uint8_t r3 : 1; /*!< Reserved */
      uint8_t wps : 1; /*!< Write Protect Selection(Volatile/Non-Volatile Writable) */
      uint8_t r4 : 1; /*!< Reserved */
      uint8_t r5 : 1; /*!< Reserved */
  } __attribute__ ((packed)) sSR3_t;

  W25QClass(uint8_t csPin = W25Q_CHIP_CS_PIN):_spi(&SPI),_cs(csPin){}
  W25QClass(SPIClass *spi, uint8_t csPin = W25Q_CHIP_CS_PIN):_spi(spi),_cs(csPin){}
  /**
   * @brief W25Q FLASH初始化
   * @return 成功返回true，失败返回false
   */
  bool begin(uint8_t csPin = W25Q_CHIP_CS_INVAILD, uint32_t freq = 0);
  /**
   * @brief 读取FLASH唯一ID,64位ID，每片被固化的芯片的ID号都是唯一的
   * @return 返回FLASH的ID的值,该ID值唯一，可用来区分不同的物理芯片
   */
  String readUniqueID();

  /**
   * @brief 读取制造商和芯片ID号，该数据可用来区分不同类别的W25Q系列，
   * @n 比如W25Q128系列为0XEF17，W25Q16为0XEF14，其中EF为制造商ID，14和17代表芯片ID
   * @return 返回16位的数据，高8位字节为制造商ID，低8位ID为芯片ID号
   */
  uint16_t readManufacterAndDeviceID();
  /**
   * @brief 读取flash容量,单位字节
   * @return 返回flash的容量
   */
  uint32_t readFlashCapacityByte();
  
  /**
   * @brief 读取芯片ID号，该命令还可以将设备从power-down状态下唤醒，该ID号可用来
   * @n 区分芯片是W25Q128还是W25Q16，还是其他W25Q系列的产品。换言之，即所有的
   * @n W25Q128系列的ID号都是0x17，与readManufactDeviceID()函数一样，都可以用来区分
   * @n W25Q的种类，只是readManufactDeviceID()多了一个字节用来说明制造商
   * @return 芯片ID号
   */
  uint8_t readDeviceID();
  
  /**
   * @brief 读取状态寄存器的值
   * @param sr 代表状态寄存器的名字,即读取哪个状态寄存器的值,可取eSR_t结构里的枚举变量
   * @return 返回某个状态寄存器的值
   */
  uint8_t readSR(eSR_t sr);
  /**
   * @brief 写状态寄存器的值
   * @param sr 代表状态寄存器的名字,即读取哪个状态寄存器的值,可取eSR_t结构里的枚举变量
   * @param srVal 状态寄存器的值 
   * @param mode 表示采用什么方式写入数据，eNonVolMode默认为写入数据后，掉电不丢失,eVolMode掉电丢失
   * @return 空
   */
  void writeSR(eSR_t sr, uint8_t srVal, eWEMode_t mode = eNonVolMode);
  /**
   * @brief 读取FLASH
   * @param addr FLASH的地址，及从flash的指定位置addr处读取
   * @param pBuf 存储从FLASH中读取的内容
   * @param len 读取内容的大小，单位字节，即从FLASH中读取多少字节的数据
   * @return 无
   */
  void readFlash(uint32_t addr, void *pBuf, uint16_t len);
  /**
   * @brief 写入FLASH
   * @param addr FLASH的地址，及从flash的指定位置addr处读取
   * @param pBuf 存放要写入FLASH中的数据
   * @param len 写入数据的字节大小，即要向FLASH中写入多少字节的数据
   * @return 无
   */
  void writeFlash(uint32_t addr, void *pBuf, uint16_t len);
  /**
   * @brief 整片擦除,以W25Q128为例，芯片容量为16MB，该功能会将芯片里的内容全部擦除
   */
  void eraseChip();
  /**
   * @brief 扇区擦除，一个扇区4KBytes
   * @param number 扇区编号， 以W25Q128为例，它的容量为16MB，而每个扇区为4KB，
   * @n 则W25Q128有4096个扇区, number取值为0~4095
   * @param isFirstAddr 在擦除扇区之前，是否检查Addr为扇区首地址，如果为true，
   * @n 则表示在擦除之前需检测addr是否为首地址，如果不是则不擦除，如果为false，表示
   * @n 无需检测，直接执行擦除命令
   */
  void eraseSector(uint32_t addr, bool isFirstAddr = true);
  /**
   * @brief 块擦除，一个块64KB, 这里只檫除32KB
   * @param addr 24位地址
   * @param isFirstAddr 在擦除块之前，是否检查Addr为块首地址，如果为true，
   * @n 则表示在擦除之前需检测addr是否为首地址，如果不是则不擦除，如果为false，表示
   * @n 无需检测，直接执行擦除命令
   */
  void eraseBlock32(uint32_t addr, bool isFirstAddr = true);
  /**
   * @brief 块擦除，一个块64KB, 这里檫除整个块，64KB
   * @param addr 24位地址
   * @param isFirstAddr 在擦除块之前，是否检查Addr为块首地址，如果为true，
   * @n 则表示在擦除之前需检测addr是否为首地址，如果不是则不擦除，如果为false，表示
   * @n 无需检测，直接执行擦除命令
   */
  void eraseBlock64(uint32_t addr, bool isFirstAddr = true);
  /**
   * @brief 擦除或页编程挂起，可以暂停正在进行的块、扇区的擦除指令，或页编程指令
   * @n 注：改操作对芯片擦除无效
   */
  void eraseOrProgramSuspend();
  /**
   * @brief 恢复被挂起的擦除或页编程操作
   */
  void eraseOrProgramResume();
  /**
   * @brief 写使能，模块上电后，在进行读写操作之前必须发送给写使能，该功能生效后WEL位自动清除
   * @n 软件控制写保护（SRP0，SRP1, BP）（由于硬件设计问题，M0板载的W25Q128芯片不支持硬件写保护）
   * @param mode 表示采用什么方式写入数据，默认为写入数据后，掉电不丢失eNonVolMode
   * @return 根据SR1寄存器的WEL位判断，为1返回True，为0返回False，注意eVolMode模式下，WEL的值不会改变
   */
  bool writeEnable(eWEMode_t mode = eNonVolMode);
  /**
   * @brief 写保护，写入内容后，调用该函数禁止写入
   */
  void writeDisable();
  /**
   * @brief 进入power-down模式，该模式下待机电流更小
   */
  void powerDown();
  /**
   * @brief 将芯片从power-down状态下唤醒，恢复正常状态
   */
  void releasePowerDown();
  
  /**
   * @brief 复位，一旦调用复位函数，芯片会恢复到上电时候的初始状态，所有易失数据（volatile）
   * @n 都将丢失，相当于掉电重启
   */
  void reset();
  /**
   * @brief 获取地址addr的扇区首地址和所在扇区号
   * @param addr 24位flash地址
   * @param secNum 指针，指向addr所在的扇区号，从0开始，默认为空,即不获取扇区号
   * @return 返回addr所在扇区的首地址
   */
  uint32_t getSectorFirstAddr(uint32_t addr, uint16_t *secNum = NULL);
  /**
   * @brief 获取地址addr的block(32KB)首地址和所在block号
   * @param addr 24位flash地址
   * @param b32Num 指针，指向addr所在的block号，从0开始，默认为空,即不获取block号
   * @return 返回addr所在block的首地址
   */
  uint32_t getBlock32FirstAddr(uint32_t addr, uint16_t *b32Num = NULL);
  /**
   * @brief 获取地址addr的block(64KB)首地址和所在block号
   * @param addr 24位flash地址
   * @param b64Num 指针，指向addr所在的block号,从0开始，默认为空,即不获取block号
   * @return 返回addr所在block的首地址
   */
  uint32_t getBlock64FirstAddr(uint32_t addr, uint16_t *b64Num = NULL);
  /**
   * @brief 等待空闲，判断FLASH芯片写操作是否完成
   * @return 如果FLASH繁忙，则返回true，否则会返回false
   */
  bool waitBusy();


private:
  SPIClass *_spi;
  uint8_t _cs;
};

extern W25QClass W25Q;

#endif
