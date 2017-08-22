/****************************************************************************//**
 * @file instaspin.h
 * @brief Prototypes for functions of instaSPIN motor controller
 *
 * Created by Jakub Klein on 22 Aug 2017 for &reg; Radinn
 *
 * Following prototypes already exist in spintac_velocity.h:
 * - void ST_runVelMove(ST_Handle handle, CTRL_Handle ctrlHandle);
 * - void ST_runVelCtl(ST_Handle handle, CTRL_Handle ctrlHandle);
 ********************************************************************************/

#ifndef SRC_INSTASPIN_H_
#define SRC_INSTASPIN_H_




/***********************************************************************************************************************
 **                                                                                                          DEFINES  **
 ***********************************************************************************************************************/

//! \brief Defines the number of main iterations before global variables are updated
//!
#define NUM_MAIN_TICKS_FOR_GLOBAL_VARIABLE_UPDATE  1

//! \brief Defines the speed acceleration scale factor.
//!
#define MAX_ACCEL_KRPMPS_SF  _IQ(USER_MOTOR_NUM_POLE_PAIRS * 1000.0 / USER_TRAJ_FREQ_Hz / USER_IQ_FULL_SCALE_FREQ_Hz / 60.0)



//! \brief Initialization values of global variables
//!
#define MOTOR_Vars_INIT {true, \
                         false, \
                         false, \
                         true, \
                         false, \
                         false, \
                         true, \
                         false, \
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
                         _IQ(5.0), \
                         _IQ20(80.0), \
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





/***********************************************************************************************************************
 **                                                                                                          TYPEDEFS **
 ***********************************************************************************************************************/

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





/***********************************************************************************************************************
 **                                                                                                       PROTOTYPES  **
 ***********************************************************************************************************************/

//! @brief Perform all the tasks to initialize the controller.
//!
void INSTASPIN_init(void);


//! \brief     Performs all the tasks to run the controller
//!
void INSTASPIN_run(void);


//! \brief     Updates the global motor variables
//!
void updateGlobalVariables_motor(CTRL_Handle handle, ST_Handle stHandle);

//! \brief     Updates Kp and Ki gains in the controller object
//!
void updateKpKiGains(CTRL_Handle handle);

//! @brief Runs the Rs re-calibration online
//!
void runRsOnLine(CTRL_Handle handle);









#endif /* SRC_INSTASPIN_H_ */
