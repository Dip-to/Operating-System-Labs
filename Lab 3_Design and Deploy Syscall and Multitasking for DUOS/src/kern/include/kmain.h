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

#ifndef __KMAIN_H
#define __KMAIN_H

#include <sys_init.h>
#include <cm4.h>
#include <kstdio.h>
#include <types.h>
#include <unistd.h>
#include <schedule.h>

#define O_RDONLY 1
#define O_WRDONLY 2
#define O_APPEND 3



#define MAX_TASKS 30

/* some stack memory calculations */
#define SIZE_TASK_STACK 1024U
#define SIZE_SCHED_STACK 1024U

#define SRAM_START 0x20000000U
#define SIZE_SRAM ((128) * (1024))
#define SRAM_END ((SRAM_START) + (SIZE_SRAM))

#define KERNEL_STACK_START SRAM_END      // main stack
#define KERNEL_STACK_SIZE ((4) * (1024)) // 4KB
#define KERNEL_STACK_END ((KERNEL_STACK_START) - (KERNEL_STACK_SIZE))

#define TASK_STACK_START KERNEL_STACK_END
#define TASK_STACK_SIZE ((1) * (1024)) // 1KB

#define DEVICE_STACK_START TASK_STACK_START - ((MAX_TASKS)*TASK_STACK_SIZE)
#define DEVICE_STACK_SIZE ((1) * (1024)) // 1KB

#define TICK_HZ 1000U

#define HSI_CLOCK 16000000U
#define SYSTICK_TIM_CLK HSI_CLOCK

#define DUMMY_XPSR 0x01000000U

#define TASK_READY_STATE 0x00
#define TASK_BLOCKED_STATE 0XFF

#define INTERRUPT_DISABLE()             \
    do                                  \
    {                                   \
        __asm volatile("MOV R0,#0x1");  \
        asm volatile("MSR PRIMASK,R0"); \
    } while (0)

#define INTERRUPT_ENABLE()              \
    do                                  \
    {                                   \
        __asm volatile("MOV R0,#0x0");  \
        asm volatile("MSR PRIMASK,R0"); \
    } while (0)

#endif /* KMAIN_H */