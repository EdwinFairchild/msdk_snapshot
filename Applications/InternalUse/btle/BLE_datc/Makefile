################################################################################
# Copyright (C) 2014 Maxim Integrated Products, Inc., All Rights Reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
# OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#
# Except as contained in this notice, the name of Maxim Integrated
# Products, Inc. shall not be used except as stated in the Maxim Integrated
# Products, Inc. Branding Policy.
#
# The mere transfer of this software does not imply any licenses
# of trade secrets, proprietary technology, copyrights, patents,
# trademarks, maskwork rights, or any other form of intellectual
# property whatsoever. Maxim Integrated Products, Inc. retains all
# ownership rights.
#
# $Id: Makefile 48522 2019-11-07 21:35:49Z kevin.gillespie $
#
################################################################################

# This is the name of the build output file
ifeq "$(PROJECT)" ""
PROJECT=max32665
endif

# Specify the target processor
ifeq "$(TARGET)" ""
TARGET=MAX32665
endif

# Select 'GCC' or 'IAR' compiler
COMPILER=GCC

# Specify the board used
ifeq "$(BOARD)" ""
BOARD=EvKit_V1
endif

# Create Target name variables
TARGET_UC:=$(shell echo $(TARGET) | tr a-z A-Z)
TARGET_LC:=$(shell echo $(TARGET) | tr A-Z a-z)

# Last octet address modifier
ifneq "$(BLE_ADDR_LSB)" ""
PROJ_CFLAGS+=-DBLE_ADDR_LSB=$(BLE_ADDR_LSB)
endif

## Subsitute WSF critical section entry for default MAXUSB
#PROJ_CFLAGS+=-DMAXUSB_ENTER_CRITICAL=WsfCsEnter
#PROJ_CFLAGS+=-DMAXUSB_EXIT_CRITICAL=WsfCsExit

# This is the path to the CMSIS root directory
ifeq "$(MAXIM_PATH)" ""
LIBS_DIR=../../../../Libraries
NDALIBS_DIR=../../../../NDALibraries
else
LIBS_DIR=/$(subst \,/,$(subst :,,$(MAXIM_PATH))/Firmware/$(TARGET_UC)/Libraries)
NDALIBS_DIR=/$(subst \,/,$(subst :,,$(MAXIM_PATH))/Firmware/$(TARGET_UC)/NDALibraries)
endif
CMSIS_ROOT=$(LIBS_DIR)/CMSIS

# Source files for this test (add path to VPATH below)
SRCS  = main.c
SRCS += stack_datc.c
SRCS += datc_main.c

# Where to find source files for this test
VPATH  = .

# Where to find header files for this test
IPATH  = .

################################################################################
# Project build configuration.

#-------------------------------------------------------------------------------
# Configuration passed via environment vars.

ifdef BTLE_APP_USE_LEGACY_API
ifeq "$(BTLE_APP_USE_LEGACY_API)" ""
PROJ_CFLAGS+=-DBTLE_APP_USE_LEGACY_API=TRUE
else
ifeq "$(BTLE_APP_USE_LEGACY_API)" "0"
else
ifeq "$(BTLE_APP_USE_LEGACY_API)" "FALSE"
else
PROJ_CFLAGS+=-DBTLE_APP_USE_LEGACY_API=TRUE
endif
endif
endif
endif

ifdef CONSOLE_UART
ifneq "$(CONSOLE_UART)" ""
PROJ_CFLAGS+=-DCONSOLE_UART=$(CONSOLE_UART)
endif
endif

#-------------------------------------------------------------------------------
# Configuration specified here.

# Enable assertion checking for development
PROJ_CFLAGS+=-DMXC_ASSERT_ENABLE=TRUE
PROJ_CFLAGS+=-DWSF_TRACE_ENABLED=TRUE
PROJ_CFLAGS+=-DWSF_ASSERT_ENABLED=TRUE
# PROJ_CFLAGS+=-DLL_TRACE_ENABLED=TRUE
# PROJ_CFLAGS+=-DLL_DBG_PIN_ENABLED=TRUE

# ENABLE_SDMA?=#1

# PROJ_CFLAGS+=-DINIT_BROADCASTER
PROJ_CFLAGS+=-DINIT_OBSERVER
PROJ_CFLAGS+=-DINIT_CENTRAL
# PROJ_CFLAGS+=-DINIT_PERIPHERAL
PROJ_CFLAGS+=-DINIT_ENCRYPTED

ENABLE_LL_TESTER?=#1

#MXC_OPTIMIZE_CFLAGS=-Os

# Point this variable to a linker file to override the default file
#LINKERFILE=$(CMSIS_ROOT)/Device/Maxim/$(TARGET_UC)/Source/GCC/$(TARGET_LC).ld

################################################################################
# Include external library makefiles here
DEBUG=1

# Include the BSP
BOARD_DIR=$(LIBS_DIR)/Boards/$(BOARD)
include $(BOARD_DIR)/board.mk

# Include the Peripheral Driver Library
PERIPH_DRIVER_DIR=$(LIBS_DIR)/$(TARGET)PeriphDriver
include ${PERIPH_DRIVER_DIR}/periphdriver.mk

# Include Cordio BTLE Library
CORDIO_DIR=../cordio-mbed
include ${CORDIO_DIR}/btle.mk

# Include MAXUSB library
MAXUSB_DIR=$(LIBS_DIR)/MAXUSB
include $(MAXUSB_DIR)/maxusb.mk

################################################################################
# Include the rules for building for this target. All other makefiles should be
# included before this one.
include $(CMSIS_ROOT)/Device/Maxim/$(TARGET_UC)/Source/$(COMPILER)/$(TARGET_LC).mk
