/*
 * timers.c
 *
 *  Created on: 18 Aug 2017
 *      Author: Jakub
 */


#include "main.h"
#include "globals.h"



void TIMERS_init(void)
{
    // initialize the CPU usage module
    int i;
    for(i = 0; i < CPU_TIME_NO; i++)
    {
        cpu_timeHandle[i] = CPU_TIME_init(&cpu_time[i],sizeof(cpu_time[i]));
        CPU_TIME_setParams(cpu_timeHandle[i], PWM_getPeriod(halHandle->pwmHandle[0]));
    }
}


void TIMERS_startCpuTimeMeasurement(int timer_no)
{
    // read the timer 1 value and update the CPU usage module
    uint32_t timerCnt = HAL_readTimerCnt(halHandle,2);
    CPU_TIME_updateCnts(cpu_timeHandle[timer_no], timerCnt);
}


void TIMERS_stopCpuTimeMeasurement(int timer_no)
{
    // read the timer 1 value and update the CPU usage module
    int32_t timerCnt = HAL_readTimerCnt(halHandle,2);
    CPU_TIME_run(cpu_timeHandle[timer_no], timerCnt);
    CPU_TIME_calcRealTime(cpu_timeHandle[timer_no], USER_SYSTEM_FREQ_MHz);
}
