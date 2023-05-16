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
 * @brief   read and write sdhc
 * @details This example uses the sdhc to read and write to an sdhc card
 * You must connect a sd card reader to pins 0.31 and 1.0 to 1.6 according to the pin layout of the sd card reader.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "mxc_config.h"
#include "mxc_sys.h"
#include "sdhc_regs.h"
#include "led.h"
#include "tmr_utils.h"
#include "gpio.h"
#include "sdhc.h"
#include "ff.h"

/***** Definitions *****/

/***** Globals *****/
    FATFS *fs;
    FIL file;
    FRESULT err;
    FILINFO fno;
    TCHAR volume = '0';
    TCHAR *FF_ERRORS[20];
    TCHAR message[256];
    TCHAR directory[256];
    TCHAR cwd[256];
    TCHAR filename[256];
    TCHAR volume_label[24];
    DWORD clusters_free = 0, sectors_free = 0, sectors_total = 0, volume_sn = 0;
    UINT bytes_written = 0, bytes_read = 0, mounted = 0;
    BYTE work[4096];
    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!"; 

/***** FUNCTIONS *****/

int mount() {
    //open SD Card
    fs = malloc(sizeof(FATFS));
    if((err = f_mount(fs, "", 1)) != FR_OK) {
        printf("Error opening SD Card: %d\n", FF_ERRORS[err]);
    } else {
        printf("SD Card Mounted!\n");
        mounted = 1;
    }
    f_getcwd(cwd, sizeof(cwd));
    return err;
}

int umount() {
    if((err = f_mount(NULL, "", 0)) != FR_OK){
        printf("Error unmounting volume: %d\n", FF_ERRORS[err]);
    } else {
        printf("SD Card Unmounted!\n");
        mounted = 0;
    }
    free(fs);
    return err;
}

int formatSDHC() {
    printf("\n\n*****THE DRIVE WILL BE FORMATTED IN 5 SECONDS*****\n\n");
    TMR_Delay(MXC_TMR0, MSEC(5000));
    printf("FORMATTING DRIVE\n");
    if((err = f_mkfs("", FM_ANY, 0, work, sizeof(work))) != FR_OK) {
        printf("Error Formatting SD Card: %d\n", FF_ERRORS[err]);
    } else {
        printf("Drive Formatted\n");
    }
    mount();
    if((err = f_setlabel("MAXIM")) != FR_OK){
        printf("Error setting drive label: %d\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
    }
    umount();
    return err;
} 

int getSize() {
    if(!mounted) {
        mount();
    }

    //Find Size of SD Card
    if((err = f_getfree(&volume,&clusters_free, &fs)) != FR_OK) {
        printf("Error finding free size of card: %d\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
    }

    sectors_total = (fs->n_fatent -2) * fs->csize;
    sectors_free = clusters_free * fs->csize;

    printf("Disk Size: %u bytes\n", sectors_total/2);
    printf("Avaliable: %u bytes\n", sectors_free/2);
    return err;
}

int ls() {
    if(!mounted) {
        mount();
    }
    DIR dir;
    printf("Listing Contents of %s - \n", cwd);
    if((err = f_opendir(&dir, cwd)) == FR_OK) {
        while(1){   
            err = f_readdir(&dir, &fno);
            if(err != FR_OK || fno.fname[0] == 0) break;
            printf("%s/%s", cwd, fno.fname);
            if(fno.fattrib & AM_DIR) {
                printf("/");
            }
            printf("\n");
        }
        f_closedir(&dir);
    } else {
        printf("Error Opening Directory!\n");
        return err;
    }
    printf("\nFinished Listing Contents\n");
    return err;
}

int createFile() {
    if(!mounted) {
        mount();
    }

    int length = 128;

    printf("Enter the Name of the Text File: \n");
    scanf("%s", filename);
    printf("Enter the length of the File: (256 max)\n");
    scanf("%d", &length);
    printf("Creating File %s with length %d\n",filename, length);

    if((err = f_open(&file, filename, FA_CREATE_ALWAYS|FA_WRITE)) != FR_OK){
        printf("Error opening file: %d\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0); return err;
    }
    printf("File opened!\n");

    for(int i = 0 ; i < length; i++) {
        message[i] =  charset[rand()%(sizeof(charset)-1)];
    }

    if((err = f_write(&file, &message, length, &bytes_written)) != FR_OK){
        printf("Error writing file: %d\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0); return err;
    }
    printf("%d bytes written to file!\n", bytes_written);
        
    if((err = f_close(&file)) != FR_OK){
        printf("Error closing file: %d\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0); return err;
    }   
    printf("File Closed!\n");
    return err;
}

int appendFile() {
    if(!mounted) {
        mount();
    }
    int length = 0;

    printf("Type name of file to append: \n");
    scanf("%s", filename);
    printf("Type length of random data to append: \n");
    scanf("%d", &length);

    const TCHAR *name = filename;
    if((err = f_stat(name, &fno)) == FR_NO_FILE) {
        printf("File %s doesn't exist!\n", filename);
        return err;
    }
    if((err = f_open(&file, filename, FA_OPEN_APPEND|FA_WRITE)) != FR_OK) {
        printf("Error opening file %d\n", FF_ERRORS[err]);
        return err;
    }
    printf("File opened!\n");

    for(int i = 0 ; i < length; i++) {
        message[i] =  charset[rand()%(sizeof(charset)-1)];
    }

    if((err = f_write(&file, &message, length, &bytes_written)) != FR_OK){
        printf("Error writing file: %d\n", FF_ERRORS[err]);
        return err;
    }
    printf("%d bytes written to file!\n", bytes_written);
        
    if((err = f_close(&file)) != FR_OK){
        printf("Error closing file: %d\n", FF_ERRORS[err]);
        return err;
    }   
    printf("File Closed!\n");
    return err;
}

int mkdir() {
    if(!mounted) {
        mount();
    }
    printf("Enter Directory Name: \n");
    scanf("%s", directory);

    const TCHAR *name = directory;
    err = f_stat(name, &fno);
    if(err == FR_NO_FILE) {
        printf("Creating Directory...\n");
        if((err = f_mkdir(name)) != FR_OK){
            printf("Error creating directory: %d\n", FF_ERRORS[err]);
            f_mount(NULL, "", 0); while(1){}
        } else {
            printf("Directory %s created!\n", directory);
        }
    } else {
        printf("Directory Already Exists!\n");
    }
    return err;
}

int cd() {
    if(!mounted) {
        mount();
    }
    printf("Directory to Change into: \n");
    scanf("%s", directory);

    const TCHAR *name = directory;
    if((err = f_stat(name, &fno)) == FR_NO_FILE) {
        printf("Directory Doesn't Exist (Did you mean mkdir?)\n");
        return err;
    }

    if((err = f_chdir(name)) != FR_OK){
        printf("Error in chdir: %d\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0); return err;
    }
    printf("Chaged to %s\n", directory);
    f_getcwd(cwd, sizeof(cwd));
    return err;
}

int delete() {
    if(!mounted) {
        mount();
    }
    printf("File or Directory to delete (always recursive!)\n");
    scanf("%s", filename);

    const TCHAR *name = filename;
    if((err = f_stat(name, &fno)) == FR_NO_FILE) {
        printf("File or Directory Doesn't Exist\n");
        return err;
    }

    if((err = f_unlink(filename)) != FR_OK) {
        printf("Error Deleting File\n");
        return err;
    }
    printf("Deleted file %s\n", filename);
    return err;

}

int example() {
    printf("\n\n*****THE DRIVE WILL BE FORMATTED IN 5 SECONDS*****\n\n");
    TMR_Delay(MXC_TMR0, MSEC(5000));
    printf("FORMATTING DRIVE\n");
    if((err = f_mkfs("", FM_ANY, 0, work, sizeof(work))) != FR_OK) {
        printf("Error Formatting SD Card: %d\n", FF_ERRORS[err]);
        while(1){}
    }

    //open SD Card
    fs = malloc(sizeof(FATFS));
    if((err = f_mount(fs, "", 1)) != FR_OK) {
        printf("Error opening SD Card: %d\n", FF_ERRORS[err]);
        while(1){}
    }
    printf("SD Card Opened!\n");

    if((err = f_setlabel("MAXIM")) != FR_OK){
        printf("Error setting drive label: %d\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0); while(1){}
    }

    //Find Size of SD Card
    if((err = f_getfree(&volume,&clusters_free, &fs)) != FR_OK) {
        printf("Error finding free size of card: %d\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0); while(1){}
    }

    sectors_total = (fs->n_fatent -2) * fs->csize;
    sectors_free = clusters_free * fs->csize;

    printf("Disk Size: %u bytes\n", sectors_total/2);
    printf("Avaliable: %u bytes\n", sectors_free/2);

    if((err = f_getlabel(&volume,volume_label,&volume_sn)) != FR_OK){
        printf("Error reading drive label: %d\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0); while(1){}
    }

    //write size to SD Card
    if((err = f_open(&file, "0:HelloWorld.txt", FA_CREATE_ALWAYS|FA_WRITE)) != FR_OK){
        printf("Error opening file: %d\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0); while(1){}
    }
    printf("File opened!\n");

    if((err = f_write(&file, &message, sizeof(message), &bytes_written)) != FR_OK){
        printf("Error writing file: %d\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0); while(1){}
    }
    printf("%d bytes written to file!\n", bytes_written);
        
    if((err = f_close(&file)) != FR_OK){
        printf("Error closing file: %d\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0); while(1){}
    }   
    printf("File Closed!\n");

    if((err = f_chmod("HelloWorld.txt", 0, AM_RDO | AM_ARC | AM_SYS | AM_HID)) != FR_OK){
        printf("Error in chmod: %d\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0); while(1){}
    }   

    err = f_stat("MaximSDHC", &fno);
    if(err == FR_NO_FILE) {
        printf("Creating Directory...\n");
        if((err = f_mkdir("MaximSDHC")) != FR_OK){
            printf("Error creating directory: %d\n", FF_ERRORS[err]);
            f_mount(NULL, "", 0); while(1){}
        }
    }

    printf("Renaming File...\n");
    if((err = f_rename("0:HelloWorld.txt", "0:MaximSDHC/HelloMaxim.txt")) != FR_OK){
        printf("Error moving file: %d\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0); while(1){}
    }

    if((err = f_chdir("/MaximSDHC")) != FR_OK){
        printf("Error in chdir: %d\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0); while(1){}
    }

    printf("Attempting to read back file...\n");
    if((err = f_open(&file, "HelloMaxim.txt", FA_READ)) != FR_OK){
        printf("Error opening file: %d\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0); while(1){}
    }

    if((err = f_read(&file, &message, bytes_written, &bytes_read)) != FR_OK){
        printf("Error reading file: %d\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0); while(1){}
    }

    printf("Read Back %d bytes\n", bytes_read);
    printf("Message: ");
    printf(message);
    printf("\n");
        
    if((err = f_close(&file)) != FR_OK){
        printf("Error closing file: %d\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0); while(1){}
    }   
    printf("File Closed!\n");

    //unmount SD Card
    //f_mount(fs, "", 0);
    if((err = f_mount(NULL, "", 0)) != FR_OK){
        printf("Error unmounting volume: %d\n", FF_ERRORS[err]);
        while(1){}
    }  
     
    free(fs);
    return 0;
}

/******************************************************************************/
int main(void) {
    //srand(12347439);
    int run = 1;
    printf("\n\n*****SDHC Example Application*****\n");
    while(run) {
        f_getcwd(cwd, sizeof(cwd));

        printf("\nChoose one of the following options: \n");
        printf("0. Find the Size of the SD Card and Free Space\n");
        printf("1. Format the Card\n");
        printf("2. Manually Mount Card\n");        
        printf("3. List Contents of Current Directory\n");
        printf("4. Create a Directory\n");
        printf("5. Move into a Directory (cd)\n");
        printf("6. Create a File of Random Data\n");
        printf("7. Add Random Data to an Existing File\n");
        printf("8. Delete a File\n");        
        printf("9. Format Card and Run Exmaple of FatFS Operations\n");
        printf("10. Unmount Card and Quit\n");
        printf("%s>>",cwd);

        int input = -1;
        scanf("%d", &input);
        printf("%d\n", input);

        err = 0;

        switch(input) {
            case 0: getSize();
                break;
            case 1: formatSDHC();
                break;
            case 3: ls();
                break;
            case 6: createFile();
                break;
            case 7: appendFile();
                break;
            case 4: mkdir();
                break;
            case 5: cd();
                break;
            case 9: example();
                break;
            case 10: umount(); run = 0;
                break;
            case 2: mount(); 
                break;
            case 8: delete();
                break;
            default: printf("Invalid Selection %d!\n", input); err = -1;
                break;
        }
        if(err >= 0 && err <= 19) {
            printf("Function Returned with code: %d\n", FF_ERRORS[err]);
        }
        else {
            printf("Function Returned with code: %d\n", err);
        }
        TMR_Delay(MXC_TMR0, MSEC(500));
    }
    printf("Exmaple quit! Try and read the card!\n");
    while(1){}
}

FF_ERRORS[0] = "FR_OK";
FF_ERRORS[1] = "FR_DISK_ERR";
FF_ERRORS[2] = "FR_INT_ERR";
FF_ERRORS[3] = "FR_NOT_READY";
FF_ERRORS[4] = "FR_NO_FILE";
FF_ERRORS[5] = "FR_NO_PATH";
FF_ERRORS[6] = "FR_INVLAID_NAME";
FF_ERRORS[7] = "FR_DENIED";
FF_ERRORS[8] = "FR_EXIST";
FF_ERRORS[9] = "FR_INVALID_OBJECT";
FF_ERRORS[10] = "FR_WRITE_PROTECTED";
FF_ERRORS[11] = "FR_INVALID_DRIVE";
FF_ERRORS[12] = "FR_NOT_ENABLED";
FF_ERRORS[13] = "FR_NO_FILESYSTEM";
FF_ERRORS[14] = "FR_MKFS_ABORTED";
FF_ERRORS[15] = "FR_TIMEOUT";
FF_ERRORS[16] = "FR_LOCKED";
FF_ERRORS[17] = "FR_NOT_ENOUGH_CORE";
FF_ERRORS[18] = "FR_TOO_MANY_OPEN_FILES";
FF_ERRORS[19] = "FR_INVALID_PARAMETER";