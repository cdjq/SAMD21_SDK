#include "csv.h"
#include <WString.h>
#include <Stream.h>
#include <UD.h>
#include <SD.h>
#include "ComFile.h"
#ifdef ARDUINO_SAM_ZERO
#include <variant.h>
#endif

#ifndef DFRobot_CSV_H
#define DFRobot_CSV_H
#define DEC 10
#define HEX 16
#define OCT 8
#ifdef BIN // Prevent warnings if BIN is previously defined in "iotnx4.h" or similar
#undef BIN
#endif
#define BIN 2
#define TYPE_CSV_UD   1
#define TYPE_CSV_SD   2
#define FLAG_CSV_UD   "UD:"
#define FILE_SEND_SIZE  0xFFFF
#define CSV_WRITE_BUFFER     512
#define CSV_READ_BUFFER     512

typedef struct{//存放最后一行的数据
  uint8_t cmd;
  int rawIndex;
  int colIndex;
  int startRaw;//起始行
  int startColumn;//起始列
  int endRaw;//结束行
  int endColumn;//结束列
  int raw; //行
  int column;//列
  int rawMax;//最大行
  int colMax;//最大列
  uint32_t startPos;//起始位置
  uint32_t endPos;//结束位置
  uint32_t posIndex;
  String data;
}sItem_t, *pItem_t;

static void cbCountColumn(void *s, size_t i, void *data);
static void cbCountRow(int c, void *data);

#define CSV_CMD_UPDATE  0
#define CSV_CMD_READ_RAW  1
#define CSV_CMD_WRITE_RAW  2
#define CSV_CMD_READ_COL  3
#define CSV_CMD_WRITE_COL  4
#define CSV_CMD_READ_ITEM  5
#define CSV_CMD_WRITE_ITEM  6
#define CSV_CMD_ADD_RAW  7
#define CSV_CMD_ADD_COL  8
#define CSV_CMD_ADD_ITEM  9
#define CSV_CMD_GET_MAXCOLOFRAW  10
class DFRobot_CSV: public Print,public CommonFile{
public:
  DFRobot_CSV();
  DFRobot_CSV(void *file);
  ~DFRobot_CSV();
  void CreateNewStandardForm(int row, int column);
  bool read(String *s);//顺序读，成功返回True，失败返回false
  String readRaw(int raw);//整行读
  bool readRaw(int raw, String *s);//整行读
  bool writeRaw(int raw, String s);//整行写
  bool readColumn(int col, String *s);//整列读
  //bool writeColumn(int col, String *s);//整列写，暂未实现
  bool readItem(int row, int column, String *s);//定位读
  String readItem(int row, int column);//定位读
  //bool writeItem(int row, int column, String s);//定位写,暂未实现
  //bool deleteItem(int row, int column);//定位删除,暂未实现
  //bool deleteRaw(int raw);//整行删除,自动补齐,暂未实现
  //bool deleteColumn(int raw);//整列删除,自动补齐,暂未实现
  //bool deleteArea(int sRaw, int sCol, int eRaw, int eCol);//整列删除,自动补齐,暂未实现
  //bool addRaw(int raw, String s);//增加行,暂未实现
  //bool addColumn(int column, Stirng s);//增加列,暂未实现
  int getRaw();//获取最大行
  int getColumn();//获取最大列，若每行列数不相等，则取所有行中最大的列数
  int readMaxColumnOfRaw(int raw);//获取一行中的列数
  
  virtual size_t write(uint8_t val);
  size_t write(const char *str) {
      if (str == NULL) return 0;
      return write((const uint8_t *)str, strlen(str));
  }
  virtual size_t write(const uint8_t *buffer, size_t size);
  size_t write(const char *buffer, size_t size) {
      return write((const uint8_t *)buffer, size);
  }
  size_t print(const String &s){Print::print(s);}
  size_t print(const char str[]){Print::print(str);}
  size_t print(char c){Print::print(c);}
  size_t print(unsigned char b, int base = DEC){Print::print(b, base);}
  size_t print(int n, int base= DEC){print((long)n, base);}
  size_t print(unsigned int n, int base= DEC){Print::print(n,base);}
  size_t print(long n, int base = DEC);//{Print::print(n,base);}
  size_t print(unsigned long n, int base= DEC){Print::print(n, base);}
  size_t print(double, int = 2);

  size_t println(const String &s){Print::println(s);}
  size_t println(const char str[]){Print::println(str);}
  size_t println(char c){Print::println(c);}
  size_t println(unsigned char b, int base = DEC){Print::println(b, base);}
  size_t println(int n, int base= DEC){println((long)n, base);}
  size_t println(unsigned int n, int base= DEC){Print::println(n,base);}
  size_t println(long n, int base = DEC);
  size_t println(unsigned long n, int base= DEC){Print::println(n, base);}
  size_t println(double, int = 2);
  size_t println(void){Print::println();}
protected:
  bool update();//更新文件的最大行和最大列
private:
  typedef struct csv_parser sCSVParse_t;
  sCSVParse_t _p;
  uint8_t _readBuf[CSV_READ_BUFFER];
  uint8_t _writeBuf[CSV_WRITE_BUFFER];
  uint8_t _flag;//是否加逗号，true:加，false:不加
  sItem_t _item;
  uint8_t _position;
};

#endif