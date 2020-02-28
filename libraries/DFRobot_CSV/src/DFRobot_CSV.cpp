
#include <math.h>



#include <DFRobot_CSV.h>

DFRobot_CSV::DFRobot_CSV()
{
	csv_init(&_p,0);
	_file = NULL;
}

DFRobot_CSV::DFRobot_CSV(File &file)
{
	csv_init(&_p,0);
	_file = & file;
}


DFRobot_CSV::~DFRobot_CSV()
{
	if(_p.entry_buf)
	_p.free_func(_p.entry_buf);
}


size_t DFRobot_CSV::write(uint8_t val)
{
  return write(&val, 1);
}

size_t DFRobot_CSV::write(const uint8_t *buf, size_t size)
{
  return _file->write(buf,size);
}

size_t DFRobot_CSV::print(const String &s)
{
  int i,j,t;
  void *te = malloc(s.length()*2+3);
  char * temp = (char *)te;
  temp[0] = '"';
  for(i=0,j=1;i<s.length();i++) {
	  if(*(s.c_str()+i) == '"') {
          temp[j++] = '"';
	  }
	  temp[j++] = *(s.c_str()+i);	  
  }
  temp[j++] = '"';
  temp[j++] = ',';
  t = write(temp,j);
  free(temp);
  return t;
}

size_t DFRobot_CSV::print(const char str[])
{
  int i,j,t;
  void *te = malloc(strlen(str)*2+3);
  char * temp = (char *)te;
  temp[0] = '"';
  for(i=0,j=1;i<strlen(str);i++) {
	  if(str[i] == '"') {
          temp[j++] = '"';
	  }
	  temp[j++] = str[i];	  
  }
  temp[j++] = '"';
  temp[j++] = ',';
  t = write(temp,j);
  free(temp);
  return t;
}

size_t DFRobot_CSV::print(char c)
{
  int i = 1;
  char temp[5];
  temp[0] = '"';
  if(c == '"')
	  temp[i++] = '"';
  temp[i++] = c;
  temp[i++] = '"';
  temp[i++] = ',';
  return write(temp,i);
}

size_t DFRobot_CSV::print(unsigned char b, int base)
{
  return print((unsigned long) b, base);
}

size_t DFRobot_CSV::print(int n, int base)
{
  return print((long) n, base);
}

size_t DFRobot_CSV::print(unsigned int n, int base)
{
  return print((unsigned long) n, base);
}

size_t DFRobot_CSV::print(long n, int base)
{
  size_t tem;
  if (base == 0) {
    tem = write(n);
	tem += write(',');
	return tem;
  } else if (base == 10) {
    if (n < 0) {
      int t = print('-');
      n = -n;
      tem = printNumber(n, 10) + t;
	  tem += write(',');
	  return tem;
    }
    tem = printNumber(n, 10);
	tem += write(',');
	return tem;
  } else {
    tem = printNumber(n, base);
	tem += write(',');
	return tem;
  }
}

size_t DFRobot_CSV::print(unsigned long n, int base)
{
  size_t t;
  if (base == 0) {t = write(n);t += write(',');return t;}
  else {t =  printNumber(n, base);t += write(',');return t;}
}

size_t DFRobot_CSV::print(double n, int digits)
{
  size_t t;
  t = printFloat(n, digits);
  t += write(',');
  return t;
}

size_t DFRobot_CSV::println(void)
{
  return write("\r\n");
}

size_t DFRobot_CSV::println(const String &s)
{
  int i,j,t;
  void *te = malloc(s.length()*2+4);
  char * temp = (char *)te;
  temp[0] = '"';
  for(i=0,j=1;i<s.length();i++) {
	  if(*(s.c_str()+i) == '"') {
          temp[j++] = '"';
	  }
	  temp[j++] = *(s.c_str()+i);	  
  }
  temp[j++] = '"';
  temp[j++] = '\r';
  temp[j++] = '\n';
  t = write(temp,j);
  free(temp);
  return t;
}

size_t DFRobot_CSV::println(const char str[])
{
  int i,j,t;
  void *te = malloc(strlen(str)*2+4);
  char * temp = (char *)te;
  temp[0] = '"';
  for(i=0,j=1;i<strlen(str);i++) {
	  if(str[i] == '"') {
          temp[j++] = '"';
	  }
	  temp[j++] = str[i];	  
  }
  temp[j++] = '"';
  temp[j++] = '\r';
  temp[j++] = '\n';
  t = write(temp,j);
  free(temp);
  return t;
}

size_t DFRobot_CSV::println(char c)
{
  int i = 1;
  char temp[6];
  temp[0] = '"';
  if(c == '"')
	  temp[i++] = '"';
  temp[i++] = c;
  temp[i++] = '"';
  temp[i++] = '\r';
  temp[i++] = '\n';
  return write(temp,i);
}

size_t DFRobot_CSV::println(unsigned char b, int base)
{
  return println((unsigned long) b, base);
}

size_t DFRobot_CSV::println(int num, int base)
{
  return println((long) num, base);
}

size_t DFRobot_CSV::println(unsigned int num, int base)
{
  return println((unsigned long) num, base);
}

size_t DFRobot_CSV::println(long num, int base)
{
  size_t tem;
  if (base == 0) {
    tem = write(num);
	tem += write("\r\n");
	return tem;
  } else if (base == 10) {
    if (num < 0) {
      int t = print('-');
      num = -num;
      tem = printNumber(num, 10) + t;
	  tem += write("\r\n");
	  return tem;
    }
    tem = printNumber(num, 10);
	tem += write("\r\n");
	return tem;
  } else {
    tem = printNumber(num, base);
	tem += write("\r\n");
	return tem;
  }
}

size_t DFRobot_CSV::println(unsigned long n, int base)
{
  size_t t;
  if (base == 0) {t = write(n);t += write("\r\n");return t;}
  else {t =  printNumber(n, base);t += write("\r\n");return t;}
}

size_t DFRobot_CSV::println(double num, int digits)
{
  size_t n = print(num, digits);
  n += println();
  return n;
}


// Private Methods /////////////////////////////////////////////////////////////

size_t DFRobot_CSV::printNumber(unsigned long n, uint8_t base)
{
  char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

  do {
    char c = n % base;
    n /= base;

    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);

  return write(str);
}

size_t DFRobot_CSV::printFloat(double number, uint8_t digits) 
{ 
  size_t n = 0;
  
  if (isnan(number)) return print("nan");
  if (isinf(number)) return print("inf");
  if (number > 4294967040.0) return print ("ovf");  // constant determined empirically
  if (number <-4294967040.0) return print ("ovf");  // constant determined empirically
  
  // Handle negative numbers
  if (number < 0.0)
  {
     n += write('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  n += printNumber(int_part,DEC);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0) {
    n += write('.'); 
  }

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    unsigned int toPrint = (unsigned int)(remainder);
    n += printNumber(toPrint, DEC);
    remainder -= toPrint; 
  } 
  
  return n;
}

/*
DFRobot_CSV::operator bool() {
  return (_file->available());
}
*/

static struct {
	size_t column;
	size_t row;
}_flags;

typedef struct{
//	sp_t *point;
    String *sp;
	size_t row;
	size_t column;
} sRead_t;

static void cbCountRow(int c, void *)
{
	_flags.row++;
}

int DFRobot_CSV::getRow()
{ 
  uint8_t readBuf[512] = {0};
  uint16_t temp;
  memset(&_flags,0,sizeof(_flags));
   _file->seek(0);
   while((temp =_file->read(readBuf, 512)) > 0) {
      if(csv_parse(&_p,readBuf,temp,NULL,cbCountRow,NULL)!=temp) {
        _file->close();
		return -1;
	  }
    }
    csv_fini(&_p,NULL,cbCountRow,NULL);     
    csv_free(&_p);
	return _flags.row;
}

static void cbCountColumn(void *s, size_t i, void *data)
{
	if(_flags.row == ((sRead_t *)data)->row - 1) 
	    _flags.column++;
}


int DFRobot_CSV::getColumn(int row)
{
	sRead_t data = {0,row};  
	uint8_t readBuf[512] = {0};
	uint16_t temp;
	String str;
	memset(&_flags,0,sizeof(_flags));
	_file->seek(0);
    while((_flags.row < row) && (temp =_file->read(readBuf, 512))) {
        if(csv_parse(&_p,readBuf,temp,cbCountColumn,cbCountRow,&data)!=temp) {
          _file->close();
		  return -1;
		}
    }
    csv_fini(&_p,cbCountColumn,cbCountRow,&data);
	csv_free(&_p);
	return _flags.column;
}


static void cbAftFieldCount(void *s, size_t i, void *data) {
    if(_flags.row == ((sRead_t *)data)->row - 1) {
	    _flags.column++;
		if(_flags.column == (*(sRead_t *)data).column) {
		    char arr[i+1];
		    for(int j=0;j<i;j++)
			    arr[j] = *((char*)s+j);
		    arr[i] = 0;
		    String string(arr);
		    void *p = calloc(1,sizeof(String));
		    *((String *)p) = string;
			((sRead_t *)data)->sp = (String *)p;
		}
    }
}

String DFRobot_CSV::readItem(int row, int column)
{
	sRead_t data = {0,row,column};  
	uint8_t readBuf[512] = {0};
	uint16_t temp;
	String str;
	memset(&_flags,0,sizeof(_flags));
	_file->seek(0);
    while((_flags.row < row) && (temp =_file->read(readBuf, 512))) {
        if(csv_parse(&_p,readBuf,temp,cbAftFieldCount,cbCountRow,&data)!=temp) {
          _file->close();
		  return String();
		}
    }
    csv_fini(&_p,cbAftFieldCount,cbCountRow,&data);
	if(data.sp) {
	    str = *(data.sp);
	    free((char *)(data.sp->c_str()));
	    free(data.sp);
	    data.sp = NULL;
	}
    csv_free(&_p);
	return str;
}

static void cbReadColumn(void *s, size_t i, void *data)
{   
	if(_flags.row == ((sRead_t *)data)->row-1) {
		char arr[i+1];
		for(int j=0;j<i;j++)
			arr[j] = *((char*)s+j);
		arr[i] = 0;
		String string(arr);
		((sRead_t *)data)->sp[_flags.column] = string;
		_flags.column++;
//		Serial.print("string");Serial.println(string);
/*		void *p = calloc(1,sizeof(sp_t));
		
		((sp_t *)p)->pt += string;
/*		((sp_t *)p)->pt.buffer = (char *)malloc(i+1);
		((sp_t *)p)->pt.capacity = i;
		if(i == 0) buffer[0] = 0;
		((sp_t *)p)->pt.len = i;
		strcpy(buffer, arr);*/

//		memcpy(p,&string,sizeof(string));
/*        if(((sRead_t *)data)->point) {
			sp_t *plast = ((sRead_t *)data)->point;
			while((*plast).ppt) {
				plast = (*plast).ppt;
			}
			(*plast).ppt = (sp_t *)p;
		}else {
			((sRead_t *)data)->point = (sp_t*)p;
		}*/
	}
}


bool DFRobot_CSV::readRow(int row, String * ss)
{
	uint8_t readBuf[512] = {0};
	uint16_t temp;
	String str;
	sRead_t data = {ss,row};
	memset(&_flags,0,sizeof(_flags));
	_file->seek(0);
	while((_flags.row < row) && (temp = _file->read(readBuf, 512))) {
        if(csv_parse(&_p, readBuf, temp, cbReadColumn, cbCountRow, &data) != temp) {
          _file->close();
		  return 1;
		}
	}
	csv_fini(&_p, cbReadColumn, cbCountRow, &data);
/*	if(data.point) {
		plast = data.point;
		do {
			str += (*plast).pt;
		    plast = (*plast).ppt;
			if(plast)
			    str += String(',');
		   }while(plast);	
	}
	while(data.point) {
		pll = plast = data.point;
		while((*plast).ppt) {
			pll = plast;
			plast = (*plast).ppt;
		}
		free((char *)((*plast).pt.c_str()));
		pll->ppt = NULL;
		free(plast);
		if(plast == data.point)
			data.point = NULL;
	}*/
	csv_free(&_p);
	return 0;
}