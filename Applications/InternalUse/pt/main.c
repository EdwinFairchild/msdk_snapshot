/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 * $Date: 2018-08-10 09:36:39 -0500 (Fri, 10 Aug 2018) $
 * $Revision: 36825 $
 *
 ******************************************************************************/

/**
 * @file    main.c
 * @brief   Pulse train test file.
 */

/***** Includes *****/
#include <stdio.h>
#include "mxc_config.h"
#include "board.h"
#include "gpio.h"
#include "mxc_pins.h"
// #include "clkman.h"
#include "pt.h"
#include "nvic_table.h"

/***** Definitions *****/

/***** Globals *****/

/***** Functions *****/
static void PT_IRQ_Handler(void);

// *****************************************************************************

int PT_Init_Test()
{
    int error = 0;

    //set up interrupt
    NVIC_SetVector(PT_IRQn, PT_IRQ_Handler);
    MXC_PT->inten = 0xFFFF;
    PT_Init();

    if(MXC_PT->enable != 0)
        error = -1;

    if(MXC_PT->intfl != 0)
        error = -1;

    return error;
}

static void sqr_wave_handler()
{
    //static uint8_t changePattern = 1;

    if(PT_IsActive(MXC_PT, 9)) {
        PT_Stop(MXC_PT, 9);
        printf("PT Stopped\n");


    } else {
        /*if(changePattern)
        {
            PT_SetPattern(MXC_PT, , 9,0x36);
            changePattern = 0;
        }
        else
        {
            PT_SetPattern(MXC_PT, , 9,0xAA);
            changePattern = 1;
        }*/

        PT_Start(MXC_PT, 9);
        //printf("PT Started, pattern = 0x%08x\n",MXC_PT8->train);
    }
}

int Pulse_Train_Test(void)
{
    int error = 0;

    pt_pt_cfg_t ptConfig;

    //Setup GPIO to PT output function
    sys_cfg_pt_t gpio;
    // gpio.port = 3;
    // gpio.mask = PIN_0;
    // gpio.func = GPIO_FUNC_PT;
    // gpio.pad = GPIO_PAD_OPEN_DRAIN;
    //GPIO_Config(&gpio);
    gpio =  gpio_cfg_pt8_0;

    ptConfig.bps = 10;
    ptConfig.ptLength = 8;
    ptConfig.pattern = 0xAA;
    ptConfig.loop = 2;
    ptConfig.loopDelay = 0;

    PT_PTConfig(MXC_PT, 8, &ptConfig,&gpio);
    PT_SetRestart(MXC_PT, 8, 11, 0);
    PT_Start(MXC_PT, 8);

    return error;
}

int SquareWave_Test()
{
    int error = 0;

    //Setup GPIO to PT output function
    sys_cfg_pt_t gpio;
    // gpio.port = 3;
    // gpio.mask = PIN_1;
    // gpio.func = GPIO_FUNC_PT;
    // gpio.pad = GPIO_PAD_OPEN_DRAIN;
    // GPIO_Config(&gpio);
    gpio = gpio_cfg_pt9_0;

    PT_SqrWaveConfig(MXC_PT, 9, 10, &gpio);
    PT_Start(MXC_PT, 9);

    //Setup push button to start/stop pulse train
    // PB_RegisterCallback(0, sqr_wave_handler);

    return error;
}

// static void PB5_5_handler()
// {
//     if(PT_IsActiveMulti(0xC00)) {
//         PT_StopMulti(0xC00);
//         printf("stop multiple\n");
//         //PT_Resync(0xc00);
//         //printf("resync\n");
//     } else {
//         PT_RestartDisable(MXC_PT10, 0);
//         PT_StartMulti(0xC00);
//         printf("start multiple\n");
//     }

// }

int Restart_Test()
{
    int error = 0;

    pt_pt_cfg_t ptConfig;

    //Setup GPIO to PT10 output function
    sys_cfg_pt_t gpioPT10;
    // gpioPT10.port = 3;
    // gpioPT10.mask = PIN_2;
    // gpioPT10.func = GPIO_FUNC_PT;
    // gpioPT10.pad = GPIO_PAD_OPEN_DRAIN;
    //GPIO_Config(&gpio);
    gpioPT10 = gpio_cfg_pt10_0;

    //Setup GPIO to PT11 output function
    sys_cfg_pt_t gpioPT11;
    // gpioPT11.port = 3;
    // gpioPT11.mask = PIN_3;
    // gpioPT11.func = GPIO_FUNC_PT;
    // gpioPT11.pad = GPIO_PAD_OPEN_DRAIN;
    //GPIO_Config(&gpio);
    gpioPT11 = gpio_cfg_pt11_0;

    //Setup push button to start/stop pulse train
    // PB_RegisterCallback(1,PB5_5_handler);

    ptConfig.bps = 3;
    ptConfig.ptLength = 8;
    ptConfig.pattern = 0xAA;
    ptConfig.loop = 1;
    ptConfig.loopDelay = 0;

    PT_PTConfig(MXC_PT, 0, &ptConfig, &gpioPT10);
    PT_PTConfig(MXC_PT, 1, &ptConfig, &gpioPT11);

    PT_SetPattern(MXC_PT, 11, 0xCC);
    PT_SetRestart(MXC_PT, 11, 10, 0);
    PT_SetRestart(MXC_PT, 10, 11, 0);
    PT_SetRestart(MXC_PT, 10, 8, 1);
    
    return error;
}
static void PT_IRQ_Handler(void)
{
    printf("flags = 0x%08x\n", PT_GetFlags());

    PT_ClearFlags(0xFFFF);

    if(PT_GetFlags() != 0)
        printf("Flags not cleared\n");

}

// *****************************************************************************
int main(void)
{
    printf("program begin\n");

    int error = 0;

    PT_Init_Test();

#if 0
    Restart_Test();
#endif

#if 1
    error = Pulse_Train_Test();

#endif

#if 1
    SquareWave_Test();
#endif

    if(error == E_NO_ERROR) {
        printf("No Error :)\n");
    } else {
        printf("ERROR :(\n");
    }

    printf("program end\n");

    while(1) {}
    return 0;
}