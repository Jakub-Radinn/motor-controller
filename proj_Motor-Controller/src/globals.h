/****************************************************************************//**
 * @file globals.h
 * @brief The file contains declarations for global variables defined
 * in corresponding "globals.c". Include this header in any file that
 * uses global variables.
 *
 * Created by Jakub Klein on 22 Aug 2017 for &reg; Radinn
 ********************************************************************************/



#ifndef HDR_GLOBALS_H_
#define HDR_GLOBALS_H_


#include "main.h"



// ------------------------------ CPU Time calculation ---------------------------------

extern CPU_TIME_Handle cpu_timeHandle[CPU_TIME_NO];
extern CPU_TIME_Obj cpu_time[CPU_TIME_NO];



// ------------------------------ InstaSPIN variables and objects -----------------------------

extern uint_least16_t gCounter_updateGlobals;
extern uint16_t gLEDcnt;
extern bool Flag_Latch_softwareUpdate;
extern USER_Params gUserParams;
extern volatile MOTOR_Vars_t gMotorVars;
extern CTRL_Handle ctrlHandle;
extern CTRL_Obj *controller_obj;
extern HAL_Handle halHandle;
extern HAL_PwmData_t gPwmData;
extern HAL_AdcData_t gAdcData;
extern ST_Obj st_obj;
extern ST_Handle stHandle;
extern _iq gMaxCurrentSlope;
extern _iq gFlux_pu_to_Wb_sf;
extern _iq gFlux_pu_to_VpHz_sf;
extern _iq gTorque_Ls_Id_Iq_pu_to_Nm_sf;
extern _iq gTorque_Flux_Iq_pu_to_Nm_sf;

// Watch window interface to the 8301 SPI
extern DRV_SPI_8301_Vars_t gDrvSpi8301Vars;















#endif /* HDR_GLOBALS_H_ */
