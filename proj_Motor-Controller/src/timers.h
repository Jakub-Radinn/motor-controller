/****************************************************************************//**
 * @file timers.h
 * @brief Function prototypes for timers related functions
 *
 * Created by Jakub Klein on 22 Aug 2017 for &reg; Radinn
 ********************************************************************************/

#ifndef SRC_TIMERS_H_
#define SRC_TIMERS_H_


//! @brief Define the number of time measurements used
#define CPU_TIME_NO         3



//! @brief Initialize timers used in the code
void TIMERS_init(void);

//! @brief Start selected time measurement
void TIMERS_startCpuTimeMeasurement(int timer_no);

//! @brief Start selected time measurement
void TIMERS_stopCpuTimeMeasurement(int timer_no);







#endif /* SRC_TIMERS_H_ */
