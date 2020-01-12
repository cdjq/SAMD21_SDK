/*!
 * @file DFRobot_EasyFlash.h
 * @brief 定义DFRobot_EasyFlash 类所需函数
 * @n 它定义了移植 easyflash 所需要用到的函数
 * @n 这些函数需要在easyflash.h中声明，供别的文件使用
 * @n 调试用的打印函数
 * @n 初始化SPI和SPI传输函数
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [LiYue](yue.li@dfrobot.com)
 * @version  V1.0
 * @date  2019-12-16
 * @url https://github.com/cdjq/DFRobot_EasyFlash
 */
 
#include <easyflash.h>
#include <variant.h>
#include <SPI.h>

void test(uint8_t a)
{
	Serial.print(a);
}

void test1(const char str[])
{
	Serial.print(str);
}

void test2(uint32_t a)
{
	Serial.print(a);
}

uint8_t transfer(uint8_t a)
{
	return SPI.transfer(a);
}

void spiBegin()
{
	return SPI.begin();
}