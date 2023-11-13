/*
 * Copyright (c) 2022
 * Computer Science and Engineering, University of Dhaka
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys_init.h>
#include <cm4.h>
#include <kmain.h>
#include <kstdio.h>
#include <kstring.h>
#include <stdint.h>
#include <usart.h>
#include <seven_segment.h>
#include <sys.h>
#include <test_interrupt.h>


void delay(uint32_t delay)
{
	while (delay--);
}

void kmain(void)
{
	__sys_init();
	__SysTick_init(100000000);

	Exti_config();

	while (1)
	{

		int inp;
		
		kprintf("\n********************************************\n");
		kprintf("Welcome to the Interrupt Testing:\n0: Reboot\n1: Faultmask Configuration\n5: HardFault Event\n6: SysTick Interrupt Event\n7: Systick Interrupt Priority Change \n8: Change BASEPRI Value\n9: Change External Interrupt priority\n");
		kprintf("Press Button for External Interrupt\n");
		kprintf("********************************************\n\n");

		kscanf("%d", &inp);
		if(inp==0)
		{
			reboot();
		}
		else if(inp==1)
		{
			config_faultmask();
		}
		else if (inp == 5)
		{
			enable_hardfault_event();
		}
		else if (inp == 6)
		{
			enableSysTickInterrupt();
		}
		else if (inp == 7)
		{
			change_systick_priority();
		}
		else if (inp == 8)
		{
			config_basepri();
		}
		else if (inp == 9)
		{
			change_exti1_priority();

		}
	}
}
