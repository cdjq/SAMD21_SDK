/* Arduino FAT Library
 * Copyright (C) 2009 by William Greiman
 *
 * This file is part of the Arduino SdFat Library
 *
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Arduino FAT Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include "UdFat.h"
//------------------------------------------------------------------------------
namespace UDLib{
// raw block cache
// init cacheBlockNumber_to invalid SD block number
uint32_t UdVolume::cacheBlockNumber_ = 0XFFFFFFFF;
cache_t  UdVolume::cacheBuffer_;     // 512 byte cache for UdDisk
UdDisk* UdVolume::uDisk_ ;          // pointer to USB Disk object
uint8_t  UdVolume::cacheDirty_ = 0;  // cacheFlush() will write block if true
uint32_t UdVolume::cacheMirrorBlock_ = 0;  // mirror  block for second FAT
//------------------------------------------------------------------------------
// find a contiguous group of clusters
uint8_t UdVolume::allocContiguous(uint32_t count, uint32_t* curCluster) {
  // start of group
  uint32_t bgnCluster;

  // flag to save place to start next search
  uint8_t setStart;

  // set search start cluster
  if (*curCluster) {
    // try to make file contiguous
    bgnCluster = *curCluster + 1;

    // don't save new start location
    setStart = false;
  } else {
    // start at likely place for free cluster
    bgnCluster = allocSearchStart_;

    // save next search start if one cluster
    setStart = 1 == count;
  }
  // end of group
  uint32_t endCluster = bgnCluster;

  // last cluster of FAT
  uint32_t fatEnd = clusterCount_ + 1;

  // search the FAT for free clusters
  for (uint32_t n = 0;; n++, endCluster++) {
    // can't find space checked all clusters
    if (n >= clusterCount_) return false;

    // past end - start from beginning of FAT
    if (endCluster > fatEnd) {
      bgnCluster = endCluster = 2;
    }
    uint32_t f;
    if (!fatGet(endCluster, &f)) return false;

    if (f != 0) {
      // cluster in use try next cluster as bgnCluster
      bgnCluster = endCluster + 1;
    } else if ((endCluster - bgnCluster + 1) == count) {
      // done - found space
      break;
    }
  }
  // mark end of chain
  if (!fatPutEOC(endCluster)) return false;

  // link clusters
  while (endCluster > bgnCluster) {
    if (!fatPut(endCluster - 1, endCluster)) return false;
    endCluster--;
  }
  if (*curCluster != 0) {
    // connect chains
    if (!fatPut(*curCluster, bgnCluster)) return false;
  }
  // return first cluster number to caller
  *curCluster = bgnCluster;

  // remember possible next free cluster
  if (setStart) allocSearchStart_ = bgnCluster + 1;

  return true;
}
//------------------------------------------------------------------------------
uint8_t UdVolume::cacheFlush(void) {
  if (cacheDirty_) {
    if (!uDisk_->writeBlock(cacheBlockNumber_, cacheBuffer_.data)) {
      return false;
    }
    // mirror FAT tables
    if (cacheMirrorBlock_) {
      if (!uDisk_->writeBlock(cacheMirrorBlock_, cacheBuffer_.data)) {
        return false;
      }
      cacheMirrorBlock_ = 0;
    }
    cacheDirty_ = 0;
  }
  return true;
}
//------------------------------------------------------------------------------
uint8_t UdVolume::cacheRawBlock(uint32_t blockNumber, uint8_t action) {
  if (cacheBlockNumber_ != blockNumber) {
    if (!cacheFlush()) return false;
    if (!uDisk_->readBlock(blockNumber, cacheBuffer_.data)) return false;
    cacheBlockNumber_ = blockNumber;
  }
  cacheDirty_ |= action;
  return true;
}
//------------------------------------------------------------------------------
// cache a zero block for blockNumber
uint8_t UdVolume::cacheZeroBlock(uint32_t blockNumber) {
  if (!cacheFlush()) return false;

  // loop take less flash than memset(cacheBuffer_.data, 0, 512);
  for (uint16_t i = 0; i < 512; i++) {
    cacheBuffer_.data[i] = 0;
  }
  cacheBlockNumber_ = blockNumber;
  cacheSetDirty();
  return true;
}
//------------------------------------------------------------------------------
// return the size in bytes of a cluster chain
uint8_t UdVolume::chainSize(uint32_t cluster, uint32_t* size) const {
  uint32_t s = 0;
  do {
    if (!fatGet(cluster, &cluster)) return false;
    s += 512UL << clusterSizeShift_;
  } while (!isEOC(cluster));
  *size = s;
  return true;
}
//------------------------------------------------------------------------------
// Fetch a FAT entry
uint8_t UdVolume::fatGet(uint32_t cluster, uint32_t* value) const {
  uint8_t valueL, valueM, clu;
  uint16_t offBlo;
  uint16_t triClu = cluster/1024;uint16_t offClu = cluster %1024;
  uint32_t lba = fatStartBlock_;
  if (cluster > (clusterCount_ + 1)) return false;
//  lba +=  cluster / 341;
  

  if (offClu<342) clu = 0;
  else if (offClu<683) clu = 1;
  else clu =2;
  lba += (triClu * 3 + clu);
   

  if (lba != cacheBlockNumber_) {
    if (!cacheRawBlock(lba, CACHE_FOR_READ)) return false;
  }
  
  if((offClu !=341) && (offClu !=682)) {
    switch (clu) {
      case 0:
	    offBlo =  offClu % 341 * 1.5;
        break;
      case 1: 
	    offBlo = (offClu-1) % 341 * 1.5 + 1;
        break;
      case 2:
        offBlo = (offClu-1) % 341 * 1.5 + 0.5;
		break;
	  default: return false;
    }
    if(offClu % 2) {
		    valueL = cacheBuffer_.data[offBlo]>>4;
		    valueM = cacheBuffer_.data[offBlo+1];
		    *value = (valueM << 4) + valueL;
	} else {
			valueL = cacheBuffer_.data[offBlo];
            valueM = cacheBuffer_.data[offBlo+1] &0x0F;
            *value = (valueM << 8) + valueL;
	}	
  } else {
	  switch (offClu) {
		  case 341:
		    valueL = cacheBuffer_.data[511]>>4;
            cacheRawBlock(lba+1, CACHE_FOR_READ);
            valueM = cacheBuffer_.data[0];
            *value = (valueM << 4) + valueL;
			break;
		  case 682:
		    valueL = cacheBuffer_.data[511];
		    cacheRawBlock(lba+1, CACHE_FOR_READ);
		    valueM = cacheBuffer_.data[0] & 0x0F;
		    *value = (valueM << 8) + valueL;
            break;
        default: return false;
	  }
  }
  return true;
}

//------------------------------------------------------------------------------
// Store a FAT entry
uint8_t UdVolume::fatPut(uint32_t cluster, uint32_t value) {
  uint8_t valueL, valueM, clu;
  uint16_t offBlo;uint16_t triClu = cluster/1024;uint16_t offClu = cluster %1024;
  // error if reserved cluster
  if (cluster < 2) return false;

  // error if not in FAT
  if (cluster > (clusterCount_ + 1)) return false;

  // calculate block address for entry
  uint32_t lba = fatStartBlock_;
//  lba +=  cluster / 341;


  if (offClu<342) clu = 0;
  else if (offClu<683) clu = 1;
  else clu =2;
  lba += (triClu * 3 + clu);

  if (lba != cacheBlockNumber_) {
    if (!cacheRawBlock(lba, CACHE_FOR_READ)) return false;
  }

  if((offClu !=341) && (offClu !=682)) {
    switch (clu) {
      case 0: 
	    offBlo =  offClu % 341 * 1.5;
        break;
      case 1: 
	    offBlo = (offClu-1) % 341 * 1.5 + 1;
        break;
      case 2:
        offBlo = (offClu-1) % 341 * 1.5 + 0.5;
		break;
	  default: return false;
    }
    if(offClu % 2) {
        cacheBuffer_.data[offBlo] = ((value & 0x0F) << 4) + (cacheBuffer_.data[offBlo] & 0x0F);
        cacheBuffer_.data[offBlo+1] = (value & 0x0FF0) >> 4;		
	} else {
		cacheBuffer_.data[offBlo] = value & 0xFF;
		cacheBuffer_.data[offBlo+1] = (cacheBuffer_.data[offBlo+1] & 0xF0) + ((value & 0x0F00) >> 8);
	}
    cacheMirrorBlock_ = lba + blocksPerFat_;	
  } else {
	  switch (offClu) {
		  case 341:
		    cacheBuffer_.data[511] = ((value & 0x0F) << 4) + (cacheBuffer_.data[511] & 0x0F);
            cacheSetDirty();
			cacheMirrorBlock_ = lba + blocksPerFat_;
            cacheRawBlock(lba+1, CACHE_FOR_READ);
		    cacheBuffer_.data[0] = (value & 0x0FF0) >> 4;
			break;
		  case 682:
		    cacheBuffer_.data[511] = value & 0xFF;
            cacheSetDirty();
			cacheMirrorBlock_ = lba + blocksPerFat_;
            cacheRawBlock(lba+1, CACHE_FOR_READ);
		    cacheBuffer_.data[0] = (cacheBuffer_.data[0] & 0xF0) + ((value & 0x0F00) >> 8);
            break;
        default: return false;
	  }
	  cacheMirrorBlock_ = lba + blocksPerFat_ + 1;
  }   
  cacheSetDirty();
  
  return true;
}
//------------------------------------------------------------------------------
// free a cluster chain
uint8_t UdVolume::freeChain(uint32_t cluster) {
  // clear free cluster location
  allocSearchStart_ = 2;

  do {
    uint32_t next;
    if (!fatGet(cluster, &next)) return false;

    // free cluster
    if (!fatPut(cluster, 0)) return false;

    cluster = next;
  } while (!isEOC(cluster));

  return true;
}
//------------------------------------------------------------------------------
/**
 * Initialize a FAT volume.
 *
 * \param[in] dev The SD card where the volume is located.
 *
 * \param[in] part The partition to be used.  Legal values for \a part are
 * 1-4 to use the corresponding partition on a device formatted with
 * a MBR, Master Boot Record, or zero if the device is formatted as
 * a super floppy with the FAT boot sector in block zero.
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.  Reasons for
 * failure include not finding a valid partition, not finding a valid
 * FAT file system in the specified partition or an I/O error.
 */


uint8_t UdVolume::init(UdDisk* dev, uint8_t part) {
  uint32_t volumeStartBlock = 0;//卷起始块
  uDisk_ = dev;

  if (!cacheRawBlock(volumeStartBlock, CACHE_FOR_READ)) return false;
  bpb_t* bpb = &cacheBuffer_.fbs.bpb;
  if (bpb->fatCount == 0 ||
    bpb->reservedSectorCount == 0 ||
    bpb->sectorsPerCluster == 0) {
       // not valid FAT volume
      return false;
  }
  
  uint8_t multi = bpb->bytesPerSector / 512;
  fatCount_ = bpb->fatCount;
  blocksPerCluster_ = bpb->sectorsPerCluster * multi;

  // determine shift that is same as multiply by blocksPerCluster_
  clusterSizeShift_ = 0;
  while (blocksPerCluster_ != (1 << clusterSizeShift_)) {
    // error if not power of 2
    if (clusterSizeShift_++ > 7) return false;
  }
  
  blocksPerFat_ = bpb->sectorsPerFat16 * multi;

  fatStartBlock_ = volumeStartBlock + bpb->reservedSectorCount * multi;

  // count for FAT16 zero for FAT32
  rootDirEntryCount_ = bpb->rootDirEntryCount;

  // directory start for FAT16 dataStart for FAT32
  rootDirStart_ = fatStartBlock_ + bpb->fatCount * blocksPerFat_;

  // data start for FAT16 and FAT32
  dataStartBlock_ = rootDirStart_ + ((32 * bpb->rootDirEntryCount + 511)/512);

  // total blocks for FAT16 or FAT32
  uint32_t totalBlocks = bpb->totalSectors16 * multi;

  // total data blocks
  clusterCount_ = totalBlocks - (dataStartBlock_ - volumeStartBlock);

  // divide by cluster size to get cluster count
  clusterCount_ >>= clusterSizeShift_;

  // FAT type is determined by cluster count
  fatType_ = 16;

  return true;
}
};