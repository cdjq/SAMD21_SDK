# DFRobot_EasyFlash


## 产品链接（www.dfrobot.com）
    SKU：产品名称
   
## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary

### 这个库实现了spiflash上的数据库功能和log功能
这个库应用于FireBeetle M0主板，基于ATmelSAMD21处理器和W25Q128外部spiflash。
可以写入键值对，再通过键获得值。

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++
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

  /**
   * @brief 在数据库中设置键值对，注意key值不能超过32字节，value不能超过4010字节
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
   * @param savedValueLen 指向一个size_t型数据，函数运行时会将该指针指向的值赋为value实际的字节数
   * @param value 指向字符或字符串value的const char*型指针
   * @param value String对象
   * @return 返回读取到的value的字节数
   * @return 返回String对象
   */	
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
```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
FireBeetle         |      √       |              |             | 


## History

- data 2019-12-24
- version V1.0


## Credits

Written by Jack(yue.li@dfrobot.com), 2019. (Welcome to our [website](https://www.dfrobot.com/))





