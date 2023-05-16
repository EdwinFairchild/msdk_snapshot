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
 * @file    	main.c
 * @brief   	AES Example
 * @details 	Encryption and decryption of AES on different modes (ECB and OFB) with different bit sizes (128, 192, and 256)
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_config.h"
#include "mxc_sys.h"
#include "tmr_utils.h"

/***** Definitions *****/

/***** Globals *****/

/***** Functions *****/

// *****************************************************************************
int main(void)
{
    PB_Init();
    printf("MAX32650 EvKit Example\n");
    
    printf("Press SW3 to start AES\n");
    while(!PB_Get(2));
    aes_main();
        
    printf("Press SW3 to start CRC\n");
    while(!PB_Get(2));
    crc_main();
        
    printf("Press SW3 to start Data Cache\n");
    while(!PB_Get(2));
    dcc_main();
        
    printf("Press SW3 to start DMA\n");
    while(!PB_Get(2));
    dma_main();
        
    printf("Press SW3 to start Flash\n");
    while(!PB_Get(2));
    flash_main();
        
    printf("Press SW3 to start GPIO\n");
    while(!PB_Get(2));
    gpio_main();
        
    printf("Press SW2 and SW3 to skip I2C\n");
    printf("Please Connect P2.7 and P.17\n");
    printf("Also Connect P2.8 and P2.18\n");
    printf("Press SW3 to start I2C\n");
    while(!PB_Get(2));
    TMR_Delay(MXC_TMR0, MSEC(25));
    if(!PB_Get(1)) {
        i2c_main();
    }
        
    printf("Press SW3 to start Instruction Cache\n");
    while(!PB_Get(2));
    icc_main();
        
    printf("Press SW3 to start MAA\n");
    while(!PB_Get(2));
    maa_main();
        
    printf("Press SW3 to start Pulse Train\n");
    while(!PB_Get(2));
    pt_main();	
        
    printf("Press SW3 to start RTC\n");
    while(!PB_Get(2));
    rtc_main();
        
    printf("Press SW2 and SW3 to skip SDHC\n");
    printf("Please insert an SDHC Card\n");
    printf("Press SW3 to start SDHC\n");
    while(!PB_Get(2));
    TMR_Delay(MXC_TMR0, MSEC(25));
    if(!PB_Get(1)) {
        sdhc_example();
    }
        
    printf("Press SW3 to start Semaphore\n");
    while(!PB_Get(2));
    sema_main();
        
    printf("Press SW2 and SW3 to skip SPI\n");
    printf("Please connect some pins that\n");
    printf("I don't know right now\n");
    printf("It's the SPIS to SPIM connection\n");
    printf("Press SW3 to start SPI\n");
    while(!PB_Get(2));
    TMR_Delay(MXC_TMR0, MSEC(25));
    if(!PB_Get(1)) {
        spi_main();
    }
        
    printf("Press SW3 to start TRNG\n");
    while(!PB_Get(2));
    trng_main();
        
    printf("Press SW2 and SW3 to skip USB\n");
    printf("After Starting the Demo,\n");
    printf("Connnect your Computer to the USB Device Port\n");
    printf("Press SW3 to start USB\n");
    while(!PB_Get(2));
    TMR_Delay(MXC_TMR0, MSEC(25));
    if(!PB_Get(1)) {
        usb_main();
    }
        
    printf("Press SW3 to start Watchdog Timer\n");
    while(!PB_Get(2));
    wdt_main();

    printf("Examples Complete!\n");
    while(1) {}
}
