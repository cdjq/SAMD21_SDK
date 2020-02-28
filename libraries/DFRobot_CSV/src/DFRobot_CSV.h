#include "csv.h"
#include <WString.h>
#include <Stream.h>
#include <UD.h>
#include <variant.h>

#ifndef DFRobot_CSV_H
#define DFRobot_CSV_H
#define DEC 10
#define HEX 16
#define OCT 8
#ifdef BIN // Prevent warnings if BIN is previously defined in "iotnx4.h" or similar
#undef BIN
#endif
#define BIN 2

class DFRobot_CSV
{
public:

/*
typedef struct plist{
	String pt;
	struct plist *ppt;
} sp_t;*/
 
    DFRobot_CSV();
  //  template <class T>
    DFRobot_CSV(File &file);
    ~DFRobot_CSV();

    size_t write(uint8_t);
    size_t write(const char *str) {
      if (str == NULL) return 0;
      return write((const uint8_t *)str, strlen(str));
    }
    size_t write(const uint8_t *buf, size_t size);
    size_t write(const char *buffer, size_t size) {
      return write((const uint8_t *)buffer, size);
    }

//  size_t print(const __FlashStringHelper *);
    size_t print(const String &);
    size_t print(const char[]);
    size_t print(char);
    size_t print(unsigned char, int = DEC);
    size_t print(int, int = DEC);
    size_t print(unsigned int, int = DEC);
    size_t print(long, int = DEC);
//    size_t print(const Printable&);
    size_t print(unsigned long, int = DEC);
    size_t print(double, int = 2);
  
//  size_t println(const __FlashStringHelper *);
    size_t println(const String &s);
    size_t println(const char[]);
    size_t println(char);
    size_t println(unsigned char, int = DEC);
    size_t println(int, int = DEC);
    size_t println(unsigned int, int = DEC);
    size_t println(long, int = DEC);
    size_t println(unsigned long, int = DEC);
    size_t println(double, int = 2);
//  size_t println(const Printable&);
    size_t println(void);
	

    int getRow();
    int getColumn(int);
    bool readRow(int, String *);
    String readItem(int row, int column);
//    operator bool();
private:
    typedef struct csv_parser sCSVParse_t;
    sCSVParse_t _p;
    File * _file;
    size_t printNumber(unsigned long, uint8_t);
    size_t printFloat(double, uint8_t); 

};


#endif