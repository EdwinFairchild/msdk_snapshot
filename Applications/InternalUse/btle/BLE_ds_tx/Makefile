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
# $Id: Makefile 47812 2019-10-17 12:37:39Z kevin.gillespie $
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

# This is the path to the CMSIS root directory
ifeq "$(MAXIM_PATH)" ""
LIBS_DIR=../../../../Libraries
NDALIBS_DIR=../../../../NDALibraries
else
LIBS_DIR=/$(subst \,/,$(subst :,,$(MAXIM_PATH))/Firmware/$(TARGET_UC)/Libraries)
endif
CMSIS_ROOT=$(LIBS_DIR)/CMSIS

# Source files for this test (add path to VPATH below)
SRCS  = main.c
SRCS += stack_setup.c
SRCS += wakeup.c

# Where to find source files for this test
VPATH  = .

# Where to find header files for this test
IPATH  = .

################################################################################
# Project build configuration.

# Enable assertion checking for development
PROJ_CFLAGS+=-DWSF_ASSERT_ENABLE=TRUE
# PROJ_CFLAGS+=-DLL_TRACE_ENABLED=TRUE
PROJ_CFLAGS+=-DLL_DBG_PIN_ENABLED=TRUE

#--------------------
# Stack CFLAGS configuration
PROJ_CFLAGS+=-DINIT_BROADCASTER
PROJ_CFLAGS+=-DINIT_OBSERVER
PROJ_CFLAGS+=-DINIT_CENTRAL
PROJ_CFLAGS+=-DINIT_PERIPHERAL
PROJ_CFLAGS+=-DINIT_ENCRYPTED

LL_ENABLE_TESTER=1

# Use this variable to override default compilier optimization.
#MXC_OPTIMIZE_CFLAGS=-Os
#MXC_OPTIMIZE_CFLAGS=-O0 -fomit-frame-pointer

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
CORDIO_DIR=$(NDALIBS_DIR)/BTLE
include ${CORDIO_DIR}/btle.mk

################################################################################
# Include the rules for building for this target. All other makefiles should be
# included before this one.
include $(CMSIS_ROOT)/Device/Maxim/$(TARGET_UC)/Source/$(COMPILER)/$(TARGET_LC).mk
