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

#ifndef USER_EVENT_H
#define USER_EVENT_H

// Include for MSEC definition.
#include "tmr_utils.h"

/* Application declarations */

/// @brief Specifies the STARTUP DELAY used to avoid bricking the device.
#define STARTUP_DELAY       SEC(5)

/// @brief Specifies the LED used to indicate when the device is asleep.
#define SLEEP_GPIO_LED      0

/* Definitions for demo LED assignment */

/// @brief Specifies the LED used to indicate when a user event is pending.
#define USER_GPIO_LED       1

/* Timing definitions */

/// @brief How long to spend handling event
#define USER_EVENT_DELAY    USEC(500)

/* Pin assignments for input event */

/// @brief Specifies the GPIO port used to signal a user event.
#define USER_PIN_PORT PORT_1

/// @brief Specifies the GPIO pin used to signal a user event.
#define USER_PIN_PIN PIN_6

/// @brief Specifies the GPIO pin pullup configuration for
/// the pin used to signal user events.
#define USER_PIN_PAD GPIO_PAD_PULL_UP

/* Pin configuration indices */
#define USER_PIN_IN         0
#define USER_PIN_OUT        1
#define USER_PIN_COUNT      2

/**************************************************************************************************
  Globals
**************************************************************************************************/

#ifndef MAIN_C
extern const gpio_cfg_t user_pin[USER_PIN_COUNT];
extern gpio_cfg_t user_interrupt_status;
#endif /* MAIN_C */

/**************************************************************************************************
  Functions
**************************************************************************************************/

void HandleUserEventInterrupt(void *cbdata);
void RegisterUserEvent(void);
void SetUserLED(int bEnabled);

#endif /* USER_EVENT_H */
