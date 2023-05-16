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
 * $Date: 2018-08-27 11:13:26 -0500 (Mon, 27 Aug 2018) $
 * $Revision: 37347 $
 *
 ******************************************************************************/

/**
 * @file    main.c
 * @brief   SPIM test file.
 * @details Put a jumper across P0.5 and P0.6 to loopback MOSI to MISO
 */

/***** Includes *****/
#include <stdio.h>
#include <string.h>
#include "mxc_config.h"
#include "spim.h"
#include "mx25_med.h"

/***** Definitions *****/
#define BUFF_SIZE   256

/***** Globals *****/
uint8_t txdata[BUFF_SIZE];
uint8_t rxdata[BUFF_SIZE];

/***** Functions *****/

/******************************************************************************/
int prog_test(void)
{
    int error, i;

    // Initialize the SPIM
    spim_cfg_t cfg;
    cfg.mode = 0;
    cfg.ssel_pol = 0;
    cfg.baud = 10000000;
    cfg.baud = 10000000;


    if((error = SPIM_Init(MXC_SPIM2, &cfg, NULL)) != E_NO_ERROR) {
        printf("Error initializing SPIM %d\n", error);
        return -1;
    } else {
        printf("SPIM Initialized\n");
    }

    // Initialize the MX25
    MX25_med_init(MXC_SPIM2, 0);
    MX25_med_reset();
    printf("MX25 Initialized\n");

    // Erase the test sector
    MX25_med_erase(0, MX25_Erase_4K);

    if(MX25_med_quad(1) != E_NO_ERROR) {
        printf("Error enabling quad mode\n");
        return -1;
    }

    // Program the MX25
    for(i = 0; i < BUFF_SIZE; i++) {
        txdata[i] = i;
    }
    MX25_med_program_page(0, txdata, BUFF_SIZE, SPIM_WIDTH_1);

    // Read back the memory
    MX25_med_read(0, rxdata, BUFF_SIZE, SPIM_WIDTH_1);

    if((error = memcmp(rxdata, txdata, BUFF_SIZE)) != 0) {
        printf("Error verifying MX25 memory %d\n", error);
        return -1;
    } else {
        printf("MX25 memory verified\n");
    }

    // Disable the SPIM
    if((error = SPIM_Init(MXC_SPIM2, &cfg, NULL)) != E_NO_ERROR) {
        printf("Error Disabling SPIM %d\n", error);
        return -1;
    } else {
        printf("SPIM Disabling\n");
    }

    return 0;
}


/******************************************************************************/
int main(void)
{
    int retval = 0;

    printf("***** SPIX Driver test *****\n");

#if 1
    if(prog_test() != 0) {
        printf("Error with prog_test\n");
        while(1) {}
    }
#endif

    if(retval != 0) {
        printf("You are not prepared..\n");
    } else {
        printf("Tests Passed!\n");
    }

    while(1) {}
    return 0;
}
