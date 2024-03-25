#include <kmain.h>
#include <schedule.h>
#include <sem.h>

#define STOP 10000000

int semaphore = 0;

TCB_TypeDef task[22], _sleep;
int count = 0;
//int x = 10;

finished[task_count] = {0};

void print_report()
{
    int avg_ta=0,avg_wait=0,avg_st=0,avg_res=0,avg_exc=0,avg_comp=0;
    kprintf("-----------------------------------------------------------------------------------------------------\n");

    kprintf("|id|   Starting   |    Response   |  Completion   |    Execution   |     Waiting     |   Turnaround |\n");
    kprintf("-----------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < task_count; i++)
    {
        int ta=task[i].completion_time-task[i].starting_time;
        int wa=ta-task[i].execution_time;
        avg_ta+=ta;
        avg_wait+=wa;
        avg_st+=task[i].starting_time;
        avg_res+=task[i].response_time;
        avg_exc+=task[i].execution_time;
        avg_comp+=task[i].completion_time;
        printf("| %d|     %dms     |      %dms     |     %dms    |     %dms    |     %dms    |    %dms   |\n",i, task[i].starting_time, task[i].response_time, task[i].completion_time, task[i].execution_time,wa,ta);
    }
    kprintf("-----------------------------------------------------------------------------------------------------\n");
    kprintf("|  avg Starting  |avg Response | avg Completion| avg Execution|    avg  Waiting |  avg Turnaround |\n");
    kprintf("----------------------------------------------------------------------------------------------------------\n");

    printf("|       %dms     |      %dms   |     %dms     |     %dms     |     %dms       |     %dms     |\n", avg_st/task_count,  avg_res/task_count, avg_comp/task_count,  avg_exc/task_count,avg_wait,avg_ta);
    kprintf("-----------------------------------------------------------------------------------------------------------\n");

}   

void task_sleep(void)
{
    print_report();
    int reboot_ = 0;
    printf("\nAll tasks has been completed.\nENTER 1 TO REBOOT DUOS. \nENTER 0 TO SHUTDOWN\n\r");
    uscanf("%d", &reboot_);
    if (reboot_ == 1)
        reboot();

    printf("SHUTTING DOWN DUOS...\n\r");
    while (1)
        ;
}

void Task(void)
{
    TCB_TypeDef task = getpid(); /* It is an SVC call*/
    uint32_t value;
    uint32_t inc_count = 0;
    while (1)
    {
        sem_inc(&semaphore); //semaphore increase (critical region start)
        value = count;
        //context switching
        value++;

        if (value != count + 1)
        {
            printf("Task %d ", task.task_id-1000);
            printf("Error %d != %d\n\r", value, count + 1); /* It is an SVC call*/
        }
        else
        {
            count = value;
            inc_count++;
        }
        sem_dec(&semaphore); //semaphore decrease
        if (count >= STOP)
        {
            printf("Total increment done by task %d is: %d\n\r", task.task_id-1000, inc_count);
            break;
        }
    }
    int index = task.task_id - 1000;
    finished[index] = 1;
    exit();
}

void unprivileged_mode(void)
{
    __asm volatile("MRS R0, CONTROL");
    __asm volatile("ORRS R0, R0, #1");
    __asm volatile("MSR CONTROL, R0");
}
int con;
void fcfs() //fcfs
{
    con=2;
    for (int i = 0; i < task_count; i++)
    {
        task_create(task + i, Task, (uint32_t *)(TASK_STACK_START - (i * TASK_STACK_SIZE)));
    }

    task_create(&_sleep, task_sleep, (uint32_t *)(TASK_STACK_START - (task_count * TASK_STACK_SIZE)));

    initialize_queue();

    for (int i = 0; i < task_count; i++)
        add_to_ready_queue(task + i);
    set_sleeping_task(&_sleep);

    set_task_pending(1);

    task_start();
}
void rr() //round robin
{
    con=1;
    for (int i = 0; i < task_count; i++)
    {
        task_create(task + i, Task, (uint32_t *)(TASK_STACK_START - (i * TASK_STACK_SIZE)));
    }

    task_create(&_sleep, task_sleep, (uint32_t *)(TASK_STACK_START - (task_count * TASK_STACK_SIZE)));

    initialize_queue();

    for (int i = 0; i < task_count; i++)
        add_to_ready_queue(task + i);
    set_sleeping_task(&_sleep);

    set_task_pending(1);

    task_start();
}
void kmain(void)
{
    __sys_init();

    __NVIC_SetPriority(SVCall_IRQn, 1);
    __NVIC_SetPriority(SysTick_IRQn, 0x2);
    __NVIC_SetPriority(PendSV_IRQn, 0xFF);
   
    // kprintf("\nRound Robin scheduling:-------------------\n") ;
    // rr(); //round robin
    kprintf("\nFCFS scheduling:-------------------\n") ;
    fcfs(); //fcfs


    while (1)
        ;
}
