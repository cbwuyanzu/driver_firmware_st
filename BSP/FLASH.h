#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f0xx.h"


#define FLASH_PAGE_SIZE         ((uint32_t)0x00000400)   /* FLASH Page Size */
#define FLASH_USER_ADDR_I_PROG  ((uint32_t)0x08003000)   /* Start @ of user Flash area */
#define FLASH_USER_ADDR_I_MAX   ((uint32_t)0x08003400)   /* Start @ of user Flash area */
#define FLASH_USER_ADDR_I_MIN   ((uint32_t)0x08003800)   /* Start @ of user Flash area */
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

TestStatus erase_write_flash(uint32_t address,uint16_t data);
uint16_t read_flash(uint32_t address);
#endif