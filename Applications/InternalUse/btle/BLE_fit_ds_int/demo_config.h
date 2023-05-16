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
#define INTERVAL_TIME_OST_BASREAD   MSEC(100)   // (ms) delay to read battery level
#define OST_TIMER           MXC_TMR4      // Can be MXC_TMR0 through MXC_TMR5
#define OST_TIMER_IDX       MXC_TMR_GET_IDX(OST_TIMER)

//#define USE_96M_CLOCK 

/* Definitions for demo LED assignment */

/// @brief Specifies the LED used to indicate when the part is in deepsleep mode.
#define DEEPSLEEP_LED       0

/// @brief Specifies the LED used to indicate when the part is in background mode.
/// @note May be shared with DEEPSLEEP_LED to indicate low power states generally.
#define BACKGROUND_LED       0

/// @brief Specifies the LED used to indicate when a user event is pending.
#define USER_GPIO_LED       1

/* Timing definitions */

/// @brief Specifies the time slice that the scheduler needs
/// to be idle for user event handling to occur.
//#define EVENT_TIME_REQUIREMENT (100) // 100us required to handle event.
#undef EVENT_TIME_REQUIREMENT             // Don't check for time to handle event.

/// @brief Specifies the time the user event handler
/// is to wait to simulate performing actual work.
/// This must be shorter than EVENT_TIME_REQUIREMENT for
/// the BLE operation to not be disrupted.
//#define EVENT_TIME_SIM_DELAY (50) // 50us delay to simulate event work.
//#define EVENT_TIME_SIM_DELAY (-1000) // 1000 instruction (min) delay to simulate event work.
#undef EVENT_TIME_SIM_DELAY               // No simulated delay

/* Pin assignments for input event */

/// @brief Specifies the GPIO port used to signal a user event.
#define USER_PIN_PORT PORT_1

/// @brief Specifies the GPIO pin used to signal a user event.
#define USER_PIN_PIN PIN_6

/// @brief Specifies the GPIO pin pullup configuration for
/// the pin used to signal user events.
#define USER_PIN_PAD GPIO_PAD_PULL_UP

/// @brief Enable handling of user events indicated using GPIO interrupt in main loop.
#define MAIN_LOOP_HANDLER

/// @brief Enable handling of user events indicated using GPIO interrupt in sleep loop.
#define SLEEP_LOOP_HANDLER

/// @brief Force wsf dispatch.
//#define USE_FORCE_WSF_DISPATCH

/* Demo selection */

/// @brief Specifies that the LED be toggled each time a user event is handled.
#define USER_GPIO_TOGGLE_DEMO

/// @brief Specifies that the LED be turned on when an led interrupt occurs,
/// and off when the event is handled.
#undef USER_GPIO_TIMING_DEMO
