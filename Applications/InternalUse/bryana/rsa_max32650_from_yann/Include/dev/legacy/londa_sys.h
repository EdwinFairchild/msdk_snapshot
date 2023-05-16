/***************************************************
******  Londa system .h file
***************************************************/
#ifndef _LONDA_SYS_H_
#define _LONDA_SYS_H_

#include "bsp_types.h"
#include "sim_tf.h"
#include "arm_defines.h"
#include "tst_msg.h"
#include "bsp_utils.h"
#include "sim_kypd_tf.h"

#include "armver.h"
#include "za9_int.h"
#include "za9_gpio.h"
#include "za9_dma.h"
#include "DmaTests.h"
#include "za9_spi.h"
#include "za9_uart.h"
#include "za9_kypd.h"
#include "kypd_isr.h"
#include "kypd_lib.h"
#define  REG32(ADDR)    (*(volatile UINT32 *)ADDR)

void pass();
void fail();

#endif
