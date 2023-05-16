/*******************************************************************************
 * Copyright (C) 2021 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date$
 * $Revision$
 *
 ******************************************************************************/

/* **************************************** */
/* Application definitions */

#define INTERVAL_TIME_OST_STARTUP   SEC(5)      // (s) before advertising
#define OST_TIMER           MXC_TMR4      // Can be MXC_TMR0 through MXC_TMR5
#define OST_TIMER_IDX       MXC_TMR_GET_IDX(OST_TIMER)

//#define USE_96M_CLOCK 

/* Definitions for demo LED assignment */

/// @brief Specifies the LED used to indicate when the part is in deepsleep mode.
#define DEEPSLEEP_LED       0

/// @brief Specifies the LED used to indicate when the part is in background mode.
/// @note May be shared with DEEPSLEEP_LED to indicate low power states generally.
#define BACKGROUND_LED      1
