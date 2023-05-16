#ifndef __SCT_CONFIG_H
#define __SCT_CONFIG_H

; /*-Memory Regions-*/

#define IFLASH_start__        0x10000000
#define IFLASH_size__         0x00100000
#define CODE_size__           0x000FDFFF
#define PAL_NVM_DB_START__       0x100FE000
#define PAL_NVM_SIZE          0x00002000
#define IRAM_start__          0x20000000
#define IRAM_size__           0x00070000
#define PAL_NVM_DB_END__      PAL_NVM_DB_START__ + PAL_NVM_SIZE

#endif