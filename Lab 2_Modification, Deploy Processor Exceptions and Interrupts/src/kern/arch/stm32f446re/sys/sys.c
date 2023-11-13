#include <sys.h>
#include <seven_segment.h>
#include <kstdio.h>
/* Functions of NVIC */
void __NVIC_SetPriority(IRQn_TypeDef IRQn, uint32_t priority)
{
      if (IRQn >= 0)
      {
            NVIC->IP[IRQn] = (uint8_t)((priority << 4));
      }
      else 
      {
            SCB->SHPR[(IRQn & 15)-4] = (uint8_t)(priority << 4);
      }
}

uint32_t __NVIC_GetPriority(IRQn_TypeDef IRQn)
{
      if (IRQn >= 0)
      {
            return (NVIC->IP[IRQn] >> 4);
      }
      else 
      {
            return (SCB->SHPR[(IRQn & 15)-4] >> 4);
      }

}

void __NVIC_EnableIRQn(IRQn_TypeDef IRQn)
{
      if (IRQn >= 0)
      {
            NVIC->ISER[IRQn / 32] |= (1 << (IRQn % 32));
      }
      else 
      {
            uint32_t exception_number = (-IRQn) & 0x0F; // Get the lower 4 bits
            SCB->SHCSR |= (1 << (exception_number + 16)); // Enable system exception
      }
}

void __NVIC_DisableIRQn(IRQn_TypeDef IRQn)
{
      if (IRQn >= 0)
      {
            NVIC->ICER[IRQn / 32] |= (1 << (IRQn % 32));
      }
      else 
      {
            uint32_t exception_number = (-IRQn) & 0x0F; // Get the lower 4 bits
            SCB->SHCSR &= ~(1 << (exception_number + 16)); // Disable system exception
      }
}


void __set_BASEPRI(uint32_t value) {
    /*
    programming manual - p24
    BASEPRI[7:4] Priority mask bits
    */
    value = (value << 4);
    asm volatile("MSR BASEPRI, %0" : : "r" (value) : "memory");
    kprintf("BASEPRI disabled interrupt with priority lower than %d\n",(value>>4));

}

uint32_t get_basepri_value(void) {
   
    uint32_t value;
    asm("mrs %0, basepri" : "=r"(value));
    return (value >> 4);
}
void __unset_BASEPRI(uint32_t value)
{
      asm("mov r0,#0");
      asm("msr basepri,r0;");
      kprintf("BASEPRI reset complete.");
}


void __disable_irq()
{
      asm("mov r0,#1");
      asm("msr primask,r0;");
}

void __enable_irq()
{
      asm("mov r0,#0");
      asm("msr primask,r0;");
}

void __set_PRIMASK()
{
      asm("mov r0, #1");
      asm("msr primask, r0");
}

uint32_t get_PRIMASK(void)
{
      uint32_t value;
      asm("mrs r0,primask");
      asm("mov %0,r0"
          : "=r"(value));
      return value;
}

void __enable_fault_irq(void)
{
      asm("mov r0, #0");
      asm("msr primask, r0");
}

void __set_FAULTMASK()
{
      asm("mov r0, #1");
      asm("msr faultmask, r0");
}

void __disable_FAULTMASK(void)
{
      asm("mov r0, #0");
      asm("msr faultmask, r0");
}

uint32_t __get_FAULTMASK(void)
{
      uint32_t value;
      asm("mrs r0,faultmask");
      asm("mov %0,r0"
          : "=r"(value));
      return value;
}

void __clear_pending_IRQn(IRQn_TypeDef IRQn)
{
      if (IRQn >= 0)
      {
            NVIC->ICPR[IRQn / 32] |= (1 << (IRQn % 32));
      }
}

uint32_t __get_pending_IRQn(IRQn_TypeDef IRQn)
{
      uint32_t pendingState;
      if (IRQn >= 0)
      {
            int regNumber = IRQn / 32;
            int offset = IRQn % 32;
            pendingState = NVIC->ICPR[regNumber] & (1 << regNumber);
            pendingState = pendingState >> 5;
            return pendingState;
      }
}

uint32_t __NVIC_GetActive(IRQn_TypeDef IRQn)
{
      uint32_t pendingState;
      if (IRQn >= 0)
      {
            int regNumber = IRQn / 32;
            int offset = IRQn % 32;
            pendingState = NVIC->IABR[regNumber] & (1 << regNumber);
            pendingState = pendingState >> 5;
            return pendingState;
      }
}