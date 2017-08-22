/****************************************************************************//**
 * @file main.h
 * @brief The file contains basic includes and macros for the program.
 *
 * Created by Jakub Klein on 22 Aug 2017 for &reg; Radinn
 ********************************************************************************/




#ifndef MAIN_H_
#define MAIN_H_


/***********************************************************************************************************************
 **                                                                                                         INCLUDES  **
 ***********************************************************************************************************************/
// user created
#include "src/defines.h"
#include "src/instaspin.h"
#include "src/timers.h"

// modules
#include "sw/modules/math/src/32b/math.h"
#include "sw/modules/memCopy/src/memCopy.h"
#include "sw/modules/est/src/32b/est.h"
#include "sw/modules/svgen/src/32b/svgen_current.h"
#include "sw/modules/fw/src/32b/fw.h"
#include "sw/modules/slip/src/32b/slip.h"
#include "sw/modules/cpu_time/src/32b/cpu_time.h"

// platforms
#include "sw/modules/ctrl/src/32b/ctrl.h"
#include "hal.h"
#include "user.h"

// SpinTAC
#include "sw/solutions/instaspin_motion/src/spintac_velocity.h"



/***********************************************************************************************************************
 **                                                                                                          DEFINES  **
 ***********************************************************************************************************************/

//! \brief Defines the macros for delay functions - DELAY_US() and DELAY_MS()
//!
#define CPU_RATE   11.111L   // for a 90MHz CPU clock speed (SYSCLKOUT)
#define DELAY_US(A)  usDelay(((((long double) A * 1000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L)
#define DELAY_MS(A)  usDelay(((((long double) A * 1000000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L)




/***********************************************************************************************************************
 **                                                                                                       PROTOTYPES  **
 ***********************************************************************************************************************/

//! \brief The main interrupt service (ISR) routine
//!
interrupt void mainISR(void);




//@} //defgroup
#endif // end of _MAIN_H_ definition



