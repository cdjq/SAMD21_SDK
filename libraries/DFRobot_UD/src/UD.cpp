/*!
 * @file UD.h
 * @brief This library aims to expose a subset of UD disk functionality
 * @n in the form of a higher level "wrapper" object.
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence GNU General Public License V3
          (Because sdfatlib is licensed with this.)
 */

#include "UD.h"

namespace UDLib {

// Used by `getNextPathComponent`
#define MAX_COMPONENT_LEN 12 // What is max length?
#define PATH_COMPONENT_BUFFER_LEN MAX_COMPONENT_LEN+1

bool getNextPathComponent(const char *path, unsigned int *p_offset,
              char *buffer) {
  /*
    Parse individual path components from a path.

      e.g. after repeated calls '/foo/bar/baz' will be split
           into 'foo', 'bar', 'baz'.

    This is similar to `strtok()` but copies the component into the
    supplied buffer rather than modifying the original string.


    `buffer` needs to be PATH_COMPONENT_BUFFER_LEN in size.

    `p_offset` needs to point to an integer of the offset at
    which the previous path component finished.

    Returns `true` if more components remain.

    Returns `false` if this is the last component.
      (This means path ended with 'foo' or 'foo/'.)

   */

  // TODO: Have buffer local to this function, so we know it's the
  //       correct length?

  int bufferOffset = 0;

  int offset = *p_offset;

  // Skip root or other separator
  if (path[offset] == '/') {
    offset++;
  }
  // Copy the next next path segment
  while (bufferOffset < MAX_COMPONENT_LEN
     && (path[offset] != '/')
     && (path[offset] != '\0')) {
    buffer[bufferOffset++] = path[offset++];
  }

  buffer[bufferOffset] = '\0';

  // Skip trailing separator so we can determine if this
  // is the last component in the path or not.
  if (path[offset] == '/') {
    offset++;
  }

  *p_offset = offset;

  return (path[offset] != '\0');
}



boolean walkPath(const char *filepath, UdFile& parentDir,
         boolean (*callback)(UdFile& parentDir,
                     const char *filePathComponent,
                     boolean isLastComponent,
                     void *object),
         void *object = NULL) {
  /*
     
     When given a file path (and parent directory--normally root),
     this function traverses the directories in the path and at each
     level calls the supplied callback function while also providing
     the supplied object for context if required.

       e.g. given the path '/foo/bar/baz'
            the callback would be called at the equivalent of
        '/foo', '/foo/bar' and '/foo/bar/baz'.

     The implementation swaps between two different directory/file
     handles as it traverses the directories and does not use recursion
     in an attempt to use memory efficiently.

     If a callback wishes to stop the directory traversal it should
     return false--in this case the function will stop the traversal,
     tidy up and return false.

     If a directory path doesn't exist at some point this function will
     also return false and not subsequently call the callback.

     If a directory path specified is complete, valid and the callback
     did not indicate the traversal should be interrupted then this
     function will return true.

   */


  UdFile subfile1;
  UdFile subfile2;

  char buffer[PATH_COMPONENT_BUFFER_LEN]; 

  unsigned int offset = 0;

  UdFile *p_parent;
  UdFile *p_child;

  UdFile *p_tmp_sdfile;  
  
  p_child = &subfile1;
  
  p_parent = &parentDir;

  while (true) {

    boolean moreComponents = getNextPathComponent(filepath, &offset, buffer);

    boolean shouldContinue = callback((*p_parent), buffer, !moreComponents, object);

    if (!shouldContinue) {
      // TODO: Don't repeat this code?
      // If it's one we've created then we
      // don't need the parent handle anymore.
      if (p_parent != &parentDir) {
        (*p_parent).close();
      }
      return false;
    }
    
    if (!moreComponents) {
      break;
    }
    
    boolean exists = (*p_child).open(*p_parent, buffer, O_RDONLY);

    // If it's one we've created then we
    // don't need the parent handle anymore.
    if (p_parent != &parentDir) {
      (*p_parent).close();
    }
    
    // Handle case when it doesn't exist and we can't continue...
    if (exists) {
      // We alternate between two file handles as we go down
      // the path.
      if (p_parent == &parentDir) {
        p_parent = &subfile2;
      }

      p_tmp_sdfile = p_parent;
      p_parent = p_child;
      p_child = p_tmp_sdfile;
    } else {
      return false;
    }
  }
  
  if (p_parent != &parentDir) {
    (*p_parent).close(); // TODO: Return/ handle different?
  }

  return true;
}



/*

   The callbacks used to implement various functionality follow.

   Each callback is supplied with a parent directory handle,
   character string with the name of the current file path component,
   a flag indicating if this component is the last in the path and
   a pointer to an arbitrary object used for context.

 */

boolean callback_pathExists(UdFile& parentDir, const char *filePathComponent, 
                boolean /* isLastComponent */, void * /* object */) {
  /*

    Callback used to determine if a file/directory exists in parent
    directory.

    Returns true if file path exists.

  */
  UdFile child;

  boolean exists = child.open(parentDir, filePathComponent, O_RDONLY);
  
  if (exists) {
     child.close(); 
  }
  
  return exists;
}



boolean callback_makeDirPath(UdFile& parentDir, const char *filePathComponent, 
                 boolean isLastComponent, void *object) {
  /*

    Callback used to create a directory in the parent directory if
    it does not already exist.

    Returns true if a directory was created or it already existed.

  */
  boolean result = false;
  UdFile child;
  
  result = callback_pathExists(parentDir, filePathComponent, isLastComponent, object);
  if (!result) {
    result = child.makeDir(parentDir, filePathComponent);
  } 
  
  return result;
}




boolean callback_remove(UdFile& parentDir, const char *filePathComponent, 
            boolean isLastComponent, void * /* object */) {
  if (isLastComponent) {
    return UdFile::remove(parentDir, filePathComponent);
  }
  return true;
}

boolean callback_rmdir(UdFile& parentDir, const char *filePathComponent, 
            boolean isLastComponent, void * /* object */) {
  if (isLastComponent) {
    UdFile f;
    if (!f.open(parentDir, filePathComponent, O_READ)) return false;
    return f.rmDir();
  }
  return true;
}

/* Implementation of class used to create `SDCard` object. */
boolean UDClass::begin(uint8_t csPin) {
  if(root.isOpen()) root.close();
  /* Performs the initialisation required by the sdfatlib library.
   * Return true if initialization succeeds, false otherwise.
   */
  return disk.init( csPin) &&
         volume.init(disk) &&
         root.openRoot(volume);
}

boolean UDClass::begin(uint32_t clock, uint8_t csPin) {
  if(root.isOpen()) root.close();

  return disk.init(csPin,clock) &&
         volume.init(disk) &&
         root.openRoot(volume);
}

//call this when a card is removed. It will allow you to insert and initialise a new card.
void UDClass::end()
{
  root.close();
}

// this little helper is used to traverse paths
UdFile UDClass::getParentDir(const char *filepath, int *index) {
  // get parent directory
  UdFile d1;
  UdFile d2;

  d1.openRoot(volume); // start with the mostparent, root!

  // we'll use the pointers to swap between the two objects
  UdFile *parent = &d1;
  UdFile *subdir = &d2;
  
  const char *origpath = filepath;

  while (strchr(filepath, '/')) {

    // get rid of leading /'s
    if (filepath[0] == '/') {
      filepath++;
      continue;
    }
    
    if (! strchr(filepath, '/')) {
      // it was in the root directory, so leave now
      break;
    }

    // extract just the name of the next subdirectory
    uint8_t idx = strchr(filepath, '/') - filepath;
    if (idx > 12)
      idx = 12;    // dont let them specify long names
    char subdirname[13];
    strncpy(subdirname, filepath, idx);
    subdirname[idx] = 0;

    // close the subdir (we reuse them) if open
    subdir->close();
    if (! subdir->open(parent, subdirname, O_READ)) {
      // failed to open one of the subdirectories
      return UdFile();
    }
    // move forward to the next subdirectory
    filepath += idx;

    // we reuse the objects, close it.
    parent->close();

    // swap the pointers
    UdFile *t = parent;
    parent = subdir;
    subdir = t;
  }

  *index = (int)(filepath - origpath);
  // parent is now the parent diretory of the file!
  return *parent;
}


File UDClass::open(const char *filepath, uint8_t mode) {
  /*

     Open the supplied file path for reading or writing.

     The file content can be accessed via the `file` property of
     the `SDClass` object--this property is currently
     a standard `UdFile` object from `sdfatlib`.

     Defaults to read only.

     If `write` is true, default action (when `append` is true) is to
     append data to the end of the file.

     If `append` is false then the file will be truncated first.

     If the file does not exist and it is opened for writing the file
     will be created.

     An attempt to open a file for reading that does not exist is an
     error.

   */

  int pathidx;

  // do the interative search
  UdFile parentdir = getParentDir(filepath, &pathidx);
  // no more subdirs!

  filepath += pathidx;

  if (! filepath[0]) {
    // it was the directory itself!
    return File(parentdir, "/");
  }

  // Open the file itself
  UdFile file;

  // failed to open a subdir!
  if (!parentdir.isOpen())
    return File();

  if ( ! file.open(parentdir, filepath, mode)) {
    return File();
  }
  // close the parent
  parentdir.close();

  if ((mode & (O_APPEND | O_WRITE)) == (O_APPEND | O_WRITE))
    file.seekSet(file.fileSize());
  return File(file, filepath);
}


boolean UDClass::exists(const char *filepath) {
  /*
   * Returns true if the supplied file path exists.
   */
  return walkPath(filepath, root, callback_pathExists);
}


boolean UDClass::mkdir(const char *filepath) {
  /*
   * Makes a single directory or a heirarchy of directories.
   * A rough equivalent to `mkdir -p`.
   */
  return walkPath(filepath, root, callback_makeDirPath);
}

boolean UDClass::rmdir(const char *filepath) {
  /*
   * Remove a single directory or a heirarchy of directories.
   * A rough equivalent to `rm -rf`.
   */
  return walkPath(filepath, root, callback_rmdir);
}

boolean UDClass::remove(const char *filepath) {
  return walkPath(filepath, root, callback_remove);
}


// allows you to recurse into a directory
File File::openNextFile(uint8_t mode) {
  dir_t p;

  //Serial.print("\t\treading dir...");
  while (_file->readDir(&p) > 0) {

    // done if past last used entry
    if (p.name[0] == DIR_NAME_FREE) {
      //Serial.println("end");
      return File();
    }

    // skip deleted entry and entries for . and  ..
    if (p.name[0] == DIR_NAME_DELETED || p.name[0] == '.') {
      //Serial.println("dots");
      continue;
    }

    // only list subdirectories and files
    if (!DIR_IS_FILE_OR_SUBDIR(&p)) {
      //Serial.println("notafile");
      continue;
    }

    // print file name with possible blank fill
    UdFile f;
    char name[13];
    _file->dirName(p, name);
    //Serial.print("try to open file ");
    //Serial.println(name);

    if (f.open(_file, name, mode)) {
      //Serial.println("OK!");
      return File(f, name);    
    } else {
      //Serial.println("ugh");
      return File();
    }
  }

  //Serial.println("nothing");
  return File();
}

void File::rewindDirectory(void) {  
  if (isDirectory())
    _file->rewind();
  }
};
UDLib::UDClass UD;