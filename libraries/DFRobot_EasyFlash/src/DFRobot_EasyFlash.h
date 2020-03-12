/*!
 * @file DFRobot_EasyFlash.h
 * @brief 定义DFRobot_EasyFlash 类的基础结构
 * @n 它包含了这个类的所有函数的声明和内联函数的定义
 * @n 初始化
 * @n 重新加载分配内存空间
 * @n 读
 * @n 写
 * @n 删除
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [LiYue](yue.li@dfrobot.com)
 * @version  V1.0
 * @date  2019-12-16
 * @url https://github.com/cdjq/DFRobot_EasyFlash
 */
 
#ifndef DF_EASYFLASH_H
#define DF_EASYFLASH_H

#include "easyflash.h"
#include <WString.h>
class DFRobot_EasyFlash
{
public:
 
/* @brief EfErrCode是easyflash库自定义的枚举类型，用来反应程序运行状态，定义在ef_def.h：
 typedef enum {
    EF_NO_ERR,
    EF_ERASE_ERR,
    EF_READ_ERR,
    EF_WRITE_ERR,
    EF_ENV_NAME_ERR,
    EF_ENV_NAME_EXIST,
    EF_ENV_FULL,
    EF_ENV_INIT_FAILED,
} EfErrCode;
 */
  /**
   * @brief 初始化函数
   * @n 调用ef_port_init()完成引脚和SPI配置
   * @n 调用ef_env_init()检查ef_cfg.h中宏是否配置正确
   *                     完成一些全局变量初始化
   *                     初始化sector和env的缓存数组用来加快查找速度
   *                     调用内部ef_load_env()函数完成内存再分配的功能
   *                     检查固件版本，如果有更新，自动更新写入默认环境变量
   * @n 调用ef_log_init()检查宏，找到log的起始和结束地址，并判断log内存有没有错误，有错就格式化log
   * @return 返回程序运行状态
   */
    EfErrCode begin(void);

  /**
   * @brief 在程序初始化之后，调用该函数会重新分配数据库内存
   * @n 该函数功能等价于ef_load_env()
   * @n 检查所有sector的头，如果所有头都错误，则格式化所有sector并写入默认env
   * @n 若有dirty状态为gc的sector，进行gc_collect:判断内存使用是否超过threshold，若超过，找出所有dirty状态为ture和gc的sector，
        移动sector中所有状态为write和pre_delete的env，并格式化sector
   * @n 找出所有状态为pre_delete和pre_write的env，对他们进行delete和write
   * @n 若产生gc请求，再次进行gc_collect
   * @return 返回程序运行状态
   */
    EfErrCode OptimizeMemory(void);
  /**
   * @brief 格式化env部分所有sector并写入默认env
   * @return 程序运行状态
   */
    EfErrCode format(void);
	
	#ifdef EF_USING_ENV
    /* supported on ef_env.c */

  /**
   * @brief 在数据库中设置键值对，注意key值不能超过32字节，字符串类型value不能超过128字节，非字符串类value不能超过4010字节
   * @param key 指向字符或字符串key的指针
   * @param key 含有key值的String对象
   * @param valueBuf 指向要写入数据的指针
   * @param bufLen 要写入的value的字节数
   * @param value 指向字符或字符串value的指针
   * @param value 含有value值的String对象
   * @return 返回运行状态
   */
	EfErrCode setValue(const char *key, const void *valueBuf, size_t bufLen);
    EfErrCode setValue(const char *key, const char *value);
    EfErrCode setValue(const char *key, String &value);
	EfErrCode setValue(String &key, String &value);
	EfErrCode setValue(String &key, const char *value);

  /**
   * @brief 根据key获得value值
   * @param key 指向字符或字符串key的指针
   * @param key String对象
   * @param valueBuf 指向存放数据的valueBuf的const void*型指针
   * @param bufLen valueBuf的字节数
   * @param savedValueLen 指向一个size_t型数据，函数运行时会将该指针指向的值变为value的字节数
   * @param value 指向字符或字符串value的const char*型指针
   * @param value String对象
   * @return 返回实际获得的字节数（不超过bufLen）
   * @return 返回String对象
   */
    size_t getValue(const char *key, void *valueBuf, size_t bufLen, size_t &savedValueLen);
	size_t getValue(String &key, void *valueBuf, size_t bufLen, size_t &savedValueLen);
	size_t getValue(const char *key, void *valueBuf, size_t bufLen, size_t *savedValueLen);
    size_t getValue(String &key, void *valueBuf, size_t bufLen, size_t *savedValueLen);
    size_t getValue(const char *key, void *valueBuf, size_t bufLen);
    size_t getValue(String &key, void *valueBuf, size_t bufLen);
    String getValue(const char *key);
    String getValue(String &key);

  /**
   * @brief 根据key删除键值对，实际是改变flash中env的状态为delete，并改变ram中用来加快查找速度env cache数组的状态
   * @param key 指向字符或字符串key的指针
   * @param key String对象
   * @return 返回运行状态
   */
    EfErrCode delValue(const char *key);
	EfErrCode delValue(String &key);
    #endif

    #ifdef EF_USING_LOG
    /* ef_log.c */
	
  /**
   * @brief 根据index和size，读取spiflash指定内容
   * @param index 起始索引，读取起始字节相对log首字节的偏移值，必须小于当前log文件的总长度
   * @param log 指向用来存储log的内存地址的指针
   * @param size 读取的字节数，如果index + size >当前log文件总长度，则只读取（当前文件总长 - index）字节数据 
   * @return 返回运行状态
   */
    EfErrCode logRead(size_t index, void *log, size_t size);
    EfErrCode logRead(size_t index, String &log, size_t size);

  /**
   * @brief 写log
   * @param log 指向用来存储log的内存地址的指针
   * @param size 写的字节数，必须是4的整数倍，如果不是，最后size%4个字节会丢失
   * @return 返回运行状态
   */	
    EfErrCode logWrite(const void *log, size_t size);
	EfErrCode logWrite(String &log, size_t size);
	
  /**
   * @brief 格式化log
   * @return 返回运行状态
   */    
	EfErrCode logClean(void);

  /**
   * @brief 获取使用的字节数
   * @return 返回log区使用的字节数
   */	
    size_t logSize(void);
    #endif
	/* print */
/*	void efLogDebug(const char *file, const long line, const char *format, ...);
	void efLogInfo(const char *format, ...);
	void efPrint(const char *format, ...);	*/

};

inline EfErrCode DFRobot_EasyFlash::begin(void)
{
	return easyflash_init();
}

inline EfErrCode DFRobot_EasyFlash::OptimizeMemory(void)
{
	return ef_load_env();
}

inline EfErrCode DFRobot_EasyFlash::format(void)
{
    return ef_env_set_default();
}

inline EfErrCode DFRobot_EasyFlash::setValue(const char *key, const void *valueBuf, size_t bufLen)
{
	return ef_set_env_blob(key, valueBuf, bufLen);
}

inline EfErrCode DFRobot_EasyFlash::setValue(const char *key, const char *value)
{
    return ef_set_env_blob(key, value, strlen(value));
}

inline EfErrCode DFRobot_EasyFlash::setValue(const char *key, String &value)
{
    return ef_set_env_blob(key, value.c_str(), value.length());
}

inline EfErrCode DFRobot_EasyFlash::setValue(String &key, String &value)
{
    return ef_set_env_blob(key.c_str(), value.c_str(), value.length());
}

inline EfErrCode DFRobot_EasyFlash::setValue(String &key, const char *value)
{
    return ef_set_env_blob(key.c_str(), value, strlen(value));
}


inline size_t DFRobot_EasyFlash::getValue(const char *key, void *valueBuf, size_t bufLen, size_t &savedValueLen)
{   
	return ef_get_env_blob(key, valueBuf, bufLen, &savedValueLen);
}

inline size_t DFRobot_EasyFlash::getValue(String &key, void *valueBuf, size_t bufLen, size_t &savedValueLen)
{   
	return ef_get_env_blob(key.c_str(), valueBuf, bufLen, &savedValueLen);
}

inline size_t DFRobot_EasyFlash::getValue(const char *key, void *valueBuf, size_t bufLen, size_t *savedValueLen)
{   
	return ef_get_env_blob(key, valueBuf, bufLen, savedValueLen);
}

inline size_t DFRobot_EasyFlash::getValue(String &key, void *valueBuf, size_t bufLen, size_t *savedValueLen)
{   
	return ef_get_env_blob(key.c_str(), valueBuf, bufLen, savedValueLen);
}

inline size_t DFRobot_EasyFlash::getValue(const char *key, void *valueBuf, size_t bufLen)
{   
	return ef_get_env_blob(key, valueBuf, bufLen, NULL);
}

inline size_t DFRobot_EasyFlash::getValue(String &key, void *valueBuf, size_t bufLen)
{   
	return ef_get_env_blob(key.c_str(), valueBuf, bufLen, NULL);
}


inline EfErrCode DFRobot_EasyFlash::delValue(const char *key)
{
	return  ef_del_env(key);
}

inline EfErrCode DFRobot_EasyFlash::delValue(String &key)
{
	return  ef_del_env(key.c_str());
}


inline EfErrCode DFRobot_EasyFlash::logRead(size_t index, void *log, size_t size)
{
	return ef_log_read(index, (uint32_t *)log, ((size+3)/4)*4);
}

inline EfErrCode DFRobot_EasyFlash::logRead(size_t index, String &log, size_t size)
{
	return ef_log_read(index, (uint32_t *)log.c_str()/*(reinterpret_cast<uint32_t *>)(const_cast<char *>(log.c_str()))*/, ((size+3)/4)*4);
}


inline EfErrCode DFRobot_EasyFlash::logWrite(const void *log, size_t size)
{
	return ef_log_write((const uint32_t *)log, ((size-3)/4)*4);
}

inline EfErrCode DFRobot_EasyFlash::logWrite(String &log, size_t size)
{
	return ef_log_write((const uint32_t *)log.c_str(), ((size-3)/4)*4);
}

inline 	EfErrCode DFRobot_EasyFlash::logClean(void)
{
	return ef_log_clean();
}

inline size_t DFRobot_EasyFlash::logSize(void)
{
	return ef_log_get_used_size();
}
#endif /* DF_EASYFLASH_H */