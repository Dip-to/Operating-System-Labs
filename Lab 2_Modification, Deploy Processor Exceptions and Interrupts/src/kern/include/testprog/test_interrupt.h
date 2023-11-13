#ifndef TEST_INTERRUPT_H
#define TEST_INTERRUPT_H

void enable_hardfault_event(void);
void enableSysTickInterrupt(void);
void disableSysTickInterrupt(void);
void reboot(void);
void change_systick_priority(void);
void Exti_config(void);
void config_faultmask(void);
void config_basepri(void);
void change_exti1_priority(void);

#endif // TEST_INTERRUPT_H