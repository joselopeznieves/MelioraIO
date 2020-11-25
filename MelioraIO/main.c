

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
    InitTerm();


    Report("Reading From Digital Input...\n\r");
    Report("Read: %d\n\r", ReadDigitalInput(0));
    UtilsDelay(DELAY);

    Report("Reading Digital Input...\n\r");
    Report("Read: %d\n\r", ReadDigitalInput(0));
    UtilsDelay(DELAY);

    Report("Writing 1 to Digital Output (Wet)...\n\r");
    WriteDigitalOutput(0, 1);
    UtilsDelay(DELAY);

    Report("Writing 0 to Digital Output (Wet)...\n\r");
    WriteDigitalOutput(0, 0);
    UtilsDelay(DELAY);

    Report("Writing 1 to Digital Output (Dry)...\n\r");
    WriteDigitalOutput(2, 1);
    UtilsDelay(DELAY);

    Report("Writing 0 to Digital Output (Dry)...\n\r");
    WriteDigitalOutput(2, 0);
    UtilsDelay(DELAY);

    Report("Reading Analog Input...\n\r");
    Report("\n\rVoltage is %f\n\r", ReadAnalogInput(0));
    UtilsDelay(DELAY);

    Report("Reading Analog Input...\n\r");
    Report("\n\rVoltage is %f\n\r", ReadAnalogInput(0));
    UtilsDelay(DELAY);

    Report("Writing 0V to Analog Output...\n\r");
    WriteAnalogOutput(0, 0);
    UtilsDelay(DELAY);

    Report("Writing 12V to Analog Output...\n\r");
    WriteAnalogOutput(0, 12);
    UtilsDelay(DELAY);

    Report("Writing 24V to Analog Output...\n\r");
    WriteAnalogOutput(0, 24);
    UtilsDelay(DELAY);


	return 0;

}
