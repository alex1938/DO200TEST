#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f10x.h"

//向指定地址的flash写入指定长度的内容
//默认从扇区头部开始！
//要写记得先读，本函数直接擦除！
uint8_t FLASH_WriteString(uint32_t Address, uint16_t *Pre_Content,
                          uint16_t Len);

//从指定地址flash读出指定长度的内容
//默认从扇区头开始
uint8_t FLASH_ReadString(uint32_t Address, uint16_t *Pre_Content, uint16_t Len);

#endif
