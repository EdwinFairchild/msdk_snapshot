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
 * @brief   PBM test file.
 */
// #include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "mxc_config.h"
#include "mxc_assert.h"

#include "pb.h"
#include "pbm.h"
#include "LPM012A260A.h"
#include "tmr_utils.h"

#define DISP_W      ( 216 ) /* Number of columns */
#define DISP_H      ( 180 ) /* Number of rows */
#define TICKS_TO_NS ( 25 )  /* Number of nanoseconds per systick*/


/***** Variables *****/
static uint8_t disp_buffer[DISP_H][DISP_W];
static uint8_t mem_buffer[DISP_H][DISP_W];

/*These are defined by the user*/
const gpio_cfg_t gpio_cfg_vlcd = { MXC_GPIO1, PIN_24, GPIO_FUNC_OUT, GPIO_PAD_HI_Z }; // 3V2, J16.5
const gpio_cfg_t gpio_cfg_vddp = { MXC_GPIO1, PIN_25, GPIO_FUNC_OUT, GPIO_PAD_HI_Z }; // 4V5, J16.3
const gpio_cfg_t gpio_cfg_vddc = { MXC_GPIO1, PIN_26, GPIO_FUNC_OUT, GPIO_PAD_HI_Z }; // 6V5, J16.1

pbm_cfg_t cfg = {
    0x00000000,               // version

    1,                        // ignore_skip_rows
    // PBM_MEMORY,               // output to memory
    PBM_DISPLAY,              // output_to_display
    0,                        // new_jdi_mode

    DISP_H,                   // rows
    DISP_W,                   // columns

    (uint32_t*) disp_buffer,  // uint32_t* read_buffer_addr
    (uint32_t*) mem_buffer,   // uint32_t* write_buffer_addr

    0,                        // prescaler
    (13120/TICKS_TO_NS),      // vst_delay
    (137110/TICKS_TO_NS),     // vst_pw
    (13530/TICKS_TO_NS),      // vck_delay
    (91130/TICKS_TO_NS),      // vck_pw
    16,                       // vck_skip_delay
    40,                       // vck_skip_pw
    (6480/TICKS_TO_NS),       // enb_delay
    (76550/TICKS_TO_NS),      // enb_pw
    (0/TICKS_TO_NS),          // hst_delay
    (810/TICKS_TO_NS),        // hst_pw
    (410/TICKS_TO_NS),        // hck_delay
    (810/TICKS_TO_NS),        // hck_pw
    0,                        // vcom_delay
    68,                       // vcom_pw
    (12300/TICKS_TO_NS),      // xrst_delay
    (33167700/TICKS_TO_NS),   // xrst_pw
};

/***** Functions *****/
void test1(void){
    // Testing output to display, set skip rows, and reset skip rows functionality
    // int pbval;

    while(1){
        printf("Showing Red, Red, Red\n");
        memset( disp_buffer, 0x30, sizeof( disp_buffer ) );
        PBM_UpdateDisplay((uint32_t*) disp_buffer);

        // printf("Press SW14 (Toggle P1.31) to advance to next pattern.\n\n");
        // pbval = PB_Get(0);
        // while(PB_Get(0) == pbval);
        TMR_Delay(MXC_TMR0, 500000); //0.5s

        PBM_SetSkipRows(0, (PBM_GetDisplayHeight()/3));

        printf("Showing Red, White, White\n");
        memset( disp_buffer, 0x3F, sizeof( disp_buffer ) );
        PBM_UpdateDisplay((uint32_t*) disp_buffer);

        // printf("Press SW14 (Toggle P1.31) to advance to next pattern.\n\n");
        // pbval = PB_Get(0);
        // while(PB_Get(0) == pbval);
        TMR_Delay(MXC_TMR0, 500000); //0.5s

        PBM_SetSkipRows(0, 2*(PBM_GetDisplayHeight()/3));

        printf("Showing Red, White, Blue\n");
        memset( disp_buffer, 0x03, sizeof( disp_buffer ) );
        PBM_UpdateDisplay((uint32_t*) disp_buffer);

        // // printf("Press SW14 (Toggle P1.31) to advance to next pattern.\n\n");
        // // pbval = PB_Get(0);
        // // while(PB_Get(0) == pbval);
        TMR_Delay(MXC_TMR0, 500000); //0.5s

        PBM_ResetAllSkipRows();
    }
}
int main(void)
{
    printf("\n\nPBM Example\n");

    LPM012A260A_Init(&gpio_cfg_vlcd, &gpio_cfg_vddp, &gpio_cfg_vddc, MXC_TMR0);
    PBM_Init(&cfg);

    test1();

    return 1;
}