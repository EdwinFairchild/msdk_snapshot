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
 * $Date: 2021-10-08 12:25:59 -0500 (Fri, 08 Oct 2021) $
 * $Revision: 60369 $
 *
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "mxc_config.h"
#include "wsf_types.h"
#include "wsf_os.h"
#include "wsf_buf.h"
#include "wsf_timer.h"
#include "wsf_trace.h"
#include "app_ui.h"
#include "fit/fit_api.h"
#include "app_ui.h"
#include "hci_vs.h"
#include "hci_core.h"
#include "pb.h"
#include "tmr.h"
#include "ipc_defs.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/




/*************************************************************************************************/
        
        

/* Size of buffer for stdio functions */
#define WSF_BUF_POOLS       6
#define WSF_BUF_SIZE        0x1048

/* Size of buffer for stdio functions */
#define PRINTF_BUF_SIZE     128

/* Definitions for push button handling */
#define BUTTON0_TMR         MXC_TMR0
#define BUTTON1_TMR         MXC_TMR1
#define BUTTON_SHORT_MS     200
#define BUTTON_MED_MS       500
#define BUTTON_LONG_MS      1000

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

uint32_t SystemHeapSize = WSF_BUF_SIZE;
uint32_t SystemHeap[WSF_BUF_SIZE / 4];
uint32_t SystemHeapStart;

/*! Buffer for stdio functions */
char printf_buffer[PRINTF_BUF_SIZE];

// *INDENT-OFF*
/*! Default pool descriptor. */
static wsfBufPoolDesc_t mainPoolDesc[WSF_BUF_POOLS] = {
    {  16,  8 },
    {  32,  4 },
    {  64,  4 },
    { 128,  4 },
    { 256,  4 },
    { 512,  4 }
};
// *INDENT-ON*

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*! \brief  Stack initialization for app. */
extern void StackInitFit(void);

/*************************************************************************************************/
void SysTick_Handler(void)
{
    WsfTimerUpdate(WSF_MS_PER_TICK);
}

/*************************************************************************************************/
static bool_t myTrace(const uint8_t * pBuf, uint32_t len)
{
    extern uint8_t wsfCsNesting;

    if (wsfCsNesting == 0) {
        fwrite(pBuf, len, 1, stdout);
        return TRUE;
    }

    return FALSE;
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
    uint32_t bytesUsed;

    /* setup the systick for 1MS timer */
    SysTick->LOAD = (SystemCoreClock / 1000) * WSF_MS_PER_TICK;
    SysTick->VAL = 0;
    SysTick->CTRL |=
            (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk |
            SysTick_CTRL_TICKINT_Msk);

    WsfTimerInit();

    SystemHeapStart = (uint32_t) & SystemHeap;
    memset(SystemHeap, 0, sizeof(SystemHeap));
    printf("SystemHeapStart = 0x%x\n", SystemHeapStart);
    printf("SystemHeapSize = 0x%x\n", SystemHeapSize);
    bytesUsed = WsfBufInit(WSF_BUF_POOLS, mainPoolDesc);
    printf("bytesUsed = 0x%x\n", bytesUsed);

    WsfTraceRegisterHandler(myTrace);
    WsfTraceEnable(TRUE);
}

/*
 * In two-chip solutions, setting the address must wait until the HCI interface is initialized.
 * This handler can also catch other Application events, but none are currently implemented.
 * See ble-profiles/sources/apps/app/common/app_ui.c for further details.
 *
 */
void SetAddress(uint8_t event)
{
    uint8_t bdAddr[6] = { 0x02, 0x02, 0x44, 0x8B, 0x05, 0x00 };

    switch (event) {
    case APP_UI_RESET_CMPL:
        printf("Setting address -- MAC %02X:%02X:%02X:%02X:%02X:%02X\n",
                bdAddr[5], bdAddr[4], bdAddr[3], bdAddr[2], bdAddr[1],
                bdAddr[0]);
        HciVsSetBdAddr(bdAddr);
        break;
    default:
        break;
    }
}

/*************************************************************************************************/
void HandleButton(int button)
{
    mxc_tmr_regs_t *button_tmr = MXC_TMR_GET_TMR(button);

    // Check if rising or falling
    if (PB_Get(button)) {
        // Start timer 
        TMR_Enable(button_tmr);
    } else {
        uint32_t time;
        tmr_unit_t unit;

        // Get the elapsed time since the button was pressed
        TMR_GetTime(button_tmr, TMR_GetCount(button_tmr), &time, &unit);
        TMR_Disable(button_tmr);
        TMR_SetCount(button_tmr, 0);

        if (unit == TMR_UNIT_NANOSEC) {
            time /= 1000000;
        } else if (unit == TMR_UNIT_MICROSEC) {
            time /= 1000;
        }

        if (time < BUTTON_SHORT_MS) {
            AppUiBtnTest(button ? APP_UI_BTN_2_SHORT : APP_UI_BTN_1_SHORT);
        } else if (time < BUTTON_MED_MS) {
            AppUiBtnTest(button ? APP_UI_BTN_2_MED : APP_UI_BTN_1_MED);
        } else if (time < BUTTON_LONG_MS) {
            AppUiBtnTest(button ? APP_UI_BTN_2_LONG : APP_UI_BTN_1_LONG);
        } else {
            AppUiBtnTest(button ? APP_UI_BTN_2_EX_LONG : APP_UI_BTN_1_EX_LONG);
        }
    }
}

/*************************************************************************************************/
void Button0Pressed(void *arg)
{
    HandleButton(0);
}

/*************************************************************************************************/
void Button1Pressed(void *arg)
{
    HandleButton(1);
}

/*************************************************************************************************/
#if defined(ENABLE_SDMA) && defined(ENABLE_SDMA_ERROR_REPORTING)
/**
 * @brief Checks whether the SDMA was able to be initialized.
 *
 * @return A non-zero error code if something went wrong. */
int SDMACheckForError()
{
    uint8_t sdmaFlagState;
    uint8_t sdmaError;

    // Check if the SDMA is still initializing.
    sdmaFlagState = (SDMAGetSDMAFlag() & SDMA_FLAG_STATE);

    // Return any errors.
    sdmaError = 0;
    switch (sdmaFlagState) {
    case SDMA_FLAG_ENOMEM:
        sdmaError = sdmaFlagState;
        break;
    }
}
#else /* ENABLE_SDMA_ERROR_REPORTING && ENABLE_SDMA */
#define SDMACheckForError() (0)
#endif /* ENABLE_SDMA_ERROR_REPORTING && ENABLE_SDMA */

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
#ifdef ENABLE_SDMA
#ifdef ENABLE_SDMA_ERROR_REPORTING
    int sdmaError;
#endif /* ENABLE_SDMA_ERROR_REPORTING */
#endif /* ENABLE_SDMA */

#ifndef __IAR_SYSTEMS_ICC__
    setvbuf(stdout, printf_buffer, _IOLBF, PRINTF_BUF_SIZE);
#endif

    printf("\n\n***** MAX32665 BLE Fitness Profile *****\n");

    /* Initialize Radio */
    WsfInit();
    StackInitFit();
    FitStart();

    /* Setup pushbuttons and timers */
    PB_RegisterRiseFallCallback(0, Button0Pressed);
    PB_RegisterRiseFallCallback(1, Button1Pressed);
    PB_IntEnable(0);

    TMR_Init(BUTTON0_TMR, TMR_PRES_16, NULL);
    TMR_Init(BUTTON1_TMR, TMR_PRES_16, NULL);

    tmr_cfg_t button_config;

    button_config.mode = TMR_MODE_CONTINUOUS;
    TMR_Config(BUTTON0_TMR, &button_config);
    TMR_Config(BUTTON1_TMR, &button_config);

    /* Register a handler for Application events */
    AppUiActionRegister(SetAddress);

    printf("Setup Complete\n");

    while (1) {
        wsfOsDispatcher();

#ifdef ENABLE_SDMA
#ifdef ENABLE_SDMA_ERROR_REPORTING
        sdmaError = SDMACheckForError();
        if (sdmaError) {
            MXC_ASSERT(sdmaError == SDMA_FLAG_ENOMEM);
            printf("SDMA core ran out of allocatable memory.");
            fflush(stdout);

            while (1) {
            }
        }
#endif /* ENABLE_SDMA_ERROR_REPORTING */
#endif /* ENABLE_SDMA */
    }
}

/*****************************************************************/
void HardFault_Decoder(unsigned int *f)
{
  printf("\n-- HardFault --\n");
  printf("HFSR 0x%08X ", (unsigned int)SCB->HFSR);
  printf("CFSR 0x%08X", (unsigned int)SCB->CFSR);
  if (SCB->CFSR & 0x80) {
    printf(", MMFAR 0x%08X", (unsigned int)SCB->MMFAR);
  }
  if (SCB->CFSR & 0x8000) {
    printf(", BFAR 0x%08X", (unsigned int)SCB->BFAR);
  }
  printf("\n");

  /* Hang here */
  while(1);
}
