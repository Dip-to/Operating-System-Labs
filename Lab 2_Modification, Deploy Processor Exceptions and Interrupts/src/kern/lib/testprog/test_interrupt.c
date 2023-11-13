#include <test_interrupt.h>
#include <cm4.h>
#include <kstdio.h>
#include <sys.h>

#define SYS_TICK_INTERVAL_MS 10 // 10 ms interval
#define HSE_VALUE 8000000UL  
volatile int hardfault_event_enabled = 0; // Volatile to prevent optimization
uint32_t clockChoice = 1;
uint32_t clockRateMHz = 180;

void enable_hardfault_event(void)
{
      SCB->SHCSR |= (1 << 16);                //enable mem fault
      volatile int *ptr = (int *)0xFFFFFFFF; // Invalid memory address
      int value = *ptr;                      // This will trigger a HardFault
      (void)value;                           // Just to avoid compiler warnings
}

void enableSysTickInterrupt(void) {
      SYSTICK->CTRL |= (1<<1);
}

void disableSysTickInterrupt(void) {
      SYSTICK->CTRL &= ~(1<<1);

}
void reboot()
{
      kprintf("Rebooting OS............");
      SCB->AIRCR = (0x5FA << 16) | (0x4 << 0); // SCB_AIRCR = VECTKEY | SYSRESETREQ
}

void config_faultmask()
{
      kprintf("Current Faultmask value: %d\n",__get_FAULTMASK());
      kprintf("\n0: Clear Faultmask\n1: Set Faultmask\n");
      int val;
      kscanf("%d\n", &val);
      if(val==1)
      {
            __set_FAULTMASK();
            kprintf("FaultMask Set\n");
      }
      else if(val==0)
      {
            __disable_FAULTMASK();
            kprintf("FaultMask Clear\n");
      }
      kprintf("New Faultmask value: %d\n",__get_FAULTMASK());
}
void config_basepri()
{
      kprintf("Current Basepri Value is: %d\n", get_basepri_value());
      kprintf("Enter a new value for BASEPRI: \n");
      int val;
      kscanf("%d\n", &val);
      __set_BASEPRI(val);
      kprintf("Current Basepri Value is: %d\n", get_basepri_value());
}
void change_systick_priority()
{
      int p;
      kprintf("Systick Current priority: %d\n",NVIC_GetPriority(SysTick_IRQn));
      kprintf("Enter a value for set new Systick priority: ");
      kscanf("%d\n",&p);
      NVIC_SetPriority(SysTick_IRQn,p);
      kprintf("Systick priority changed to: %d\n",NVIC_GetPriority(SysTick_IRQn));
}
void 	change_exti1_priority()
{
      kprintf("Current External Interrupt priority is: %d\n", __NVIC_GetPriority(EXTI1_IRQn));
      kprintf("Enter a new priority for External Interrupt: \n");
      int val;
      kscanf("%d\n", &val);
      __NVIC_SetPriority(EXTI1_IRQn, val); // Set Priority
      kprintf("External Interrupt priority changed to: %d\n", __NVIC_GetPriority(EXTI1_IRQn));
}
void Exti_config()
{
	
      /*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
	1. Enable GPIO Clock
	2. Set the required Pin in the INPUT Mode
	3. Configure the PULL UP/ PULL DOWN According to your requirement

	********************************************************/
      RCC->AHB1ENR |= (1 << 0); // Enable GPIOA clock

	GPIOA->PUPDR |= (1 << 2); // Bits (3:2) = 1:0  --> PA1 is in Pull Up mode
      /*************>>>>>>> STEPS FOLLOWED <<<<<<<<************

	1. Enable the SYSCNFG bit in RCC register
	2. Configure the EXTI configuration Regiter in the SYSCNFG
	3. Enable the EXTI using Interrupt Mask Register (IMR)
	4. Configure the Rising Edge / Falling Edge Trigger
	5. Set the Interrupt Priority
	6. Enable the interrupt

	********************************************************/

	RCC->APB2ENR |= (1 << 14); // Enable SYSCNFG

	SYSCFG->EXTICR[0] &= ~(0xf << 4); // Bits[7:6:5:4] = (0:0:0:0)  -> configure EXTI1 line for PA1

	EXTI->IMR |= (1 << 1); // Bit[1] = 1  --> Disable the Mask on EXTI 1

	EXTI->RTSR &=~(1 << 1); // Disable Rising Edge Trigger for PA1

	EXTI->FTSR |= (1 << 1); // Enable Falling Edge Trigger for PA1

	__NVIC_SetPriority(EXTI1_IRQn, 0); // Set Priority

	__NVIC_EnableIRQn(EXTI1_IRQn); // Enable Interrupt
}