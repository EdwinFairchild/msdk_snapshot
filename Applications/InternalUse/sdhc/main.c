/*******************************************************************************
 * Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * @brief   read and write sdhc
 * @details This example uses the sdhc to read and write to an sdhc card
 * You must connect a sd card reader to pins 0.31 and 1.0 to 1.6 according to the pin layout of the sd card reader.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include "mxc_config.h"
#include "mxc_sys.h"
#include "sdhc_regs.h"
#include "led.h"
#include "tmr_utils.h"
#include "gpio.h"
#include "sdhc.h"

/***** Definitions *****/
#define BLOCK_COUNT 1 // number of data blocks to transfer

/***** Globals *****/

mxc_sdhc_regs_t* sd = MXC_SDHC;
uint64_t array[64][BLOCK_COUNT];    //Array to hold data read and written to card

volatile int sdhc_flag= 1;
/***** Functions *****/

/******************************************************************************/
//sdhc callback from async functions
void sdhc_cb(int error)
{
    sdhc_flag = error;
}

/******************************************************************************/
// When interrupt fires
void SDHC_IRQHandler(void)
{
    SDHC_Lib_Async_Handler();
}

/******************************************************************************/
//Set array to known value
void set_data(uint32_t data){
    volatile int i,k;

    for(i=0; i<64;i++){

        for(k=0; k<BLOCK_COUNT;k++){
            array [i][k]= data;
        }
    }
}

/******************************************************************************/
//check array vs expected value which is data
int check_data(uint32_t data){
    volatile int i,k;

    for(i=0; i<64;i++){
        
        for(k=0; k<BLOCK_COUNT;k++){
           if(array [i][k] != data){
                return -3;
            }
        }
    }
    return 0;
}

/******************************************************************************/
//Sends write async to card and then does a read async to see if the right data was written
int async_transactions(){
    // initialize array
    set_data(0xAAAAAAAA);

    sdhc_flag = 1;
    //Write data from array to the card
    SDHC_Lib_WriteAsync(BLOCK_COUNT,array,0x00000000,sdhc_cb);
    while(sdhc_flag ==1);
    if(sdhc_flag == E_NO_ERROR){
         printf("Card Written To.\n");

    }
    else{
        printf("Write Failed\n");
        return sdhc_flag;
    }

    // initialize array
    set_data(0);

    sdhc_flag = 1;
    //Read data from card and store in array
    SDHC_Lib_Read_Async(BLOCK_COUNT,array,0x00000000,sdhc_cb);
    while(sdhc_flag ==1);
    if(sdhc_flag == E_NO_ERROR){
         printf("Card Read From.\n");

    }
    else{
        printf("Read Failed\n");
        return sdhc_flag;
    }
    return check_data(0xAAAAAAAA);

}

/******************************************************************************/
//erases card with blocking functions
int erase(){
    volatile int error;
    set_data(0);
    //Write data from array to the card
    if((error = SDHC_Lib_Write(BLOCK_COUNT,array,0x00000000)) == E_NO_ERROR){
         printf("Card Written To.\n");

    }
    else{
        printf("Write Failed\n");
        return error;
    }
    set_data(1);
    //Read data from card and store in array
    if((error = SDHC_Lib_Read(BLOCK_COUNT,array,0x00000000)) == E_NO_ERROR){
         printf("Card Read From.\n");

    }
    else{
        printf("Read Failed\n");
        return error;
    }
    return check_data(0);
}

/******************************************************************************/
//Write and then read what was written to card using blocking methods.
int blocking_transactions(){
    int error;
    
    // initialize array
    set_data(0xAAAAAAAA);
    //Write data from array to the card
    if((error =SDHC_Lib_Write(BLOCK_COUNT,array,0x00002000)) == E_NO_ERROR){
        printf("Card Written To.\n");

    }
    else{
       printf("Write Failed\n");
       return error;
    }

    // initialize array
    set_data(1);
    
    //Read data from card and store in array
    if((error = SDHC_Lib_Read(BLOCK_COUNT,array,0x00000001))== E_NO_ERROR){
         printf("Card Read From.\n");

    }
    else{
        printf("Read Failed\n");
        return error;
    }
    return check_data(0xAAAAAAAA);
    
}

/******************************************************************************/
int main(void)
{
    sdhc_cfg_t cfg;
    int error;
    int test=0;
    printf("Hello World SDHC1!\n");

    // Set up Interupt
    NVIC_EnableIRQ(SDHC_IRQn);
    sd->int_stat = 0;
    sd->int_en = 0;
    sd->int_signal = 0;

    // Initialize SDHC peripheral
    cfg.bus_voltage = SDHC_Bus_Voltage_1_8;
    cfg.block_gap = 0;
    cfg.clock_frequency_divider = 0;
    SDHC_Init(&cfg);

    // wait for card to be inserted
    while(test == 0){
        test = SDHC_Card_Inserted();
    }
    printf("Card detected.\n");
    // set up card to get it ready for a transaction
    SDHC_Lib_Setup();
    printf("Card Initialized.\n");

#if 1  //Blocking Transaction example
    TMR_Delay(MXC_TMR0, USEC(100));
    if((error =blocking_transactions()) != 0) {
        printf("blocking error %d\n",error);
    }
    else{
        printf("Passed blocking\n");
    }
#endif
#if 0 //erase the card after blocking
    TMR_Delay(MXC_TMR0, USEC(100));
    if((error = erase()) != 0) {
        printf("Erase failed %d\n",error);
    }
    else{
        printf("Passed erase\n");
    }
#endif
#if 0 //Async Transaction example
    TMR_Delay(MXC_TMR0, USEC(100));
    if((error = async_transactions()) != 0) {
        printf("async error %d\n",error);
    }
    else{
        printf("Passed async\n");
    }
#endif
    while(1);
}
