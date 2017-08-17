/*********************************************************************************
 * Motor-Controller/src/instaspin.c
 *
 * File containing all code related to instaSPIN motor controller
 * INSTASPIN object encapsulates all functions used to drive the motor
 * as used in instaSPIN labs in main() function and main_ISR
 *********************************************************************************/

#include "globals.h"
#include "instaspin.h"




/***********************************************************************************************************************
 **                                                                                                   INSTASPIN_init  **
 ***********************************************************************************************************************/
void INSTASPIN_init(void)
{
    uint_least8_t estNumber = 0;
    uint_least8_t ctrlNumber = 0;


    // initialize the hardware abstraction layer
    halHandle = HAL_init(&hal,sizeof(hal));

    // check for errors in user parameters
    USER_checkForErrors(&gUserParams);

    // store user parameter error in global variable
    gMotorVars.UserErrorCode = USER_getErrorCode(&gUserParams);

    // do not allow code execution if there is a user parameter error
    if(gMotorVars.UserErrorCode != USER_ErrorCode_NoError)
      {
        for(;;)
          {
            gMotorVars.Flag_enableSys = false;
          }
      }
    else
    {
        gMotorVars.Flag_enableSys = true;
    }

    // initialize the user parameters
    USER_setParams(&gUserParams);

    // set the hardware abstraction layer parameters
    HAL_setParams(halHandle,&gUserParams);

    // initialize the controller
    ctrlHandle = CTRL_initCtrl(ctrlNumber, estNumber);        //v1p6 format (06xF and 06xM devices)
    controller_obj = (CTRL_Obj *)ctrlHandle;

    CTRL_Version version;

    // get the version number
    CTRL_getVersion(ctrlHandle,&version);

    gMotorVars.CtrlVersion = version;

    // set the default controller parameters
    CTRL_setParams(ctrlHandle,&gUserParams);

    // setup faults
    HAL_setupFaults(halHandle);

    // initialize the interrupt vector table
    HAL_initIntVectorTable(halHandle);

    // enable the ADC interrupts
    HAL_enableAdcInts(halHandle);

    // enable global interrupts
    HAL_enableGlobalInts(halHandle);

    // enable debug interrupts
    HAL_enableDebugInt(halHandle);

    // disable the PWM
    HAL_disablePwm(halHandle);

    // initialize the SpinTAC Components
    stHandle = ST_init(&st_obj, sizeof(st_obj));

    // setup the SpinTAC Components
    ST_setupVelCtl(stHandle);
    ST_setupVelMove(stHandle);

    // turn on the DRV8301 if present
    HAL_enableDrv(halHandle);
    // initialize the DRV8301 interface
    HAL_setupDrvSpi(halHandle,&gDrvSpi8301Vars);

    // enable DC bus compensation
    CTRL_setFlag_enableDcBusComp(ctrlHandle, true);

    // compute scaling factors for flux and torque calculations
    gFlux_pu_to_Wb_sf = USER_computeFlux_pu_to_Wb_sf();
    gFlux_pu_to_VpHz_sf = USER_computeFlux_pu_to_VpHz_sf();
    gTorque_Ls_Id_Iq_pu_to_Nm_sf = USER_computeTorque_Ls_Id_Iq_pu_to_Nm_sf();
    gTorque_Flux_Iq_pu_to_Nm_sf = USER_computeTorque_Flux_Iq_pu_to_Nm_sf();
}




/***********************************************************************************************************************
 **                                                                                                    INSTASPIN_run  **
 ***********************************************************************************************************************/
void INSTASPIN_run(void)
{
    // Dis-able the Library internal PI.  Iq has no reference now
    CTRL_setFlag_enableSpeedCtrl(ctrlHandle, false);

    // loop while the enable system flag is true
    if(gMotorVars.Flag_enableSys)
      {
        CTRL_Obj *obj = (CTRL_Obj *)ctrlHandle;
        ST_Obj *stObj = (ST_Obj *)stHandle;

        // increment counters
        gCounter_updateGlobals++;

        // enable/disable the use of motor parameters being loaded from user.h
        CTRL_setFlag_enableUserMotorParams(ctrlHandle,gMotorVars.Flag_enableUserParams);

        // enable/disable Rs recalibration during motor startup
        EST_setFlag_enableRsRecalc(obj->estHandle,gMotorVars.Flag_enableRsRecalc);

        // enable/disable automatic calculation of bias values
        CTRL_setFlag_enableOffset(ctrlHandle,gMotorVars.Flag_enableOffsetcalc);


        if(CTRL_isError(ctrlHandle))
          {
            // set the enable controller flag to false
            CTRL_setFlag_enableCtrl(ctrlHandle,false);

            // set the enable system flag to false
            gMotorVars.Flag_enableSys = false;

            // disable the PWM
            HAL_disablePwm(halHandle);
          }
        else
          {
            // update the controller state
            bool flag_ctrlStateChanged = CTRL_updateState(ctrlHandle);

            // enable or disable the control
            CTRL_setFlag_enableCtrl(ctrlHandle, gMotorVars.Flag_Run_Identify);

            if(flag_ctrlStateChanged)
              {
                CTRL_State_e ctrlState = CTRL_getState(ctrlHandle);

                if(ctrlState == CTRL_State_OffLine)
                  {
                    // enable the PWM
                    HAL_enablePwm(halHandle);
                  }
                else if(ctrlState == CTRL_State_OnLine)
                  {
                    if(gMotorVars.Flag_enableOffsetcalc == true)
                    {
                      // update the ADC bias values
                      HAL_updateAdcBias(halHandle);
                    }
                    else
                    {
                      // set the current bias
                      HAL_setBias(halHandle,HAL_SensorType_Current,0,_IQ(I_A_offset));
                      HAL_setBias(halHandle,HAL_SensorType_Current,1,_IQ(I_B_offset));
                      HAL_setBias(halHandle,HAL_SensorType_Current,2,_IQ(I_C_offset));

                      // set the voltage bias
                      HAL_setBias(halHandle,HAL_SensorType_Voltage,0,_IQ(V_A_offset));
                      HAL_setBias(halHandle,HAL_SensorType_Voltage,1,_IQ(V_B_offset));
                      HAL_setBias(halHandle,HAL_SensorType_Voltage,2,_IQ(V_C_offset));
                    }

                    // Return the bias value for currents
                    gMotorVars.I_bias.value[0] = HAL_getBias(halHandle,HAL_SensorType_Current,0);
                    gMotorVars.I_bias.value[1] = HAL_getBias(halHandle,HAL_SensorType_Current,1);
                    gMotorVars.I_bias.value[2] = HAL_getBias(halHandle,HAL_SensorType_Current,2);

                    // Return the bias value for voltages
                    gMotorVars.V_bias.value[0] = HAL_getBias(halHandle,HAL_SensorType_Voltage,0);
                    gMotorVars.V_bias.value[1] = HAL_getBias(halHandle,HAL_SensorType_Voltage,1);
                    gMotorVars.V_bias.value[2] = HAL_getBias(halHandle,HAL_SensorType_Voltage,2);

                    // enable the PWM
                    HAL_enablePwm(halHandle);
                  }
                else if(ctrlState == CTRL_State_Idle)
                  {
                    // disable the PWM
                    HAL_disablePwm(halHandle);
                    gMotorVars.Flag_Run_Identify = false;
                  }

                if((CTRL_getFlag_enableUserMotorParams(ctrlHandle) == true) &&
                  (ctrlState > CTRL_State_Idle) &&
                  (gMotorVars.CtrlVersion.minor == 6))
                  {
                    // call this function to fix 1p6
                    USER_softwareUpdate1p6(ctrlHandle);
                  }

              }
          }


        if(EST_isMotorIdentified(obj->estHandle))
          {
            // set the current ramp
            EST_setMaxCurrentSlope_pu(obj->estHandle,gMaxCurrentSlope);
            gMotorVars.Flag_MotorIdentified = true;

            // set the speed reference
            CTRL_setSpd_ref_krpm(ctrlHandle,gMotorVars.SpeedRef_krpm);

            // set the speed acceleration
            CTRL_setMaxAccel_pu(ctrlHandle,_IQmpy(MAX_ACCEL_KRPMPS_SF,gMotorVars.MaxAccel_krpmps));

            // enable the SpinTAC Speed Controller
            STVELCTL_setEnable(stObj->velCtlHandle, true);

            if(EST_getState(obj->estHandle) != EST_State_OnLine)
            {
                // if the estimator is not running, place SpinTAC into reset
                STVELCTL_setEnable(stObj->velCtlHandle, false);
                // if the estimator is not running, set SpinTAC Move start & end velocity to 0
                STVELMOVE_setVelocityEnd(stObj->velMoveHandle, _IQ(0.0));
                STVELMOVE_setVelocityStart(stObj->velMoveHandle, _IQ(0.0));
            }

            if(Flag_Latch_softwareUpdate)
            {
              Flag_Latch_softwareUpdate = false;

              USER_calcPIgains(ctrlHandle);

              // initialize the watch window kp and ki current values with pre-calculated values
              gMotorVars.Kp_Idq = CTRL_getKp(ctrlHandle,CTRL_Type_PID_Id);
              gMotorVars.Ki_Idq = CTRL_getKi(ctrlHandle,CTRL_Type_PID_Id);

              // initialize the watch window Bw value with the default value
              gMotorVars.SpinTAC.VelCtlBw_radps = STVELCTL_getBandwidth_radps(stObj->velCtlHandle);

              // initialize the watch window with maximum and minimum Iq reference
              gMotorVars.SpinTAC.VelCtlOutputMax_A = _IQmpy(STVELCTL_getOutputMaximum(stObj->velCtlHandle), _IQ(USER_IQ_FULL_SCALE_CURRENT_A));
              gMotorVars.SpinTAC.VelCtlOutputMin_A = _IQmpy(STVELCTL_getOutputMinimum(stObj->velCtlHandle), _IQ(USER_IQ_FULL_SCALE_CURRENT_A));
            }

          }
        else
          {
            Flag_Latch_softwareUpdate = true;

            // the estimator sets the maximum current slope during identification
            gMaxCurrentSlope = EST_getMaxCurrentSlope_pu(obj->estHandle);
          }


        // when appropriate, update the global variables
        if(gCounter_updateGlobals >= NUM_MAIN_TICKS_FOR_GLOBAL_VARIABLE_UPDATE)
          {
            // reset the counter
            gCounter_updateGlobals = 0;

            updateGlobalVariables_motor(ctrlHandle, stHandle);
          }


        // update Kp and Ki gains
        updateKpKiGains(ctrlHandle);

        // set the SpinTAC (ST) bandwidth scale
        STVELCTL_setBandwidth_radps(stObj->velCtlHandle, gMotorVars.SpinTAC.VelCtlBw_radps);

        // set the maximum and minimum values for Iq reference
        STVELCTL_setOutputMaximums(stObj->velCtlHandle, _IQmpy(gMotorVars.SpinTAC.VelCtlOutputMax_A, _IQ(1.0/USER_IQ_FULL_SCALE_CURRENT_A)), _IQmpy(gMotorVars.SpinTAC.VelCtlOutputMin_A, _IQ(1.0/USER_IQ_FULL_SCALE_CURRENT_A)));

        // enable/disable the forced angle
        EST_setFlag_enableForceAngle(obj->estHandle,gMotorVars.Flag_enableForceAngle);

        // enable or disable power warp
        CTRL_setFlag_enablePowerWarp(ctrlHandle,gMotorVars.Flag_enablePowerWarp);

        HAL_writeDrvData(halHandle,&gDrvSpi8301Vars);

        HAL_readDrvData(halHandle,&gDrvSpi8301Vars);
      } // end of while(gFlag_enableSys) loop

    else
    {
        // disable the PWM
        HAL_disablePwm(halHandle);

        // set the default controller parameters (Reset the control to re-identify the motor)
        CTRL_setParams(ctrlHandle,&gUserParams);
        gMotorVars.Flag_Run_Identify = false;

        // setup the SpinTAC Components
        ST_setupVelCtl(stHandle);
        ST_setupVelMove(stHandle);
    }
}





/***********************************************************************************************************************
 **                                                                                      updateGlobalVariables_motor  **
 ***********************************************************************************************************************/
void updateGlobalVariables_motor(CTRL_Handle handle, ST_Handle sthandle)
{
  CTRL_Obj *obj = (CTRL_Obj *)handle;
  ST_Obj *stObj = (ST_Obj *)sthandle;
  int32_t tmp;

  // get the speed estimate
  gMotorVars.Speed_krpm = EST_getSpeed_krpm(obj->estHandle);

  // get the real time speed reference coming out of the speed trajectory generator
  gMotorVars.SpeedTraj_krpm = _IQmpy(STVELMOVE_getVelocityReference(stObj->velMoveHandle),EST_get_pu_to_krpm_sf(obj->estHandle));

  // get the torque estimate
  gMotorVars.Torque_Nm = USER_computeTorque_Nm(handle, gTorque_Flux_Iq_pu_to_Nm_sf, gTorque_Ls_Id_Iq_pu_to_Nm_sf);

  // when calling EST_ functions that return a float, and fpu32 is enabled, an integer is needed as a return
  // so that the compiler reads the returned value from the accumulator instead of fpu32 registers
  // get the magnetizing current
  tmp = EST_getIdRated(obj->estHandle);
  gMotorVars.MagnCurr_A = *((float_t *)&tmp);

  // get the rotor resistance
  tmp = EST_getRr_Ohm(obj->estHandle);
  gMotorVars.Rr_Ohm = *((float_t *)&tmp);

  // get the stator resistance
  tmp = EST_getRs_Ohm(obj->estHandle);
  gMotorVars.Rs_Ohm = *((float_t *)&tmp);

  // get the stator inductance in the direct coordinate direction
  tmp = EST_getLs_d_H(obj->estHandle);
  gMotorVars.Lsd_H = *((float_t *)&tmp);

  // get the stator inductance in the quadrature coordinate direction
  tmp = EST_getLs_q_H(obj->estHandle);
  gMotorVars.Lsq_H = *((float_t *)&tmp);

  // get the flux in V/Hz in floating point
  tmp = EST_getFlux_VpHz(obj->estHandle);
  gMotorVars.Flux_VpHz = *((float_t *)&tmp);

  // get the flux in Wb in fixed point
  gMotorVars.Flux_Wb = USER_computeFlux(handle, gFlux_pu_to_Wb_sf);

  // get the controller state
  gMotorVars.CtrlState = CTRL_getState(handle);

  // get the estimator state
  gMotorVars.EstState = EST_getState(obj->estHandle);

  // Get the DC buss voltage
  gMotorVars.VdcBus_kV = _IQmpy(gAdcData.dcBus,_IQ(USER_IQ_FULL_SCALE_VOLTAGE_V/1000.0));

  // get the Iq reference from the speed controller
  gMotorVars.IqRef_A = _IQmpy(STVELCTL_getTorqueReference(stObj->velCtlHandle), _IQ(USER_IQ_FULL_SCALE_CURRENT_A));

  // gets the Velocity Controller status
  gMotorVars.SpinTAC.VelCtlStatus = STVELCTL_getStatus(stObj->velCtlHandle);

  // get the inertia setting
  gMotorVars.SpinTAC.InertiaEstimate_Aperkrpm = _IQmpy(STVELCTL_getInertia(stObj->velCtlHandle), _IQ(ST_SPEED_PU_PER_KRPM * USER_IQ_FULL_SCALE_CURRENT_A));

  // get the friction setting
  gMotorVars.SpinTAC.FrictionEstimate_Aperkrpm = _IQmpy(STVELCTL_getFriction(stObj->velCtlHandle), _IQ(ST_SPEED_PU_PER_KRPM * USER_IQ_FULL_SCALE_CURRENT_A));

  // get the Velocity Controller error
  gMotorVars.SpinTAC.VelCtlErrorID = STVELCTL_getErrorID(stObj->velCtlHandle);

  // get the Velocity Move status
  gMotorVars.SpinTAC.VelMoveStatus = STVELMOVE_getStatus(stObj->velMoveHandle);

  // get the Velocity Move profile time
  gMotorVars.SpinTAC.VelMoveTime_ticks = STVELMOVE_getProfileTime_tick(stObj->velMoveHandle);

  // get the Velocity Move error
  gMotorVars.SpinTAC.VelMoveErrorID = STVELMOVE_getErrorID(stObj->velMoveHandle);

  return;
} // end of updateGlobalVariables_motor() function





/***********************************************************************************************************************
 **                                                                                                  updateKpKiGains  **
 ***********************************************************************************************************************/
void updateKpKiGains(CTRL_Handle handle)
{
  if((gMotorVars.CtrlState == CTRL_State_OnLine) && (gMotorVars.Flag_MotorIdentified == true) && (Flag_Latch_softwareUpdate == false))
    {
      // set the kp and ki speed values from the watch window
      CTRL_setKp(handle,CTRL_Type_PID_spd,gMotorVars.Kp_spd);
      CTRL_setKi(handle,CTRL_Type_PID_spd,gMotorVars.Ki_spd);

      // set the kp and ki current values for Id and Iq from the watch window
      CTRL_setKp(handle,CTRL_Type_PID_Id,gMotorVars.Kp_Idq);
      CTRL_setKi(handle,CTRL_Type_PID_Id,gMotorVars.Ki_Idq);
      CTRL_setKp(handle,CTRL_Type_PID_Iq,gMotorVars.Kp_Idq);
      CTRL_setKi(handle,CTRL_Type_PID_Iq,gMotorVars.Ki_Idq);
    }

  return;
} // end of updateKpKiGains() function





/***********************************************************************************************************************
 **                                                                                                    ST_runVelMove  **
 ***********************************************************************************************************************/
void ST_runVelMove(ST_Handle handle, CTRL_Handle ctrlHandle)
{

    ST_Obj *stObj = (ST_Obj *)handle;
    CTRL_Obj *ctrlObj = (CTRL_Obj *)ctrlHandle;

    // Run SpinTAC Move
    // If we are not in reset, and the SpeedRef_krpm has been modified
    if((EST_getState(ctrlObj->estHandle) == EST_State_OnLine) && (_IQmpy(gMotorVars.SpeedRef_krpm, _IQ(ST_SPEED_PU_PER_KRPM)) != STVELMOVE_getVelocityEnd(stObj->velMoveHandle))) {
        // Get the configuration for SpinTAC Move
        STVELMOVE_setCurveType(stObj->velMoveHandle, gMotorVars.SpinTAC.VelMoveCurveType);
        STVELMOVE_setVelocityEnd(stObj->velMoveHandle, _IQmpy(gMotorVars.SpeedRef_krpm, _IQ(ST_SPEED_PU_PER_KRPM)));
        STVELMOVE_setAccelerationLimit(stObj->velMoveHandle, _IQmpy(gMotorVars.MaxAccel_krpmps, _IQ(ST_SPEED_PU_PER_KRPM)));
        STVELMOVE_setJerkLimit(stObj->velMoveHandle, _IQ20mpy(gMotorVars.MaxJrk_krpmps2, _IQ20(ST_SPEED_PU_PER_KRPM)));
        // Enable SpinTAC Move
        STVELMOVE_setEnable(stObj->velMoveHandle, true);
        // If starting from zero speed, enable ForceAngle, otherwise disable ForceAngle
        if(_IQabs(STVELMOVE_getVelocityStart(stObj->velMoveHandle)) < _IQ(ST_MIN_ID_SPEED_PU)) {
            EST_setFlag_enableForceAngle(ctrlObj->estHandle, true);
            gMotorVars.Flag_enableForceAngle = true;
        }
        else {
            EST_setFlag_enableForceAngle(ctrlObj->estHandle, false);
            gMotorVars.Flag_enableForceAngle = false;
        }
    }
    STVELMOVE_run(stObj->velMoveHandle);
}





/***********************************************************************************************************************
 **                                                                                                     ST_runVelCtl  **
 ***********************************************************************************************************************/
void ST_runVelCtl(ST_Handle handle, CTRL_Handle ctrlHandle)
{

    _iq speedFeedback, iqReference;
    ST_Obj *stObj = (ST_Obj *)handle;
    CTRL_Obj *ctrlObj = (CTRL_Obj *)ctrlHandle;

    // Get the mechanical speed in pu
    speedFeedback = EST_getFm_pu(ctrlObj->estHandle);

    // Run the SpinTAC Controller
    STVELCTL_setVelocityReference(stObj->velCtlHandle, STVELMOVE_getVelocityReference(stObj->velMoveHandle));
    STVELCTL_setAccelerationReference(stObj->velCtlHandle, STVELMOVE_getAccelerationReference(stObj->velMoveHandle));
    STVELCTL_setVelocityFeedback(stObj->velCtlHandle, speedFeedback);
    STVELCTL_run(stObj->velCtlHandle);

    // select SpinTAC Velocity Controller
    iqReference = STVELCTL_getTorqueReference(stObj->velCtlHandle);

    // Set the Iq reference that came out of SpinTAC Velocity Control
    CTRL_setIq_ref_pu(ctrlHandle, iqReference);
}
