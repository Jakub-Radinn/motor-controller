/*********************************************************************************
 * Motor-Controller/src/instaspin.h
 *
 * File containing headers for all code regarding instaSPIN motor controller
 *********************************************************************************/

#ifndef SRC_INSTASPIN_H_
#define SRC_INSTASPIN_H_



//! \brief     Performs tasks from instaSPIN labs placed before the main loop
//!
void INSTASPIN_init(void);


//! \brief     Performs tasks from instaSPIN labs placed inside the main loop
//!
void INSTASPIN_run(void);


//! \brief     Updates the global motor variables
//!
void updateGlobalVariables_motor(CTRL_Handle handle, ST_Handle stHandle);

//! \brief     Updates Kp and Ki gains in the controller object
//!
void updateKpKiGains(CTRL_Handle handle);




// Following prototypes already exist in spintac_velocity.h:
//void ST_runVelMove(ST_Handle handle, CTRL_Handle ctrlHandle);
//void ST_runVelCtl(ST_Handle handle, CTRL_Handle ctrlHandle);






#endif /* SRC_INSTASPIN_H_ */
