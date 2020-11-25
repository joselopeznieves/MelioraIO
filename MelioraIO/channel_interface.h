/*
 * channel_interface.c
 *
 *  Created on: Nov 15, 2020
 *      Author: jose
 */

#include <pin_mux.h>
#include <stdio.h>

#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "interrupt.h"
#include "hw_apps_rcm.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "prcm.h"
#include "gpio.h"
#include "utils.h"
#include "timer.h"
#include "adc.h"

#define TIMER_INTERVAL_RELOAD   800 // =(400*2)
#define DUTYCYCLE_GRANULARITY   2


//****************************************************************************
//
//! Update the dutycycle of the PWM timer
//!
//! \param ulBase is the base address of the timer to be configured
//! \param ulTimer is the timer to be setup (TIMER_A or  TIMER_B)
//! \param ucLevel translates to duty cycle settings (0:255)
//!
//! This function
//!    1. The specified timer is setup to operate as PWM
//!
//! \return None.
//
//****************************************************************************
void UpdateDutyCycle(unsigned long ulBase, unsigned long ulTimer,
                     unsigned int ucLevel){
    //
    // Match value is updated to reflect the new dutycycle settings
    //
    TimerMatchSet(ulBase,ulTimer, (ucLevel*DUTYCYCLE_GRANULARITY));
}

//****************************************************************************
//
//! Setup the timer in PWM mode
//!
//! \param ulBase is the base address of the timer to be configured
//! \param ulTimer is the timer to be setup (TIMER_A or  TIMER_B)
//! \param ulConfig is the timer configuration setting
//! \param ucInvert is to select the inversion of the output
//!
//! This function
//!    1. The specified timer is setup to operate as PWM
//!
//! \return None.
//
//****************************************************************************
void SetupTimerPWMMode(unsigned long ulBase, unsigned long ulTimer,
                       unsigned long ulConfig, unsigned char ucInvert){
    //
    // Set GPT - Configured Timer in PWM mode.
    //
    TimerConfigure(ulBase,ulConfig);
    TimerPrescaleSet(ulBase,ulTimer,0);

    //
    // Inverting the timer output if required
    //
    TimerControlLevel(ulBase,ulTimer,ucInvert);

    //
    // Load value set to ~0.5 ms time period
    //
    TimerLoadSet(ulBase,ulTimer,TIMER_INTERVAL_RELOAD);

    //
    // Match value set so as to output level 0
    //
    TimerMatchSet(ulBase,ulTimer,TIMER_INTERVAL_RELOAD);

}

//****************************************************************************
//
//! Sets up the identified timers as PWM to drive the peripherals
//!
//! \param none
//!
//! This function sets up the folowing
//!    1. TIMERA2 (TIMER B) as RED of RGB light
//!    2. TIMERA3 (TIMER B) as YELLOW of RGB light
//!    3. TIMERA3 (TIMER A) as GREEN of RGB light
//!
//! \return None.
//
//****************************************************************************
void InitPWMModules(){
    //
    // Initialization of timers to generate PWM output
    //
    PRCMPeripheralClkEnable(PRCM_TIMERA2, PRCM_RUN_MODE_CLK);

    //
    // TIMERA2 (TIMER B) as RED of RGB light. GPIO 9 --> PWM_5
    //
    SetupTimerPWMMode(TIMERA2_BASE, TIMER_B,
            (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM), 1);

    TimerEnable(TIMERA2_BASE,TIMER_B);

}

//****************************************************************************
//
//! Disables the timer PWMs
//!
//! \param none
//!
//! This function disables the timers used
//!
//! \return None.
//
//****************************************************************************
void DeInitPWMModules(){
    //
    // Disable the peripherals
    //
    TimerDisable(TIMERA2_BASE, TIMER_B);
    PRCMPeripheralClkDisable(PRCM_TIMERA2, PRCM_RUN_MODE_CLK);

}

//*****************************************************************************
//
//! ADC Initialization
//!
//! \param  None
//!
//! \return None
//
//*****************************************************************************

void ADCInit(void){
#ifdef CC3200_ES_1_2_1
        //
        // Enable ADC clocks.###IMPORTANT###Need to be removed for PG 1.32
        //
        HWREG(GPRCM_BASE + GPRCM_O_ADC_CLK_CONFIG) = 0x00000043;
        HWREG(ADC_BASE + ADC_O_ADC_CTRL) = 0x00000004;
        HWREG(ADC_BASE + ADC_O_ADC_SPARE0) = 0x00000100;
        HWREG(ADC_BASE + ADC_O_ADC_SPARE1) = 0x0355AA00;
#endif

        //
        // Configure ADC timer which is used to timestamp the ADC data samples
        //
        ADCTimerConfig(ADC_BASE,2^17);

        //
        // Enable ADC timer which is used to timestamp the ADC data samples
        //
        ADCTimerEnable(ADC_BASE);

        //
        // Enable ADC module
        //
        ADCEnable(ADC_BASE);

        //
        // Enable ADC channel
        //

        ADCChannelEnable(ADC_BASE, ADC_CH_2);
}

//****************************************************************************
//
//! Reads the specified digital input channel
//!
//! \param channel the desired digital input channel to be read (0, 1, 2, 3).
//!
//! \return The read state of the input channel, 1 for high or 0 low.
//
//****************************************************************************

int ReadDigitalInput(int channel){

    long in_buffer = -1;

    switch(channel){
        case 0:
            in_buffer = (GPIOPinRead(GPIOA0_BASE, 0x20) >> 5);
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
    }

    return in_buffer;


}

//****************************************************************************
//
//! Writes 1 or 0 to the specified digital output channel. Channels 0 and 1 are
//! Wet contact channels, 2 and 3 are Dry contact channels.
//!
//! \param channel the desired digital output channel to be written (0, 1, 2, 3).
//! \param state the high or low state to be written (1 or 0).
//!
//! \return None
//
//****************************************************************************

void WriteDigitalOutput(int channel, int state){

    switch(channel){
        case 0:
            if(state)
                GPIOPinWrite(GPIOA2_BASE, 0x40, 0x40);
            else
                GPIOPinWrite(GPIOA2_BASE, 0x40, 0);
            break;
        case 1:
            break;
        case 2:
            if(state)
                GPIOPinWrite(GPIOA1_BASE, 0x4, 0x4);
            else
                GPIOPinWrite(GPIOA1_BASE, 0x4, 0);
            break;
        case 3:
            break;
    }
}

//****************************************************************************
//
//! Reads the specified analog input channel
//!
//! \param channel the desired analog input channel to be read (0, 1, 2, 3).
//!
//! \return The voltage value read by input channel.
//
//****************************************************************************

float ReadAnalogInput(int channel){

    unsigned long adc_out = 0;
    float adc_voltage = 0;

    switch(channel){
        case 0:
            adc_out = ADCFIFORead(ADC_BASE, ADC_CH_2);
            adc_voltage = (((float)((adc_out >> 2 ) & 0x0FFF))*1.5)/4096;
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
    }

    return adc_voltage;
}

//****************************************************************************
//
//! Writes the desired voltage value to the analog output channel.
//!
//! \param channel the desired analog output channel to be written (0, 1, 2, 3).
//! \param voltage the voltage value to be written (0-24V)
//!
//! \return None
//
//****************************************************************************

void WriteAnalogOutput(int channel, float voltage){

    int compare = 16.6667 * voltage;

    if(compare >= 400)
        compare = 399;

    switch(channel){
        case 0:
            UpdateDutyCycle(TIMERA2_BASE, TIMER_B, compare);
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
    }
}





