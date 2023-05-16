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
 * $Date: 2020-10-19 12:19:32 -0500 (Mon, 19 Oct 2020) $
 * $Revision: 56418 $
 *
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "mxc_config.h"
#include "mxc_delay.h"
#include "wsf_types.h"
#include "wsf_os.h"
#include "wsf_buf.h"
#include "wsf_timer.h"
#include "wsf_trace.h"
#include "app_ui.h"
#include "app_ui.h"
#include "hci_vs.h"
#include "hci_core.h"
#include "dm_api.h"
#include "bb_drv.h"
#include "board.h"
#include "pb.h"
#include "tmr.h"
#include "uart.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Size of buffer for stdio functions */
#define WSF_BUF_POOLS       6
#define WSF_BUF_SIZE        0x1048

/* Size of buffer for stdio functions */
#define PRINTF_BUF_SIZE     128

/* Definitions for push button handling */
#define BUTTON0_TMR         MXC_TMR0
#define BUTTON_SHORT_MS     200
#define BUTTON_MED_MS       500
#define BUTTON_LONG_MS      1000

#define AI85_CMD_UART       MXC_UART0
#define AI85_CMD_UART_MAP   MAP_A
#define AI85_CMD_UART_BAUD  115200
#define AI85_CMD_UNLOCK     'U'

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

const uart_cfg_t cmd_uart_cfg = {
    .parity = UART_PARITY_DISABLE,
    .size   = UART_DATA_SIZE_8_BITS,
    .stop   = UART_STOP_1,
    .flow   = UART_FLOW_CTRL_DIS,
    .pol    = UART_FLOW_POL_DIS,
    .baud   = AI85_CMD_UART_BAUD,
    .clksel = UART_CLKSEL_SYSTEM
};

const sys_cfg_uart_t cmd_uart_sys_cfg = {
    .map = AI85_CMD_UART_MAP,
    .flow = Disable
};

uint32_t SystemHeapSize=WSF_BUF_SIZE;
uint32_t SystemHeap[WSF_BUF_SIZE/4];
uint32_t SystemHeapStart;

/*! Buffer for stdio functions */
char printf_buffer[PRINTF_BUF_SIZE];

/*! Default pool descriptor. */
static wsfBufPoolDesc_t mainPoolDesc[WSF_BUF_POOLS] =
{
  {  16,  8 },
  {  32,  4 },
  {  64,  4 },
  { 128,  4 },
  { 256,  4 },
  { 512,  4 }
};

extern bool_t resetFlag;

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*! \brief  Stack initialization for app. */
extern void StackInitCube(void);
extern void CubeStart(void);

/*************************************************************************************************/
void SysTick_Handler(void)
{
    WsfTimerUpdate(WSF_MS_PER_TICK);
}

/*************************************************************************************************/
static bool_t myTrace(const uint8_t *pBuf, uint32_t len)
{
    extern uint8_t wsfCsNesting;

    if (wsfCsNesting == 0)
    {
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
    /* setup the systick for 1MS timer*/
    SysTick->LOAD = (SystemCoreClock / 1000) * WSF_MS_PER_TICK;
    SysTick->VAL = 0;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);

    WsfTimerInit();

    SystemHeapStart = (uint32_t)&SystemHeap;
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
    uint8_t usn[SYS_USN_CHECKSUM_LEN];
    uint8_t checksum[SYS_USN_CHECKSUM_LEN];
    uint8_t bdAddr[6];
    uint8_t bdAddrRand[6];

    if(SYS_GetUSN(usn, checksum) != E_NO_ERROR) {
        printf("Error getting Checksum\n");
    }

    // MA-L assigend by IEEE to Maxim Integrated Products
    bdAddr[5] = 0x00;
    bdAddr[4] = 0x18;
    bdAddr[3] = 0x80;

    // USN checksum
    bdAddr[2] = checksum[2];
    bdAddr[1] = checksum[1];
    bdAddr[0] = checksum[0];

    bdAddrRand[0] = checksum[3];
    bdAddrRand[1] = checksum[4];
    bdAddrRand[2] = checksum[5];
    bdAddrRand[3] = checksum[6];
    bdAddrRand[4] = checksum[7];
    bdAddrRand[5] = checksum[8];

    switch (event) {
    case APP_UI_RESET_CMPL:
        HciVsSetBdAddr(bdAddr);
        DM_RAND_ADDR_SET(bdAddrRand, DM_RAND_ADDR_STATIC);
        DmDevSetRandAddr(bdAddrRand);
        printf("Setting random address -- MAC %02X:%02X:%02X:%02X:%02X:%02X\n", 
            bdAddrRand[5], bdAddrRand[4], bdAddrRand[3], bdAddrRand[2], bdAddrRand[1], bdAddrRand[0]);
        break;
    default:
        break;
    }
}

/*************************************************************************************************/
void HandleButton(int button)
{
    mxc_tmr_regs_t* button_tmr = MXC_TMR_GET_TMR(button);

    // Check if rising or falling
    if(PB_Get(button)) {
        // Start timer 
        TMR_Enable(button_tmr);
    } else {
        uint32_t time;
        tmr_unit_t unit;

        // Get the elapsed time since the button was pressed
        TMR_GetTime(button_tmr, TMR_GetCount(button_tmr), &time, &unit);
        TMR_Disable(button_tmr);
        TMR_SetCount(button_tmr, 0);

        if(unit == TMR_UNIT_NANOSEC) {
            time /= 1000000;
        } else if(unit == TMR_UNIT_MICROSEC) {
            time /= 1000;
        }

        if(time < BUTTON_SHORT_MS) {
            APP_TRACE_INFO1("Button %d SHORT", button);
            AppUiBtnTest(button ? APP_UI_BTN_2_SHORT : APP_UI_BTN_1_SHORT);
        } else if (time < BUTTON_MED_MS) {
            APP_TRACE_INFO1("Button %d MED", button);
            AppUiBtnTest(button ? APP_UI_BTN_2_MED : APP_UI_BTN_1_MED);
        } else if (time < BUTTON_LONG_MS) {
            APP_TRACE_INFO1("Button %d LONG", button);
            AppUiBtnTest(button ? APP_UI_BTN_2_LONG : APP_UI_BTN_1_LONG);    
        } else {
            APP_TRACE_INFO1("Button %d EX_LONG", button);
            AppUiBtnTest(button ? APP_UI_BTN_2_EX_LONG : APP_UI_BTN_1_EX_LONG);    
        }
    } 
}

/*************************************************************************************************/
void Button0Pressed(void* arg)
{
    HandleButton(0);
}

void handleCommand(void)
{
    uint8_t cmd;

    if(UART_NumReadAvail(AI85_CMD_UART) == 0) {
        return;
    }

    cmd = UART_ReadByte(AI85_CMD_UART);
    switch(cmd) {

        case AI85_CMD_UNLOCK:
        {
            printf("Received unlock command\n");
            AppUiBtnTest(APP_UI_BTN_1_SHORT);
            break;
        }
        default:
        {
            printf("Unrecognized command character\n");
            break;
        }
    }
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
#ifdef ENABLE_SDMA
    uint8_t sdmaFlag;
#endif /* ENABLE_SDMA */

#ifndef __IAR_SYSTEMS_ICC__
    setvbuf(stdout, printf_buffer, _IOLBF, PRINTF_BUF_SIZE);
#endif

    printf("\n\n***** MAX32665 Cube *****\n");

    /* Setup pushbuttons and timers */
    PB_RegisterRiseFallCallback(0, Button0Pressed);
    PB_IntEnable(0);

    TMR_Init(BUTTON0_TMR, TMR_PRES_16, NULL);

    tmr_cfg_t button_config;
    button_config.mode = TMR_MODE_CONTINUOUS;
    TMR_Config(BUTTON0_TMR, &button_config);

    /* Setup command UART */
    if (UART_Init(AI85_CMD_UART, &cmd_uart_cfg, &cmd_uart_sys_cfg) != E_NO_ERROR) {
        printf("Error initializing CMD UART\n");
    } else {
        printf("Command UART initialized\n");
    }

    /* Use VDDIO instead of VDDIOH */
    MXC_GPIO0->vssel = 0;

    UART_Enable(AI85_CMD_UART);

    /* Initialize Radio */
    WsfInit();
    StackInitCube();
    CubeStart();

    /* Register a handler for Application events */
    AppUiActionRegister(SetAddress);
    
    printf("Setup Complete\n");

    while (1)
    {
        wsfOsDispatcher();
        handleCommand();
    }
}

/*****************************************************************/
void HardFault_Handler(void)
{
    printf("\nFaultISR: CFSR %08X, BFAR %08x\n", (unsigned int)SCB->CFSR, (unsigned int)SCB->BFAR);

    // Loop forever
    while(1);
}
