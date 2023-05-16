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
PROJECT=sdma

# Specify the target processor
TARGET=MAX32665

# Create Target name variables
TARGET_UC:=$(shell echo $(TARGET) | tr a-z A-Z)
TARGET_LC:=$(shell echo $(TARGET) | tr A-Z a-z)

COMPILER=RCS

# This is the path to the CMSIS root directory
ifeq "$(MAXIM_PATH)" ""
LIBS_DIR=$(realpath ../../../../../Libraries)
NDALIBS_DIR=$(realpath ../../../../../NDALibraries)
else
LIBS_DIR=/$(subst \,/,$(subst :,,$(MAXIM_PATH))/Firmware/$(TARGET_UC)/Libraries)
endif
CMSIS_ROOT=$(LIBS_DIR)/CMSIS

# Source files for this test (add path to VPATH below)
SRCS  = main.c
SRCS += chci_drv.c
SRCS += stack_setup.c

# Where to find source files for this test
VPATH  = .

# Where to find header files for this test
IPATH  = .
IPATH += ..

# Enable assertion checking for development
# PROJ_CFLAGS+=-DMXC_ASSERT_ENABLE=TRUE
# PROJ_CFLAGS+=-DWSF_TRACE_ENABLED=TRUE
# PROJ_CFLAGS+=-DLL_TRACE_ENABLED=TRUE
# PROJ_CFLAGS+=-DLL_DBG_PIN_ENABLED=TRUE
# PROJ_CFLAGS+=-DSMP_EXTRA_TRACE=TRUE

PROJ_CFLAGS+=-DINIT_BROADCASTER
PROJ_CFLAGS+=-DINIT_OBSERVER
PROJ_CFLAGS+=-DINIT_CENTRAL
PROJ_CFLAGS+=-DINIT_PERIPHERAL
PROJ_CFLAGS+=-DINIT_ENCRYPTED
PROJ_CFLAGS+=-DLL_ENABLE_TESTER=TRUE
ENABLE_LL_TESTER=1

PROJ_CFLAGS+=-DCONSOLE_UART=1
PROJ_AFLAGS+=-DCONSOLE_UART=1

# 256kB
# SRAM_ADDR    = 0x20040440
# SRAM_LEN    = 0x3fbc0
# OTP_ADDR    = 0x20000000
# SHARED_ADDR = 0x20000040
# SDMA_ADDR   = 0x20000440
SDMA_LEN    = 0x40000

################################################################################
# Include external library makefiles here

# Include the Wireless Library
CORDIO_DIR=$(NDALIBS_DIR)/BTLE
include ${CORDIO_DIR}/btle.mk

################################################################################
# Include the rules for building for this target. All other makefiles should be
# included before this one.
include $(CMSIS_ROOT)/Device/Maxim/$(TARGET_UC)/Source/$(COMPILER)/$(TARGET_LC).mk
	
