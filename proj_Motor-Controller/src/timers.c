/****************************************************************************//**
 * @file timers.c
 * @brief The file contains code for functions using the timers.
 *
 * Created by Jakub Klein on 22 Aug 2017 for &reg; Radinn
 ********************************************************************************/



#include "main.h"
#include "globals.h"


/**
 * Initialize the CPU_USAGE module with CPU_TIME_NO time measurements
 */
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

/**
 * Place it at the line where the time measurement for specified timer (from 0 to CPU_TIME_NO) should begin
 * @param timer_no
 */
void TIMERS_startCpuTimeMeasurement(int timer_no)
{
    // read the timer 1 value and update the CPU usage module
    uint32_t timerCnt = HAL_readTimerCnt(halHandle,2);
    CPU_TIME_updateCnts(cpu_timeHandle[timer_no], timerCnt);
}

/**
 * Place it at the line where the time measurement for specified timer (from 0 to CPU_TIME_NO) should stop
 * @param timer_no
 */
void TIMERS_stopCpuTimeMeasurement(int timer_no)
{
    // read the timer 1 value and update the CPU usage module
    int32_t timerCnt = HAL_readTimerCnt(halHandle,2);
    CPU_TIME_run(cpu_timeHandle[timer_no], timerCnt);
    CPU_TIME_calcRealTime(cpu_timeHandle[timer_no], USER_SYSTEM_FREQ_MHz);
}
