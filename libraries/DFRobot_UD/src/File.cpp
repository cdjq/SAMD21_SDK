/*!
 * @file File.cpp
 * @brief This library aims to expose a subset of USB Disk functionality
 * @n in the form of a higher level "wrapper" object.
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence GNU General Public License V3
          (Because sdfatlib is licensed with this.)
 */

#include <UD.h>

namespace UDLib{

File::File(UdFile f, const char *n){
  _file = (UdFile *)malloc(sizeof(UdFile)); 
  if (_file) {
    memcpy(_file, &f, sizeof(UdFile));
    
    strncpy(_name, n, 12);
    _name[12] = 0;
  }
  String str = "UD:";
  str.toCharArray(_storageType, str.length()+1);
}

File::File(void) {
  _file = 0;
  _name[0] = 0;
  String str = "UD:";
  str.toCharArray(_storageType, str.length()+1);
}

char *File::name(void) {
  strncpy(_storageType+strlen(_storageType), _name, sizeof(_name));
  return _storageType;
}

char *File::getName(){
  return _name;
}

boolean File::isDirectory(void) {
  return (_file && _file->isDir());
}


size_t File::write(uint8_t val) {
  return write(&val, 1);
}

size_t File::write(const uint8_t *buf, size_t size) {
  size_t t;
  if (!_file) {
    setWriteError();
    return 0;
  }
  _file->clearWriteError();
  t = _file->write(buf, size);
  if (_file->getWriteError()) {
    setWriteError();
    return 0;
  }
  return t;
}

int File::peek() {
  if (! _file) 
    return 0;

  int c = _file->read();
  if (c != -1) _file->seekCur(-1);
  return c;
}

int File::read() {
  if (_file) 
    return _file->read();
  return -1;
}

int File::read(void *buf, uint16_t nbyte) {
  if (_file) 
    return _file->read(buf, nbyte);
  return 0;
}

int File::available() {
  if (! _file) return 0;

  uint32_t n = size() - position();

  return n > 0X7FFF ? 0X7FFF : n;
}

void File::flush() {
  if (_file)
    _file->sync();
}

boolean File::seek(uint32_t pos) {
  if (! _file) return false;

  return _file->seekSet(pos);
}

uint32_t File::position() {
  if (! _file) return -1;
  return _file->curPosition();
}

uint32_t File::size() {
  if (! _file) return 0;
  return _file->fileSize();
}

void File::close() {
  if (_file) {
    _file->close();
    free(_file); 
    _file = 0;
  }
}

File::operator bool() {
  if (_file) 
    return  _file->isOpen();
  return false;
}
};
