#include "ComFile.h"
CommonFile::CommonFile(void *file)
  :_type(0),_file(file){
  char *nameType = ((UDFile *)_file)->name();
  String str = String(nameType);
  if(str.length()>3){
        String sType = str.substring(0, 3);
        if((sType == "UD:") && (sType.length() < str.length())){
            _type = TYPE_CSV_UD;
        }else{
            _type = TYPE_CSV_SD;
        }
    }else{
        _type = TYPE_CSV_SD;
    }
}
CommonFile::CommonFile(): _type(0),_file(NULL){}

size_t CommonFile::writeBuf(void *pBuf, size_t nbyte){
  if((!pBuf) || (!_file)) return 0;
  uint8_t *buf = (uint8_t *)pBuf;
  switch(_type){
      case TYPE_CSV_UD:
           return ((UDFile *)_file)->write(buf, nbyte);
      case TYPE_CSV_SD:
           return ((File *)_file)->write(buf, nbyte);
  }
}
int CommonFile::readBuf(void *pBuf, uint16_t nbyte){
  if(!pBuf || (!_file)) return 0;
  uint8_t *buf = (uint8_t *)pBuf;
  switch(_type){
      case TYPE_CSV_UD:
           return ((UDFile *)_file)->read(buf, nbyte);
      case TYPE_CSV_SD:
           return ((File *)_file)->read(buf, nbyte);
  }
}

int CommonFile::peek(){
  if(!_file) return 0;
  switch(_type){
      case TYPE_CSV_UD:
           return ((UDFile *)_file)->peek();
      case TYPE_CSV_SD:
           return ((File *)_file)->peek();
  }
}
int CommonFile::available(){
  if(!_file) return 0;
  switch(_type){
      case TYPE_CSV_UD:
           return ((UDFile *)_file)->available();
      case TYPE_CSV_SD:
           return ((File *)_file)->available();
  }
}
void CommonFile::flush(){
  if(!_file) return;
  switch(_type){
      case TYPE_CSV_UD:
           ((UDFile *)_file)->flush();
      case TYPE_CSV_SD:
           ((File *)_file)->flush();
  }
}
boolean CommonFile::seek(uint32_t pos){
  if(!_file) return false;
  switch(_type){
      case TYPE_CSV_UD:
           return ((UDFile *)_file)->seek(pos);
      case TYPE_CSV_SD:
           return ((File *)_file)->seek(pos);
  }
}
uint32_t CommonFile::position(){
  if(!_file) return 0;
  switch(_type){
      case TYPE_CSV_UD:
           return ((UDFile *)_file)->position();
      case TYPE_CSV_SD:
           return ((File *)_file)->position();
  }
}
uint32_t CommonFile::size(){
  if(!_file) return 0;
  switch(_type){
      case TYPE_CSV_UD:
           return ((UDFile *)_file)->size();
      case TYPE_CSV_SD:
           return ((File *)_file)->size();
  }
}
char * CommonFile::name(){
  if(!_file) return 0;
  switch(_type){
      case TYPE_CSV_UD:
           return ((UDFile *)_file)->getName();
      case TYPE_CSV_SD:
           return ((File *)_file)->name();
  }
}
uint8_t CommonFile::type(){
  switch(_type){
      case TYPE_CSV_UD:
           return TYPE_CSV_UD;
      case TYPE_CSV_SD:
           return TYPE_CSV_SD;
      default:
           return 0;
  }
}
bool CommonFile::insert(uint32_t index, void *buf, size_t nbyte){
  if(!buf) return false;
  if(index > size()) index = size();
  uint8_t *pBuf = (uint8_t *)buf;
  uint16_t temp = 0;
  seek(index);
  uint32_t left = available();
  if(left) move(index, index + nbyte);
  left = nbyte;
  seek(index);
  while(left){
      left > 512 ? temp = 512 : temp = left;
      left -= temp;
      writeBuf(pBuf, temp);
      pBuf += temp;
  }
}
bool CommonFile::move(uint32_t startIndex, uint32_t destIndex){
  if(startIndex > size()) return false;
  seek(startIndex);
  uint32_t left = available();
  if(destIndex > (startIndex + left)) return false;
  if((startIndex == destIndex) || (startIndex == size())) return true;
  uint8_t buf[512];
  uint16_t temp = 0;
  memset(buf, 0, sizeof(buf));
  if(startIndex > destIndex){//前移
      while(left){
          left > 512 ? temp = 512 :temp = left;
          left -= temp;
          seek(startIndex);
          readBuf(buf, temp);
          startIndex += temp;
          seek(destIndex);
          writeBuf(buf, temp);
          destIndex += temp;
      }
      left = available();
      while(left){
         left > 512 ? temp = 512 :temp = left;
         left -= temp;
         memset(buf,' ',temp);
         writeBuf(buf, temp);
      }
  }else{//后移
      destIndex += left;
      startIndex += left;
      while(left){
          left > 512 ? temp = 512 :temp = left;
          left -= temp;
          startIndex -= temp;
          seek(startIndex);
          readBuf(buf, temp);
          destIndex -= temp;
          seek(destIndex);
          writeBuf(buf, temp);
      }
  }
}
/*
bool CommonFile::deleteChar(uint32_t startIndex, uint32_t length){
  if((startIndex > size()) || (length == 0)) return false;
  switch(_type){
	  case TYPE_CSV_UD:
	      char *c = name();
          String str = String(c);
		  str += 1;
		  uint8_t i = 1;
		  while(UD.exists(str)){
			  str += i;
		  }
	      UDFile myFile = UD.open(str.c_str(), FILE_WRITE);
		  uint32_t left = startIndex - 0;
		  uint16_t temp;
		  uint8_t buf[512];
		  memset(buf, 0, 512);
		  seek(0);
		  while(left){
			  left > 512 ? temp = 512 : temp = left;
			  left -= temp;
			  readBuf(buf, temp);
			  myFile.write(buf, temp);
		  }
		  seek(startIndex+length);
		  left = available();
		  while(left){
			  left > 512 ? temp = 512 : temp = left;
			  left -= temp;
			  readBuf(buf, temp);
			  myFile.write(buf, temp);
		  }
		  UD.remove(c);
		  ((UDFile *)_file) = UD.open(c, FILE_WRITE);
		  myFile.seek(0);
		  left = myFile.available();
		  while(left){
			  left > 512 ? temp = 512 : temp = left;
			  left -= temp;
			  myFile.read(buf, temp);
			  writeBuf(buf, temp);
		  }
		  UD.remove(str.c_str());
	      break;
	  case TYPE_CSV_SD:
	      break;
  }
}
*/