#include <math.h>
#include <DFRobot_CSV.h>
#include "ComFile.h"

//列计算逗号的个数
static void cbCountColumn(void *s, size_t i, void *data){
  pItem_t pItem = (pItem_t)data;
  switch(pItem->cmd){
      case CSV_CMD_UPDATE:
           pItem->colIndex++;
           break;
      case CSV_CMD_READ_RAW:
          if((pItem->rawIndex == (pItem->raw - 1))&&(pItem->colIndex == 0)){
              pItem->startPos = pItem->posIndex;
          }
          pItem->posIndex += i;
          pItem->colIndex++;
          break;
      case CSV_CMD_WRITE_RAW:
           break;
      case CSV_CMD_READ_COL:
           pItem->colIndex++;
           if(pItem->colIndex == pItem->column){
               for(size_t j = 0; j < i; j++){
                   pItem->data += ((char *)s)[j];
               }
               if(i == 0) pItem->data += "NULL\r\n";
               pItem->data += '\r\n';
           }
           break;
      case CSV_CMD_WRITE_COL:
           break;
      case CSV_CMD_READ_ITEM:
           pItem->colIndex++;
           if((pItem->rawIndex == (pItem->raw - 1))&&(pItem->colIndex == pItem->column)){
              pItem->startPos = pItem->posIndex;
              for(size_t j = 0; j < i; j++){
                   pItem->data += ((char *)s)[j];
               }
              if(i == 0) pItem->data += "NULL";
			  pItem->endPos = pItem->posIndex + i;
           }
           break;
      case CSV_CMD_WRITE_ITEM:
           break;
      case CSV_CMD_ADD_RAW:
           break;
      case CSV_CMD_ADD_COL:
           break;
      case CSV_CMD_ADD_ITEM:
           break;
	  case CSV_CMD_GET_MAXCOLOFRAW:
           pItem->colIndex++;
		   break;
  }
}
static void cbCountRow(int c, void *data){
  pItem_t pItem = (pItem_t)data;
  switch(pItem->cmd){
      case CSV_CMD_UPDATE:
           if(c < 0) pItem->colIndex -= 1;
           if(pItem->colIndex > pItem->colMax) pItem->colMax = pItem->colIndex;
           pItem->colIndex = 0;
           pItem->rawIndex++;
           if(pItem->rawIndex > pItem->rawMax) pItem->rawMax = pItem->rawIndex;
           break;
      case CSV_CMD_READ_RAW:
           pItem->rawIndex++;
           pItem->posIndex -= 1;
           if(c < 0) pItem->colIndex -= 1;
           pItem->posIndex += (pItem->colIndex+1);
           if(pItem->rawIndex == pItem->raw)
               pItem->endPos = pItem->posIndex;
           pItem->colIndex = 0;
           pItem->posIndex += 1;
           break;
      case CSV_CMD_WRITE_RAW:
           break;
      case CSV_CMD_READ_COL:
           pItem->rawIndex++;
           pItem->colIndex = 0;
           break;
      case CSV_CMD_WRITE_COL:
           break;
      case CSV_CMD_READ_ITEM:
	       pItem->colIndex = 0;
           pItem->rawIndex++;
           break;
      case CSV_CMD_WRITE_ITEM:
           break;
      case CSV_CMD_ADD_RAW:
           break;
      case CSV_CMD_ADD_COL:
           break;
      case CSV_CMD_ADD_ITEM:
           break;
	  case CSV_CMD_GET_MAXCOLOFRAW:
           pItem->rawIndex++;
		   if(pItem->rawIndex == pItem->raw){
			   pItem->column = pItem->colIndex;
		   }
		   pItem->colIndex = 0;
		   break;
  }
}
DFRobot_CSV::DFRobot_CSV(): CommonFile(NULL)
{
  csv_init(&_p,0);
  memset(_readBuf,0,sizeof(_readBuf));
  memset(_writeBuf,0,sizeof(_writeBuf));
  memset(&_item,0,sizeof(sItem_t));
  _flag = 1;
}

DFRobot_CSV::DFRobot_CSV(void *file):CommonFile(file){
  csv_init(&_p,0);
  memset(_readBuf,0,sizeof(_readBuf));
  memset(_writeBuf,0,sizeof(_writeBuf));
  memset(&_item,0,sizeof(sItem_t));
  _flag = 1;
}
DFRobot_CSV::~DFRobot_CSV()
{
  if(_p.entry_buf)
  _p.free_func(_p.entry_buf);
}

void DFRobot_CSV::CreateNewStandardForm(int row, int column){
  for(int i = 0; i < row; i++){
      for(int j = 0; j < column; j++){
          print("NULL");
      }
      println();
  }
}
bool DFRobot_CSV::update(){
  memset(_readBuf, 0, sizeof(_readBuf));
  memset(&_item, 0 , sizeof(sItem_t));
  uint16_t temp = 0;
  _item.cmd = CSV_CMD_UPDATE;
  seek(0);
  while((temp = readBuf(_readBuf, CSV_READ_BUFFER))){
      if(csv_parse(&_p, _readBuf, temp, cbCountColumn, cbCountRow, &_item) != temp){
          seek(size());
          return false;
      }
  }
  csv_fini(&_p, cbCountColumn, cbCountRow, &_item);
  seek(size());
}
bool DFRobot_CSV::read(String *s){
  if(!s) return false;
  uint16_t temp = 0;
  *s = "";
  seek(0);
  while((temp = readBuf(_readBuf, CSV_READ_BUFFER))){
      for(uint16_t i = 0; i < temp; i++){
          *s += (char)_readBuf[i];
      }
  }
  seek(size());
  return true;
}
String DFRobot_CSV::readRaw(int raw){
  String s = "";
  readRaw(raw, &s);
  seek(size());
  return s;
}
bool DFRobot_CSV::readRaw(int raw, String *s){//获取行的起始位置和结束位置
  if(!raw || !s) return false;
  *s = "";
  memset(&_item, 0, sizeof(sItem_t));
  _item.cmd = CSV_CMD_READ_RAW;
  _item.raw = raw;
  uint16_t temp = 0,left = 0;
  seek(0);
  while((_item.rawIndex < raw) && (temp = readBuf(_readBuf, CSV_READ_BUFFER))){
      _item.posIndex = position() - temp;
      if(csv_parse(&_p, _readBuf, temp, cbCountColumn, cbCountRow, &_item) != temp){
          seek(size());
          return false;
      }
  }
  csv_fini(&_p, cbCountColumn, cbCountRow, &_item);
  if(raw == getRaw()) _item.endPos = position() - 1;
  left = _item.endPos - _item.startPos + 1;
  seek(_item.startPos);
  while(left){
      left > CSV_READ_BUFFER ? temp = CSV_READ_BUFFER : temp = left;
      left -= temp;
      if(readBuf(_readBuf, temp) != temp){
          seek(size());
          return false;
      }
      for(uint16_t i = 0; i < temp; i++){
          *s += (char)_readBuf[i];
      }
  }
  seek(size());
  return true;
}
bool DFRobot_CSV::writeRaw(int raw, String s){
  if(raw > getRaw()) return false;
  String rs = readRaw(raw);
  int length = rs.length();
  seek(_item.endPos);
  if(peek() == '\n') length -= 2;
  char *pBuf = (char *)s.c_str();
  if(s.length() > length){//增加
      seek(_item.startPos);
      writeBuf(pBuf, length);
      pBuf += length;
      insert(_item.startPos+length, pBuf, (s.length() - length));
  }else{//覆盖和删除
      seek(_item.startPos);
      writeBuf(pBuf, s.length());
      uint32_t index = position();
      move(_item.startPos + length, index);
  }
  seek(size());
}
bool DFRobot_CSV::readColumn(int col, String *s){
  if(col > getColumn() || !s) return false;
  *s = "";
  memset(&_item, 0, sizeof(sItem_t));
  _item.cmd = CSV_CMD_READ_COL;
  _item.column = col;
  uint16_t temp = 0;
  seek(0);
  while(temp = readBuf(_readBuf, CSV_READ_BUFFER)){
      if(csv_parse(&_p, _readBuf, temp, cbCountColumn, cbCountRow, &_item) != temp){
          seek(size());
          return false;
      }
  }
  csv_fini(&_p, cbCountColumn, cbCountRow, &_item);
  *s += _item.data;
  seek(size());
}
String DFRobot_CSV::readItem(int raw, int column){
  String s = "";
  readItem(raw, column, &s);
  seek(size());
  return s;
}
bool DFRobot_CSV::readItem(int raw, int column, String *s){
  if(raw > getRaw() || !s) return false;
  *s = "";
  memset(&_item, 0, sizeof(sItem_t));
  _item.cmd = CSV_CMD_READ_ITEM;
  _item.raw = raw;
  _item.column = column;
  uint16_t temp = 0;
  seek(0);
  while((_item.rawIndex < raw) && (temp = readBuf(_readBuf, CSV_READ_BUFFER))){
      _item.posIndex = position() - temp;
      if(csv_parse(&_p, _readBuf, temp, cbCountColumn, cbCountRow, &_item) != temp){
          seek(size());
          return false;
      }
  }
  csv_fini(&_p, cbCountColumn, cbCountRow, &_item);
  *s += _item.data;
  seek(size());
}

int DFRobot_CSV::getRaw(){
  update();
  return _item.rawMax;
}
int DFRobot_CSV::getColumn(){
  update();
  return _item.colMax;
}
int DFRobot_CSV::readMaxColumnOfRaw(int raw){
  if(raw > getRaw()) return 0;
  memset(&_item, 0, sizeof(sItem_t));
  _item.cmd = CSV_CMD_GET_MAXCOLOFRAW;
  _item.raw = raw;
  uint16_t temp = 0;
  seek(0);
  while(temp = readBuf(_readBuf, CSV_READ_BUFFER)){
      if(csv_parse(&_p, _readBuf, temp, cbCountColumn, cbCountRow, &_item) != temp){
          seek(size());
          return false;
      }
  }
  csv_fini(&_p, cbCountColumn, cbCountRow, &_item);
  seek(size());
  return _item.column;
}
size_t DFRobot_CSV::write(uint8_t val)
{
  return write(&val, 1);
}

size_t DFRobot_CSV::write(const uint8_t *buf, size_t size)
{
  if(!buf) return 0;
  uint8_t *pBuf = (uint8_t *)buf;
  if((size > 1)&&(pBuf[size - 1] == '\n')&&(pBuf[size - 2] == '\r')){
      uint32_t pos = position();
      if((size == 2)&& (pos != 0)){
          seek(pos - 1);
          if(peek() != ',') seek(pos);
      }
      _flag = 0;
  }
  uint32_t left = size + _flag;
  while(left){
      left > CSV_WRITE_BUFFER ? size = CSV_WRITE_BUFFER : size = left;
      left -= size;
      memset(_writeBuf, 0, sizeof(_writeBuf));
      if((size < CSV_WRITE_BUFFER) && _flag){
          memcpy(_writeBuf, pBuf, size - 1);
          _writeBuf[size - 1] = ',';
      }else{
          memcpy(_writeBuf, pBuf, size);
      }
      writeBuf(_writeBuf, size);
      pBuf += size;
  }
  if(!_flag) _flag = 1;
  return left;
}

size_t DFRobot_CSV::print(long n, int base)
{
  int t = 0;
  char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];
  *str = '\0';
  // prevent crash if called with base == 1
  if (base < 2) base = 10;
  if(n < 0){
      _flag = 0;
      print('-');
      n = -n;
      t += 1;
  }
  do {
    char c = n % base;
    n /= base;

    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);
  return t + write(str);
}
size_t DFRobot_CSV::println(long n, int base){
  size_t t = print(n, base);
  t += Print::println();
  return t;
}
size_t DFRobot_CSV::print(double number, int digits){
  size_t n = 0;
  if (isnan(number)) return Print::print("nan");
  if (isinf(number)) return Print::print("inf");
  if (number > 4294967040.0) return Print::print ("ovf");  // constant determined empirically
  if (number <-4294967040.0) return Print::print ("ovf");  // constant determined empirically
  String str = "";
  if (number < 0.0)
  {
     str += '-';
     n += 1;
     number = -number;
  }
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  number += rounding;
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  str += int_part;
  // Print the decimal point, but only if there are digits beyond
  if (digits > 0){
    str += '.';
  }
  while (digits-- > 0)
  {
    remainder *= 10.0;
    unsigned int toPrint = (unsigned int)(remainder);
    str += toPrint;
    remainder -= toPrint; 
  } 
  Print::print(str);
  return n;
}
size_t DFRobot_CSV::println(double num, int digits){
  size_t n = print(num, digits);
  n += Print::println();
  return n;
}

//int DFRobot_CSV::setPosition(int row, int column, String s){
  
//}
