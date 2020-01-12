/*!
 * @file DFRobot_EasyFlash.h
 * @brief 定义DFRobot_EasyFlash 类的数据库部分函数
 * @n 它包含了这个类的数据库部分非内联函数的定义
 * @n 得到Value值，并返回String类对象
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [LiYue](yue.li@dfrobot.com)
 * @version  V1.0
 * @date  2019-12-16
 * @url https://github.com/cdjq/DFRobot_EasyFlash
 */
 
#include "DFRobot_EasyFlash.h"


static bool ef_is_str(uint8_t *value, size_t len)
{
#define __is_print(ch)       ((unsigned int)((ch) - ' ') < 127u - ' ')
    size_t i;

    for (i = 0; i < len; i++) {
        if (!__is_print(value[i])) {
            return false;
        }
    }	
    return true;
}

String DFRobot_EasyFlash::getValue(const char *key)
{   
    static char value[128/*EF_STR_ENV_VALUE_MAX_SIZE*/ + 1];
    size_t get_size;

    if ((get_size = ef_get_env_blob(key, value, 128/*EF_STR_ENV_VALUE_MAX_SIZE*/, NULL)) > 0) {
        /* the return value must be string */
        if (ef_is_str((uint8_t *)value, get_size)) {
            value[get_size] = '\0';
            return String(value);
        } else {
            EF_INFO("Warning: The ENV value isn't string. Could not be returned\n");
            return String(NULL);;
        }
    }

    return String(NULL);;
}

String DFRobot_EasyFlash::getValue(String &key)
{   
    static char value[128/*EF_STR_ENV_VALUE_MAX_SIZE*/ + 1];
    size_t get_size;

    if ((get_size = ef_get_env_blob(key.c_str(), value,128/*EF_STR_ENV_VALUE_MAX_SIZE*/, NULL)) > 0) {
        /* the return value must be string */
        if (ef_is_str((uint8_t *)value, get_size)) {
            value[get_size] = '\0';
            return String(value);
        } else {
            EF_INFO("Warning: The ENV value isn't string. Could not be returned\n");
            return String(NULL);
        }
    }

    return String(NULL);;
}


