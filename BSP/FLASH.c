#include "flash.h"


uint16_t read_flash(uint32_t address){
  uint16_t flash_data = *(__IO uint32_t *)address;
  return flash_data;
}

TestStatus erase_write_flash(uint32_t address,uint16_t data){
  __IO TestStatus MemoryProgramStatus = PASSED;
      /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  FLASH_Unlock();
    /* Erase the user Flash area
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

  /* Clear pending flags (if any) */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
  if (FLASH_ErasePage(address) != FLASH_COMPLETE){
    MemoryProgramStatus = FAILED;
    return MemoryProgramStatus;
  }
  if (FLASH_ProgramHalfWord(address, data) != FLASH_COMPLETE)
  {     
      MemoryProgramStatus = FAILED;
      return MemoryProgramStatus;
  }
    FLASH_Lock();
    return MemoryProgramStatus; 
}