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
 * @brief   SPIS example using the SPIM0 and SS0.
 * @details Uses the on board master and slave on the EvKit to show the SPIS. It sends
 * data from the master to the slave. Then the slave inverts the data that it receives and sends
 * it back to the master. This example is showing half duplex and shows both master and slave being Async,
 * master being Sync while the slave is Async, and the slave Sync and the master Async.  It is not possible
 * for both to be Sync so that is not shown here.  It can be full duplex by sending both tx and rx at the same
 * time instead of NULL for one of them.
 *
 *  To do this you will need to connect P0.4 to P4.4,P0.5 to P4.5,P0.6 to P4.6,
 * P0.7 to P4.7 with jumper wires.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_config.h"
//#include "clkman.h"
//#include "ioman.h"
#include "spim.h"
#include "spis.h"
#include "nvic_table.h"
#include "tmr.h"
#include "board.h"
#include "gpio.h"


/***** Definitions *****/
#define CLK_RATE            10000000  //hz
#define BUFF_SIZE           25
//This will add print statements to see what is going on
#define VERBOSE             0

/***** Globals *****/

volatile int spis_flag;
volatile int spim_flag;
uint8_t tx_master[BUFF_SIZE];
uint8_t rx_master[BUFF_SIZE];
uint8_t tx_slave[BUFF_SIZE];
uint8_t rx_slave[BUFF_SIZE];
spim_req_t spim1_req;
spis_req_t spis_req;
gpio_cfg_t temp ;

/***** Functions *****/
void spis_callback(spis_req_t* req, int error) {
    spis_flag = error;
}

/******************************************************************************/
void SPI1_IRQHandler(void) {

    SPIS_Handler(MXC_SPIS);
}

/******************************************************************************/
void spim_callback(spim_req_t* req, int error) {
    spim_flag = error;
}

/******************************************************************************/
void SPI2_IRQHandler(void) {
    SPIM_Handler(MXC_SPIM1);
}

/******************************************************************************/
void FIFO_Clear() {
    SPIS_DrainRX(MXC_SPIS);
    SPIS_DrainTX(MXC_SPIS);
    SPIM_DrainRX(MXC_SPIM1);
    SPIM_DrainTX(MXC_SPIM1);
}


/******************************************************************************/
void TMR2_handler() {
    int error;

    TMR_IntClear(MXC_TMR2);
    //start master as
    if ((error = SPIM_TransAsync(MXC_SPIM1, &spim1_req)) != E_NO_ERROR) {
        printf("Error with Slave Transferring Data Asynchronously: %d\n", error);
        while (1) {}
    }
    TMR_Disable(MXC_TMR2);

}

/*****************************************************************************/
int SPISTransTimer() {
    int error = 0;
    tmr_cfg_t TMR2_cfg;
    tmr_pres_t prescale = TMR_PRES_4096;
    uint32_t timeOut = 50; //ms

    NVIC_SetVector(TMR2_IRQn, TMR2_handler);

    //initialize timer

    if (error != E_NO_ERROR)
        return error;

    TMR2_cfg.mode = TMR_MODE_ONESHOT;
    error = TMR_GetTicks(MXC_TMR2, timeOut, TMR_UNIT_MILLISEC,
            &(TMR2_cfg.cmp_cnt));

    if (error != E_NO_ERROR)
        return error;

    TMR_Init(MXC_TMR2,prescale);
    //configure and start the timer
    TMR_Config(MXC_TMR2, &TMR2_cfg);
    TMR_Enable(MXC_TMR2);

    return error;
}

/******************************************************************************/
/* Send data from Master to Slave
 * Send data from Master asynchronously to Slave
 * Which receives it asynchronously
 */
void BothAsync(){
    int i,error;
    spis_flag = 1;
    spim_flag = 1;

    //set up request buffers
    spim1_req.tx_data = tx_master;
    spim1_req.rx_data = NULL;
    spim1_req.len = BUFF_SIZE;

    spis_req.tx_data = NULL;
    spis_req.rx_data = rx_slave;
    spis_req.len = BUFF_SIZE;

    //start slave async
    if ((error = SPIS_TransAsync(MXC_SPIS, &spis_req)) != E_NO_ERROR) {
      printf("Error Slave Transferring Data Asynchronously: %d\n", error);
      while (1) {}
    }

    // start master async
    if ((error = SPIM_TransAsync(MXC_SPIM1, &spim1_req)) != E_NO_ERROR) {
      printf("Error With MasterTransferring Data Asynchronously: %d\n", error);
      while (1) {}
    }

    // wait for Async call to finish
    while (spim_flag == 1){}

    if (spim_flag != E_NO_ERROR) {
      printf("Error with SPIM callback %d\n", spis_flag);
      while (1){}
    }

    // wait for Async call to finish
    while (spis_flag == 1) {}
    if (spis_flag != E_NO_ERROR) {
      printf("Error with SPIS callback %d\n", spis_flag);
      while (1) {}
    }

    //check if buffers have errors
    if ((error = memcmp(spis_req.rx_data, spim1_req.tx_data, BUFF_SIZE) != 0)) {
    #if VERBOSE
      printf("Start transaction:---------------------------------\n");
      printf("i\tmt\tmr\tst\tsr\n");
      for (i = 0; i < BUFF_SIZE; i++) {
          printf("%d\t%d\t%d\t%d\t%d\n", i, spim1_req.tx_data[i],
                  spim1_req.rx_data[i], spis_req.tx_data[i],
                  spis_req.rx_data[i]);
      }
    #endif
      printf("Master Asynchronous and Slave Asynchronous Failed\n");
      while (1) {}
    }
    printf("Master Asynchronous and Slave Asynchronous Verified\n");

    //clear buffers for next transaction
    FIFO_Clear();


}

/*****************************************************************************/
/* Send data from Slave to Master
 * Send data from Slave asynchronously to Master
 * Which receives it synchronously
 */
void SlaveAsync(){
    int i,error;

    // invert the data read in to rx and store in tx
    for (i = BUFF_SIZE - 1; i >= 0; i--) {
      tx_slave[i] = rx_slave[(BUFF_SIZE - 1) - i];
    }
    // set up request buffers
    spim1_req.tx_data = NULL;
    spim1_req.rx_data = rx_master;
    spim1_req.len = BUFF_SIZE;

    spis_req.tx_data = tx_slave;
    spis_req.rx_data = NULL;
    spis_req.len = BUFF_SIZE;

    spis_flag = 1;

    //start slave async
    if ((error = SPIS_TransAsync(MXC_SPIS, &spis_req)) != E_NO_ERROR) {
        printf("Error with Slave Transferring Data Asynchronously: %d\n", error);
        while (1) {}
    }

    //start master sync
    if ((error = SPIM_Trans(MXC_SPIM1, &spim1_req)) < E_NO_ERROR) {
        printf("Error with Master Transferring Data Synchronously: %d\n", error);
        while (1) {}
    }

    // wait for Async call to finish
    while (spis_flag == 1) {}
    if (spis_flag != E_NO_ERROR) {
        printf("Error with SPIS callback %d\n", spis_flag);
        while (1) {}
    }

    // check buffers for errors
    if ((error = (memcmp(spis_req.tx_data, spim1_req.rx_data, BUFF_SIZE)) != 0)) {
#if VERBOSE
        printf("Start transaction:---------------------------------\n");
        printf("i\tmt\tmr\tst\tsr\n");
        for (i = 0; i < BUFF_SIZE; i++) {
            printf("%d\t%d\t%d\t%d\t%d\n", i, spim1_req.tx_data[i],
                    spim1_req.rx_data[i], spis_req.tx_data[i],
                    spis_req.rx_data[i]);
        }
#endif
        printf("Master Synchronous and Slave Asynchronous Failed\n");
        while (1) {}
    }
    printf("Master Synchronous and Slave Asynchronous Verified\n");

    // clear buffers for next transaction
    FIFO_Clear();
}

/*****************************************************************************/
/* Send data from Master to Slave
 * Send data from Master synchronously to Slave
 * Which receives it asynchronously
 */
void MasterAsync(){
    int i,error;
    for (i = 0; i < BUFF_SIZE; i++) {
        tx_master[i] = rx_master[i];
    }

    // set up request buffers
    spim1_req.tx_data = tx_master;
    spim1_req.rx_data = NULL;
    spim1_req.len = BUFF_SIZE;

    spis_req.tx_data = NULL;
    spis_req.rx_data = rx_slave;
    spis_req.len = BUFF_SIZE;

    spim_flag = 1;

    // start timer to interrupt the slave function to start the master
    SPISTransTimer();

    //start slave sync
    if ((error = SPIS_Trans(MXC_SPIS, &spis_req)) < E_NO_ERROR) {
        printf("Error with Slave Transferring Data Synchronously: %d\n", error);
        while (1) {}
    }

    // check data in buffers for errors
    if ((error = (memcmp(spis_req.tx_data, spim1_req.rx_data, BUFF_SIZE)) != 0)) {
#if VERBOSE
        printf("Start transaction:---------------------------------\n");
        printf("i\tmt\tmr\tst\tsr\n");
        for (i = 0; i < BUFF_SIZE; i++) {
            printf("%d\t%d\t%d\t%d\t%d\n", i, spim1_req.tx_data[i],
                    spim1_req.rx_data[i], spis_req.tx_data[i],
                    spis_req.rx_data[i]);
        }
#endif
        printf("Master Asynchronous and Slave Synchronous Failed\n");
        while (1) {}
    }

    printf("Master Asynchronous and Slave Synchronous Verified\n");
    // clear buffers for next transaction
    FIFO_Clear();
}


int main(void) {
    int error, i;
    spim_cfg_t cfg;
    printf("\n\n******** SPIS Example ********\n");

    printf("   System freq \t: %d Hz\n", SystemCoreClock);
    printf("   SPI freq \t: %d \t %d  Hz\n", CLK_RATE, SYS_SPIM_GetFreq(MXC_SPIM1));
    printf("   SPI bus width : %d bits\n", (0x1 << SPIM_WIDTH_1));
    printf("   Write/Verify   : %d bytes\n\n", BUFF_SIZE);

    // Initialize the data buffers
    for (i = 0; i < BUFF_SIZE; i++) {
        tx_master[i] = i;
    }
    memset(rx_master, 0x0, BUFF_SIZE);
    memset(tx_slave, 0x0, BUFF_SIZE);
    memset(rx_slave, 0x0, BUFF_SIZE);

    // Initialize the SPIM
    cfg.mode = 0;
    cfg.ssel_pol = SPIM_SSEL0_LOW;
    cfg.baud = CLK_RATE;

    SPIM_Shutdown(MXC_SPIM1);

    if ((error = SPIM_Init(MXC_SPIM1, &cfg, NULL)) != E_NO_ERROR) {
        printf("Error initializing SPIM %d\n", error);
        while (1) {}
    } else {
        printf("SPIM Initialized\n");
    }
    // Slave IO Config                            core I/O, quad, fast I/O
    if ((error = SPIS_Init(MXC_SPIS, 0, NULL)) != E_NO_ERROR) {
        printf("Error initializing SPIS %d\n", error);
        while (1) {}
    } else {
        printf("SPIS Initialized\n");
    }
    //Master request
    spim1_req.ssel = 0;   // Slave Select
    spim1_req.deass = 1;
    spim1_req.width = SPIM_WIDTH_1;
    spim1_req.callback = spim_callback;  //Only used for Async
    //Slave request
    spis_req.deass = 1;
    spis_req.width = SPIM_WIDTH_1;
    spis_req.callback = spis_callback;  //Only used for Async

    // Setup the Master interrupt
    NVIC_DisableIRQ(MXC_SPIM_GET_IRQ(1));
    NVIC_ClearPendingIRQ(MXC_SPIM_GET_IRQ(1));
    NVIC_EnableIRQ(MXC_SPIM_GET_IRQ(1));

    // Setup the Slave interrupt
    NVIC_DisableIRQ(MXC_SPIS_GET_IRQ(0));
    NVIC_ClearPendingIRQ(MXC_SPIS_GET_IRQ(0));
    NVIC_EnableIRQ(MXC_SPIS_GET_IRQ(0));

    //Run data transfer between SPIM0 and SPIS
    BothAsync();

    SlaveAsync();

    // MasterAsync();

    // Verify if the buffers are correct
    spis_req.tx_data = tx_slave;
    spis_req.rx_data = rx_slave;
    spim1_req.rx_data = rx_master;
    spim1_req.tx_data = tx_master;
#if VERBOSE

    printf("i\tmt\tmr\tst\tsr\n", spis_req.read_num, spis_req.write_num);
    for (i = 0; i < 512; i++) {
        printf("%d\t%d\t%d\t%d\t%d\n", i, spim1_req.tx_data[i],
                spim1_req.rx_data[i], spis_req.tx_data[i], spis_req.rx_data[i]);
    }
#endif
    if ((memcmp(spis_req.rx_data, spim1_req.tx_data, BUFF_SIZE) != 0)
            || (memcmp(spis_req.tx_data, spim1_req.rx_data, BUFF_SIZE) != 0)) {
        printf("Data is NOT Correct\n");
    } else {
        printf("Data is Verified\n");
    }

    while (1) {}
}