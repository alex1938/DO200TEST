// flash读写文件

#include "flash.h"

//向指定地址的flash写入指定长度的内容
//默认从扇区头部开始！
//要写记得先读，本函数直接擦除！
uint8_t FLASH_WriteString(uint32_t Address, uint16_t *Pre_Content,
                          uint16_t Len) {
  uint16_t count = 0;
  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
  FLASH_ErasePage(Address);
  for (count = 0; count < Len; count++) {
    FLASH_ProgramHalfWord(Address + count * 2, Pre_Content[count]);
  }
  FLASH_Lock();
  return 1;
}

//从指定地址flash读出指定长度的内容
//默认从扇区头开始
uint8_t FLASH_ReadString(uint32_t Address, uint16_t *Pre_Content,
                         uint16_t Len) {
  uint16_t count;
  for (count = 0; count < Len; count++) {
    Pre_Content[count] = *(vu16 *)(Address + count * 2);
  }
  return 1;
}
