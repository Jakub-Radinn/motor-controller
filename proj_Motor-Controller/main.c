/****************************************************************************//**
 * @file main.c
 * @brief This is the basic framework for a project that uses HAL library with DRV8301 driver,
 * built upon proj_lab04a project from TI (C).
 *
 *  * Required directories structure is:
 *      <PARENT> directory:
 *          - <PROJECT> directory containing this project
 *          - "sw" directory copied from C:/ti/motorware (mind the project-related changes made in source files)
 *
 * To move the project to a different location, just copy the whole <PROJECT> directory and then import it to CCS.
 * You can change name of <PROJECT> directory before importing and then rename the project (right click -> rename...) without complications.
 * The output files are placed in the "Release" directory created when building the project.
 *
 * Created by Jakub Klein on 22 Aug 2017 for &reg; Radinn
 ********************************************************************************/

// system includes
#include <math.h>
#include "main.h"
#include "src/globals.h"

#include <stdio.h>


long gLoopCnt = 0;


/***********************************************************************************************************************
 **                                                                                                    MAIN FUNCTION  **
 ***********************************************************************************************************************/
void main(void)
{

    INSTASPIN_init();

    TIMERS_init();


    /*******************************************************************************************************************
     **                                                                                                  PROGRAM LOOP **
     *******************************************************************************************************************/
    printf("Entering MAIN LOOP\n");

	while(1)
	{

	    TIMERS_stopCpuTimeMeasurement(0);       // Stop time measurement
	    TIMERS_startCpuTimeMeasurement(0);      // Start time measurement



	    INSTASPIN_run();



	} // end of while(1) loop
}   // end of main() function




/***********************************************************************************************************************
 **                                                                                              INTERRUPTION ROUTINE **
 ***********************************************************************************************************************/
interrupt void mainISR(void)
{
    static uint16_t stCnt = 0;

    TIMERS_startCpuTimeMeasurement(1);          // Start time measurement

    // toggle status LED
    if(++gLEDcnt >= (uint_least32_t)(USER_ISR_FREQ_Hz / LED_BLINK_FREQ_Hz))
    {
      HAL_toggleLed(halHandle,(GPIO_Number_e)HAL_Gpio_LED2);
      gLEDcnt = 0;
    }

    // acknowledge the ADC interrupt
    HAL_acqAdcInt(halHandle,ADC_IntNumber_1);

    // convert the ADC data
    HAL_readAdcData(halHandle,&gAdcData);

    // Run the SpinTAC Components
    if(stCnt++ >= ISR_TICKS_PER_SPINTAC_TICK) {
        ST_runVelMove(stHandle, ctrlHandle);
        ST_runVelCtl(stHandle, ctrlHandle);
        stCnt = 1;
    }

    // run the controller
    CTRL_run(ctrlHandle,halHandle,&gAdcData,&gPwmData);

    // write the PWM compare values
    HAL_writePwmData(halHandle,&gPwmData);

    // setup the controller
    CTRL_setup(ctrlHandle);


    TIMERS_stopCpuTimeMeasurement(1);       // Stop time measurement


    return;
}



// end of file
