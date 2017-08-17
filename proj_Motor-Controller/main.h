#ifndef MAIN_H_
#define MAIN_H_


/***********************************************************************************************************************
 **                                                                                                         INCLUDES  **
 ***********************************************************************************************************************/
// modules
#include "sw/modules/math/src/32b/math.h"
#include "sw/modules/memCopy/src/memCopy.h"
#include "sw/modules/est/src/32b/est.h"
#include "sw/modules/svgen/src/32b/svgen_current.h"
#include "sw/modules/fw/src/32b/fw.h"
#include "sw/modules/slip/src/32b/slip.h"

// platforms
#include "sw/modules/ctrl/src/32b/ctrl.h"
#include "hal.h"
#include "user.h"

// SpinTAC
#include "sw/solutions/instaspin_motion/src/spintac_velocity.h"

/***********************************************************************************************************************
 **                                                                                                          DEFINES  **
 ***********************************************************************************************************************/

//! \brief Defines the delay functions - DELAY_US and DELAY_MS
//!
#define CPU_RATE   11.111L   // for a 90MHz CPU clock speed (SYSCLKOUT)
#define DELAY_US(A)  usDelay(((((long double) A * 1000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L)
#define DELAY_MS(A)  usDelay(((((long double) A * 1000000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L)


//! \brief Defines the number of main iterations before global variables are updated
//!
#define NUM_MAIN_TICKS_FOR_GLOBAL_VARIABLE_UPDATE  1

//! \brief Defines the speed acceleration scale factor.
//!
#define MAX_ACCEL_KRPMPS_SF  _IQ(USER_MOTOR_NUM_POLE_PAIRS * 1000.0 / USER_TRAJ_FREQ_Hz / USER_IQ_FULL_SCALE_FREQ_Hz / 60.0)

//! \brief Initialization values of global variables
//!
#define MOTOR_Vars_INIT {false, \
                         false, \
                         false, \
                         true, \
                         false, \
                         true, \
                         true, \
                         true, \
                         false, \
                         CTRL_State_Idle, \
                         EST_State_Idle, \
                         USER_ErrorCode_NoError, \
                         {0,CTRL_TargetProc_Unknown,0,0}, \
                         _IQ(0.0), \
                         _IQ(0.0), \
                         _IQ(0.1), \
                         _IQ(0.1), \
                         _IQ(0.0), \
                         _IQ(0.2), \
                         _IQ20(5.0), \
                         _IQ(0.0), \
                         _IQ(0.0), \
                         _IQ(USER_MAX_VS_MAG_PU), \
                         _IQ(0.1 * USER_MOTOR_MAX_CURRENT), \
                         400, \
                         _IQ(0.0), \
                         _IQ(0.0), \
                         0.0, \
                         0.0, \
                         0.0, \
                         0.0, \
                         0.0, \
                         0.0, \
                         0.0, \
                         _IQ(0.0), \
                         _IQ(0.0), \
                         _IQ(0.0), \
                         _IQ(0.0), \
                         _IQ(0.0), \
                         _IQ(0.0), \
                         _IQ(0.0), \
                         _IQ(0.8 * USER_MAX_VS_MAG_PU), \
                         _IQ(0.0), \
                         _IQ(0.0), \
                         _IQ(0.0), \
                         _IQ(0.0), \
                         {0,0,0}, \
                         {0,0,0}, \
                         ST_VARS_DEFAULTS}


// **************************************************************************
// the typedefs

typedef struct _MOTOR_Vars_t_
{
  bool Flag_enableSys;
  bool Flag_Run_Identify;
  bool Flag_MotorIdentified;
  bool Flag_enableForceAngle;
  bool Flag_enableFieldWeakening;
  bool Flag_enableRsRecalc;
  bool Flag_enableUserParams;
  bool Flag_enableOffsetcalc;
  bool Flag_enablePowerWarp;

  CTRL_State_e CtrlState;
  EST_State_e EstState;

  USER_ErrorCode_e UserErrorCode;

  CTRL_Version CtrlVersion;

  _iq IdRef_A;
  _iq IqRef_A;
  _iq StopSpeedRef_krpm;
  _iq SpeedRef_krpm;
  _iq SpeedTraj_krpm;
  _iq MaxAccel_krpmps;
  _iq20 MaxJrk_krpmps2;
  _iq Speed_krpm;
  _iq SpeedQEP_krpm;
  _iq OverModulation;
  _iq RsOnLineCurrent_A;
  _iq SvgenMaxModulation_ticks;
  _iq Flux_Wb;
  _iq Torque_Nm;

  float_t MagnCurr_A;
  float_t Rr_Ohm;
  float_t Rs_Ohm;
  float_t RsOnLine_Ohm;
  float_t Lsd_H;
  float_t Lsq_H;
  float_t Flux_VpHz;

  _iq Kp_spd;
  _iq Ki_spd;

  _iq Kp_Idq;
  _iq Ki_Idq;

  _iq Vd;
  _iq Vq;
  _iq Vs;
  _iq VsRef;
  _iq VdcBus_kV;

  _iq Id_A;
  _iq Iq_A;
  _iq Is_A;

  MATH_vec3 I_bias;
  MATH_vec3 V_bias;

  ST_Vars_t SpinTAC;

}MOTOR_Vars_t;


// **************************************************************************
// the globals


// **************************************************************************
// the function prototypes

//! \brief The main interrupt service (ISR) routine
//!
interrupt void mainISR(void);


//! \brief     Updates the global motor variables
//!
void updateGlobalVariables_motor(CTRL_Handle handle, ST_Handle stHandle);


//! \brief     Reset Ls Q format to a higher value when Ls identification starts
//!
void CTRL_resetLs_qFmt(CTRL_Handle handle, const uint_least8_t qFmt);


//! \brief     Recalculate Kp and Ki gains to fix the R/L limitation of 2000.0 and Kp limitation of 0.11
//! \brief     as well as recalculates gains based on estimator state to allow low inductance pmsm to id
//!
void recalcKpKiPmsm(CTRL_Handle handle);


//! \brief     Recalculate Kp and Ki gains to fix the R/L limitation of 2000.0 and Kp limitation of 0.11
//!
void recalcKpKi(CTRL_Handle handle);


//! \brief     Calculates the maximum qFmt value for Ls identification, to get a more accurate Ls per unit
//!
void CTRL_calcMax_Ls_qFmt(CTRL_Handle handle, uint_least8_t *p_qFmt);


//! \brief     Updates Kp and Ki gains in the controller object
//!
void updateKpKiGains(CTRL_Handle handle);


//! \brief     Set electrical frequency limit to zero while identifying an induction motor
//!
void setFeLimitZero(CTRL_Handle handle);


//! \brief     Calculates Dir_qFmt for ACIM
//!
void acim_Dir_qFmtCalc(CTRL_Handle handle);


//@} //defgroup
#endif // end of _MAIN_H_ definition



