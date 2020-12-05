

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


    WriteAnalogOutput(0, 0);
    WriteAnalogOutput(0, 6);
    WriteAnalogOutput(0, 12);
    WriteAnalogOutput(0, 24);

    WriteAnalogOutput(1, 0);
    WriteAnalogOutput(1, 6);
    WriteAnalogOutput(1, 12);
    WriteAnalogOutput(1, 24);

    WriteAnalogOutput(2, 0);
    WriteAnalogOutput(2, 6);
    WriteAnalogOutput(2, 12);
    WriteAnalogOutput(2, 24);

    WriteAnalogOutput(3, 0);
    WriteAnalogOutput(3, 6);
    WriteAnalogOutput(3, 12);
    WriteAnalogOutput(3, 24);

	return 0;

}
