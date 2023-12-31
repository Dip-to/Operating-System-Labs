#ifndef __SYS_H
#define __SYS_H
#ifdef __cplusplus
extern "C"
{
#endif
#define NVIC_SetPriorityGrouping __NVIC_SetPriorityGrouping
#define NVIC_GetPriorityGrouping __NVIC_GetPriorityGrouping
#define NVIC_EnableIRQ __NVIC_EnableIRQn
#define NVIC_GetEnableIRQ __NVIC_GetEnableIRQ
#define NVIC_DisableIRQ __NVIC_DisableIRQ
#define NVIC_GetPendingIRQ __NVIC_GetPendingIRQ
#define NVIC_SetPendingIRQ __NVIC_SetPendingIRQ
#define NVIC_ClearPendingIRQ __NVIC_ClearPendingIRQ
#define NVIC_GetActive __NVIC_GetActive
#define NVIC_SetPriority __NVIC_SetPriority
#define NVIC_GetPriority __NVIC_GetPriority
#define NVIC_SystemReset __NVIC_SystemReset
#include <cm4.h>
#include <gpio.h>




/******************************************* NVIC *******************************************/

/************************* Data Structures *************************/

// programming-manual p194

// NVIC address to access: 0xE000E100 (Starting Address)




      /* Functions of NVIC */
      void __NVIC_SetPriority(IRQn_TypeDef IRQn, uint32_t priority);
      uint32_t __NVIC_GetPriority(IRQn_TypeDef IRQn);
      void __NVIC_EnableIRQn(IRQn_TypeDef IRQn);
      void __NVIC_DisableIRQn(IRQn_TypeDef IRQn);
      void __disable_irq();
      void __set_BASEPRI(uint32_t value);
      void __enable_irq();
      void __unset_BASEPRI();
      void __set_PRIMASK(void);
      uint32_t __get_PRIMASK(void);
      void __enable_fault_irq(void);
      void __set_FAULTMASK(void);
      void __disable_FAULTMASK(void);
      uint32_t __get_FAULTMASK(void);
      void __clear_pending_IRQn(IRQn_TypeDef IRQn);
      uint32_t __get_pending_IRQn(IRQn_TypeDef IRQn);
      uint32_t __NVIC_GetActive(IRQn_TypeDef IRQn);
      void EXTI_Init(GPIO_TypeDef *port, uint16_t pin);
      uint32_t get_basepri_value(void);


#ifdef __cplusplus
}
#endif
#endif
