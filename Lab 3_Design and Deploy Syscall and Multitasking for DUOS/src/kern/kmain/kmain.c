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

#include <kmain.h>
#include <schedule.h>
#include <fcntl.h>
#include <errno.h>
#include<syscall.h>
#include <syscall_def.h>

#define STOP 10000000
//#define STOP 100


TCB_TypeDef task[22], _sleep;
int global_count = 0;

void task_sleep(void)
{
    int reboot_ = 0;
    kprintf("\n\r\n\rAll tasks has been completed.\n\r\n\r\tEnter 1 to reboot the system. \n\r\tEnter any other number to enter sleeping mode.\n\r");
    kscanf("%d", &reboot_);
    if (reboot_ == 1)
        reboot();

    kprintf("Entering sleep mode...\n\r");
    while (1)
        ;
}
int x=75;
int count =0;
// void task1(void)
// {
   

//     uint32_t value;
//     uint32_t inc_count = 0;
//     while (1)
//     {
//         uint16_t task_id = getpid() - 1000; /* It is an SVC call*/

//         // critical region
//         value = count;
//         value++;

//         // we check if some other tasks increase the count
//         if (value != count + 1)
//         {
//             kprintf("Task %d running", task_id);
//             kprintf("Error %d != %d\n\r", value, count + 1); /* It is an SVC call*/
//         }
//         else
//         {
//             // critical region
//             // printf("Task %d running No Error %d == %d\n\r", task_id, value, count + 1); /* It is an SVC call*/
//             count = value;
//             inc_count++;
//         }
//         if (count >= STOP)
//         {
//             /* display how many increments it has successfully done!! */
//             kprintf("Total increment done by task %d is: %d\n\r", task_id, inc_count);
//             kprintf("Total increment done by task is: %d\n\r", inc_count);
//             /* above is an SVC call */
//             int fd = fopen("S_DISPLAY", O_WRDONLY);

//             fprintf(fd, "%d\n", x);
//             x = (x + 1) % 8;
//             fclose(fd);

//             break;
//         }
//     }
//     exit();
// }

void task2(void)
{
    uint32_t value;
    uint32_t inc_count=0;
    // volatile uint32_t count=0;
    uint16_t task_id = getpid() - 1000; /* It is an SVC call*/

    while(1){

        value=count;
        value++;

        if(value != count+1){ //we check is someother task(s) increase the count
            kprintf("Task %d running", task_id);
            kprintf(" Error %d != %d\n",value,count); /* It is an SVC call*/
        }
        else{
            count=value;
            inc_count++;
            //kprintf("Task %d running No Error %d == %d\n\r", task_id, value, count); /* It is an SVC call*/

        }
        if(count >= STOP){
           // uint16_t task_id = getpid(); /* It is an SVC call*/
            /* display how many increments it has successfully done!! */
            kprintf("\nTotal increment done by task %d is: %d\n",task_id,inc_count);
            /* above is an SVC call */
            int fd=fopen("S_DISPLAY",O_WRONLY);
           // printDirectory();
            kprintf("\ntask %d fd: %d\n",task_id,fd);
            /* int x declare as local */
            fprintf(fd, "task %d x value: %d\n",task_id, x);
            x=(x+1)%8;
            fclose(fd);
            break;
        }
    }
    //syscall(SYS__exit);
   k_exit();

  
}

void unprivileged_mode(void)
{
    // read operation from special register CPSR and SPSR
    __asm volatile("MRS R0, CONTROL");
    __asm volatile("ORRS R0, R0, #1");
    // write operation to special register CPSR and SPSR
    __asm volatile("MSR CONTROL, R0");
}

void __set_interrupt_priorities(void)
{
    __NVIC_SetPriority(SVCall_IRQn, 1);
    __NVIC_SetPriority(SysTick_IRQn, 0x2);
    // lowest priority given to PendSV
    __NVIC_SetPriority(PendSV_IRQn, 0xFF);
}

void kmain(void)
{
    __sys_init();
    __set_interrupt_priorities();
    kprintf("\n--------------------open file descriptor---------------------\n");
    fopen("STDIN FILENO", O_RDONLY);
    fopen("STDOUT FILENO", O_RDONLY);
    fopen("STDERR FILENO", O_RDONLY);
    kprintf("\n----------------------Task Starting---------------------------------------\n");

    int tc = 20;

    for (int i = 0; i < tc; i++)
    {
        task_create(task + i, task2, (uint32_t *)(TASK_STACK_START - (i * TASK_STACK_SIZE)));
    
    }

    task_create(&_sleep, task_sleep, (uint32_t *)(TASK_STACK_START - (tc * TASK_STACK_SIZE)));

    // initializing queue
    initialize_queue();

    for (int i = 0; i < tc; i++)
        add_to_ready_queue(task + i);

    
    set_sleeping_task(&_sleep);

    // going to user mode
    unprivileged_mode();
    // set pendsv before starting task
    set_task_pending(1);

    task_start();
    kprintf("\n----------------------All Tasks Done!!!---------------------------------------\n");

    while (1);
}
