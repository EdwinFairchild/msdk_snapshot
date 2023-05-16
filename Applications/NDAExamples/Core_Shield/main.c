/*******************************************************************************
 * Copyright (C) 2018 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2019-11-08 11:13:40 -0600 (Fri, 08 Nov 2019) $
 * $Revision: 48553 $
 *
 ******************************************************************************/

#include <stdio.h>
#include <string.h>

/* SDK */
#include "mxc_config.h"
#include "led.h"
#include "pb.h"

/* Registers we shouldn't need to touch, but for which the SDK doesn't provide an API */
#include "gcr_regs.h"

/* Cordio stack */
#include "wsf_types.h"
#include "wsf_os.h"
#include "wsf_buf.h"
#include "wsf_timer.h"
#include "wsf_trace.h"
#include "ll_api.h"
#include "sch_api.h"
#include "wsf_assert.h"
#include "dats/dats_api.h"


/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Number of WSF buffer pools */
#define WSF_BUF_POOLS              6

#define PRINTF_BUF_SIZE 128

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

char printf_buffer[PRINTF_BUF_SIZE];

/*! Free memory for pool buffers (use word elements for word alignment). */
static uint32_t mainBufMem[3584/sizeof(uint32_t)+96];

/*! Default pool descriptor. */
static wsfBufPoolDesc_t mainPoolDesc[WSF_BUF_POOLS] =
{
  {  16,  8 },
  {  32,  4 },
  {  64,  4 },
  { 128,  4 },
  { 256,  4 },
  { 384,  4 }
};

volatile unsigned int systick_10ms = 0;

/**************************************************************************************************
  Functions
**************************************************************************************************/

int crypto_init(void);
int crypto_test(void);
int crypto_example(void);

int chademo_usb(void);

/*! \brief  Stack initialization for app. */
extern void StackInitDats(void);

/*************************************************************************************************/
void SysTick_Handler(void)
{
    WsfTimerUpdate(WSF_MS_PER_TICK);
    systick_10ms++; /* Assumes WSF_MS_PER_TICK == 10 */
}

/*************************************************************************************************/
static void myTrace(const char *pStr, va_list args)
{
    extern uint8_t wsfCsNesting;

    if (wsfCsNesting == 0)
    {
        vprintf(pStr, args);
        printf("\r\n");
    }
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize WSF.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void WsfInit(void)
{
    WsfTimerInit();
    WsfBufInit(sizeof(mainBufMem), (uint8_t*)mainBufMem, WSF_BUF_POOLS, mainPoolDesc);
    WsfTraceRegister(myTrace);
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize platform.
 *
 *  \param  msPerTick   Milliseconds per timer tick.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PlatformInit(void)
{
    /* Set VREGO_D to 1.3V */
    *((volatile uint32_t*)0x40004410) = 0x50;

    /* Set TX LDO to 1.1V and enable LDO. Set RX LDO to 0.9V and enable LDO */
    MXC_GCR->btleldocn = 0xD9; // TX 1.1V RX 0.9V

    /* Power up the 32MHz XO */
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_X32M_EN;

    /* Enable peripheral clocks */
    MXC_GCR->perckcn0 &= ~(MXC_F_GCR_PERCKCN0_GPIO0D | MXC_F_GCR_PERCKCN0_GPIO1D);  // Clear GPIO0 and GPIO1 Disable
    MXC_GCR->perckcn1 &= ~(MXC_F_GCR_PERCKCN1_BTLED | MXC_F_GCR_PERCKCN1_TRNGD );  // Clear BTLE and ICACHE0 disable
    
    /* setup the systick */
    SysTick->LOAD = (SystemCoreClock / 1000) * WSF_MS_PER_TICK;
    SysTick->VAL = 0;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize MAC layer.
 *
 *  \param  None.
 *
 *  \return None.
 */
/*************************************************************************************************/
void MacInit(void)
{
    wsfHandlerId_t handlerId;

    /* Initialize link layer. */
    BbInit();
    handlerId = WsfOsSetNextHandler(SchHandler);
    SchInit(handlerId);
    LlAdvSlaveInit();
    LlConnSlaveInit();
    handlerId = WsfOsSetNextHandler(LlHandler);
    LlHandlerInit(handlerId);
}

void led_asserted(void)
{
    /* Show human that board is faulted */
    LED_Off(0);
    LED_On(1); /* Red */
    //LED_Off(2);
    while(1);
}

void trigger(void *pb)
{
  PB_RegisterCallback(0, NULL);
  WsfAssert(__FILE__, __LINE__);
}

/*************************************************************************************************/
/*!
 *  \fn     main
 *
 *  \brief  Entry point for demo software.
 *
 *  \param  None.
 *
 *  \return None.
 */
/*************************************************************************************************/
int main(void)
{
    /* Added to prevent printf() and friends from using malloc() */
    setvbuf(stdout, printf_buffer, _IOLBF, PRINTF_BUF_SIZE);
  
    // Test all LEDs
    LED_On(0); /* Blue */
    LED_On(1); /* Red */
    //LED_On(2); /* Green */
    for(uint32_t i=0;i<(1<<23);i++) { __asm("nop"); }

    /* keep LEDs on until setup complete */
//    LED_Off(0); /* Blue */
//    LED_Off(1); /* Red */
//    LED_Off(2); /* Green */

    PB_RegisterCallback(0, trigger);
    
    printf("\n\n***** MAX32650 DATS *****\n");

    /* Wait for USB, enumerate, then fall through */
    chademo_usb();

    /* Init library */
    if (crypto_init() < 0) {
	    LED_On(1); /* Red Failure LED lit */
	printf("UCL init failure. HALT\n");
	while (1);
    }

    /* Run self-test */
    if (crypto_test() < 0) {
    	LED_On(1); /* Red Failure LED lit */
	printf("UCL self-test failure. HALT\n");
	while (1);
    }
    
//    LED_On(0);  /* Blue */
//    LED_Off(1); /* Red */
//    LED_Off(2); /* Green */

    WsfAssertRegister(led_asserted);

    PlatformInit();

    /* Start BLE app */
    WsfInit();
    MacInit();
    StackInitDats();
    DatsStart();

    printf("Setup Complete\n");

    while (1)
    {
	/* Handle WSF events */
	wsfOsDispatcher();
    }
}
