/**********************************************************
 * The file contains all global variables definitions
 * Include "hdr/globals.h" (with 'extern' declarations)
 * in any other file you want to use those variables
 *
 **********************************************************/



/***********************************************************************************************************************
 **                                                                                                         INCLUDES  **
 ***********************************************************************************************************************/
#include "main.h"





/***********************************************************************************************************************
 **                                                                                                          GLOBALS  **
 ***********************************************************************************************************************/




// ------------------------------ InstaSPIN variables and objects -----------------------------

uint_least16_t gCounter_updateGlobals = 0;
uint16_t gLEDcnt = 0;
bool Flag_Latch_softwareUpdate = true;
volatile MOTOR_Vars_t gMotorVars = MOTOR_Vars_INIT;
USER_Params gUserParams;
CTRL_Obj *controller_obj;
CTRL_Handle ctrlHandle;
HAL_Handle halHandle;
HAL_PwmData_t gPwmData = {_IQ(0.0), _IQ(0.0), _IQ(0.0)};
HAL_AdcData_t gAdcData;
ST_Obj st_obj;
ST_Handle stHandle;
_iq gMaxCurrentSlope = _IQ(0.0);
_iq gFlux_pu_to_Wb_sf;
_iq gFlux_pu_to_VpHz_sf;
_iq gTorque_Ls_Id_Iq_pu_to_Nm_sf;
_iq gTorque_Flux_Iq_pu_to_Nm_sf;

// Watch window interface to the 8301 SPI
DRV_SPI_8301_Vars_t gDrvSpi8301Vars;
