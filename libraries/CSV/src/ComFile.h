/*1.获取1行有多少列
2.最大例有多少项
0,1,2,3,4,8/r/n
1,1,2,3,4,8

typedef struct{
  int raw;
  int col;
  uint32 indexPosition;
  int length;
  uint8_t buf[0]=‘存放内容’;
}sItem_t;

同一列中最大字符数
*/
#ifndef __COMFILE_H
#define __COMFILE_H

#include <Arduino.h>
#include <SD.h>

class CommonFile{
public:
  CommonFile(File *file);
  CommonFile();
  size_t writeBuf(void *pBuf, size_t nbyte);
  int readBuf(void *pBuf, uint16_t nbyte);
  int peek();
  int available();
  void flush();
  boolean seek(uint32_t pos);
  uint32_t position();
  uint32_t size();
  bool insert(uint32_t index, void *buf, size_t nbyte);
  bool move(uint32_t startIndex, uint32_t destIndex);
  //bool deleteChar(uint32_t startIndex, uint32_t length);
  char * name();
private:
  File *_file;
};
#endif