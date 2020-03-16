/*!
 * @file UD.h
 * @brief This library aims to expose a subset of UD disk functionality
 * @n in the form of a higher level "wrapper" object.
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence GNU General Public License V3
          (Because sdfatlib is licensed with this.)
 */
#ifndef __UD_H
#define __UD_H

#include <Arduino.h>
#include <W25Q.h>

#include "utility/UdFat.h"
#ifdef ARDUINO_SAM_ZERO
#include <variant.h>
#endif
#define FILE_READ UDLib::O_READ
#define FILE_WRITE (UDLib::O_READ | UDLib::O_WRITE | UDLib::O_CREAT | UDLib::O_APPEND)

#define STORAGE_TYPE_UD 3
namespace UDLib {

class File : public Stream {
 private:
  char _name[13]; // file name
  UdFile *_file;  // underlying file pointer
  char _storageType[16];//StorageType+file name

public:
  /**
   * @brief wraps an underlying UdFile.
   * @param f the Object of UdFile class.
   * @param name file Absolute path name
   */
  File(UdFile f, const char *name);
  /**
   * @brief 'empty' constructor.
   */
  File(void);
  /**
   * @brief Write one byte into file. 
   * @return Returns the size in bytes written
   */
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buf, size_t size);
   /**
    * @brief Read 1 byte in file
    * @return Return the readings
    */
  virtual int read();
  /**
   * @brief Read 1 byte in file. Reads the value at the same position in the file.
   * @return Return the readings
   */
  virtual int peek();
  /**
   * @brief Get the number of bytes in file.
   * @return Return the number of bytes in receive buffer
   */
  virtual int available();
  /**
   * @brief Wait for the data to be transmited completely
   */
  virtual void flush();
  /**
   * @brief Read n byte in file
   * @n This operation doesn't involve receive buffer. 
   * @param buf Array for storing data 
   * @param nbyte The number of character to be read
   * @return Return the readings
   */
  int read(void *buf, uint16_t nbyte);
  /**
   * @brief Sets a file's position.
   * @param pos pos The new position in bytes from the beginning of the file.
   * @return true, is returned for success and the value zero, false, is returned for failure.
   */
  boolean seek(uint32_t pos);
  /**
   * @brief The current position for a file or directory.
   * @return The current position.
   */
  uint32_t position();
  /**
   * @brief The total number of bytes in a file or directory.
   * @return file size.
   */
  uint32_t size();
  /**
   * @brief close the file.
   */
  void close();
  operator bool();
  /**
   * @brief Strorage type and thehe file or directory name.
   * @return The Strorage type and file or directory name.
   */
  char * name();
  /**
   * @brief The file or directory.
   * @return The file or directory.
   */
  char * getName();
  /**
   * @brief Determine if the current file is a directory or a file.
   * @return True if this is a UdFile for a directory else false.
   */
  boolean isDirectory(void);
  /**
   * @brief Open all files or directories in the current directory
   * @return Return file object.
   */
  File openNextFile(uint8_t mode = O_RDONLY);
  /**
   * @brief Set the file's current position to zero.
   */
  void rewindDirectory(void);
  
  using Print::write;
};

class UDClass {

private:
  UdDisk disk;
  UdVolume volume;
  UdFile root;
  
  UdFile getParentDir(const char *filepath, int *indx);
public:
  /**
   * @brief UD disk initialized.
   * @param csPin UD chip select pin.
   * @return Successfully returns true.
   */
  boolean begin(uint8_t csPin = UD_CHIP_SELECT_PIN);
  /**
   * @brief UD disk initialized.
   * @param clock SPI clock rate.
   * @param csPin UD chip select pin.
   * @return Successfully returns true.
   */
  boolean begin(uint32_t clock, uint8_t csPin);
  
  /**
   * @brief call this when a disk is removed. 
   * @n It will allow you to insert and initialise a new card.
   */
  void end();

  // Open the specified file/directory with the supplied mode (e.g. read or
  // write, etc). Returns a File object for interacting with the file.
  // Note that currently only one file can be open at a time.
  /**
   * @brief Open the specified file/directory with the supplied mode.
   * @param filename file or directory.
   * @param mode Open mode,FILE_WRITE or FILE_READ.
   * @return A File object for interacting with the file.
   */
  File open(const char *filename, uint8_t mode = FILE_READ);
  File open(const String &filename, uint8_t mode = FILE_READ) { return open( filename.c_str(), mode ); }

  /**
   * @brief Methods to determine if the requested file path exists.
   * @param filepath file or directory path.
   * @return Exist is return true, otherwise return false.
   */
  boolean exists(const char *filepath);
  boolean exists(const String &filepath) { return exists(filepath.c_str()); }

  /**
   * @brief Create the requested directory heirarchy--if intermediate directories
   * @ do not exist they will be created.
   * @param filepath file or directory path.
   * @return Succuss is return true, otherwise return false.
   */
  boolean mkdir(const char *filepath);
  boolean mkdir(const String &filepath) { return mkdir(filepath.c_str()); }
  
  /**
   * @brief Delete the file.
   * @param filepath file or directory path.
   * @return Succuss is return true, otherwise return false.
   */
  boolean remove(const char *filepath);
  boolean remove(const String &filepath) { return remove(filepath.c_str()); }
  /**
   * @brief Delete the file.
   * @param filepath file or directory path.
   * @return Succuss is return true, otherwise return false.
   */
  boolean rmdir(const char *filepath);
  boolean rmdir(const String &filepath) { return rmdir(filepath.c_str()); }

private:

  // This is used to determine the mode used to open a file
  // it's here because it's the easiest place to pass the 
  // information through the directory walking function. But
  // it's probably not the best place for it.
  // It shouldn't be set directly--it is set via the parameters to `open`.
  int fileOpenMode;
  
  friend class File;
  friend boolean callback_openPath(UdFile&, const char *, boolean, void *); 
};
};

typedef UDLib::File    UDFile;
typedef UDLib::UDClass  UDFileSystemClass;
extern UDLib::UDClass UD;
#define UDFileSystem   UDLib::UD

#endif
