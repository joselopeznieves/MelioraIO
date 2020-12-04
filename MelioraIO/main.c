

/**
 * main.c
 */

#include <channel_interface.h>
#include <stdio.h>


#include "uart.h"
#include "uart_if.h"

#include "pin_mux.h"

#define DELAY 1


#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif

static void
BoardInit(void)
{
/* In case of TI-RTOS vector table is initialize by OS itself */
#ifndef USE_TIRTOS
    //
    // Set vector table base
    //
#if defined(ccs)
    MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif

    //
    // Enable Processor
    //
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);

    PRCMCC3200MCUInit();
}



int main(void)
{
    BoardInit();
    PinMuxConfig();
    InitPWMModules();
    ADCInit();


    WriteDigitalOutput(0, 1);
    UtilsDelay(80000000);
    WriteDigitalOutput(1, 1);
    UtilsDelay(80000000);
    WriteDigitalOutput(0, 0);
    UtilsDelay(80000000);
    WriteDigitalOutput(1, 0);
    UtilsDelay(80000000);
    WriteAnalogOutput(1, 0);
    UtilsDelay(80000000);
    WriteAnalogOutput(3, 0);
    UtilsDelay(80000000);

    WriteAnalogOutput(3, 0);
    UtilsDelay(80000000);
    WriteAnalogOutput(1, 0);
    UtilsDelay(80000000);





	return 0;

}
