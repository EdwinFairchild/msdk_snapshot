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
# $Id: Makefile 38987 2018-11-06 23:24:10Z zach.metzinger $
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
BOARD=BLEDemo_V1
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
LIBS_DIR=../../../Libraries
else
LIBS_DIR=/$(subst \,/,$(subst :,,$(MAXIM_PATH))/Firmware/$(TARGET_UC)/Libraries)
endif
CMSIS_ROOT=$(LIBS_DIR)/CMSIS

# Source files for this test (add path to VPATH below)
SRCS  = main.c
SRCS += stack_dats.c
SRCS += dats_main.c
SRCS += process_msg.c
SRCS += chademo_crypto.c
SRCS += chademo_usb.c
SRCS += hardfault.c
SRCS += $(rootDir)/fcl/src/aes.c
SRCS += $(rootDir)/fcl/src/bignum.c
SRCS += $(rootDir)/fcl/src/ecdsa_generic_api.c
SRCS += $(rootDir)/fcl/src/ecdsa_high.c
SRCS += $(rootDir)/fcl/src/sha256_stone.c
SRCS += $(rootDir)/fcl/src/sha512_stone.c
SRCS += $(rootDir)/fcl/src/ucl_aes.c
SRCS += $(rootDir)/fcl/src/ucl_aes_cbc.c
SRCS += $(rootDir)/fcl/src/ucl_aes_cbc_mac.c
SRCS += $(rootDir)/fcl/src/ucl_aes_ecb.c
SRCS += $(rootDir)/fcl/src/ucl_rng.c
SRCS += $(rootDir)/fcl/src/ucl_sha256.c
SRCS += $(rootDir)/fcl/src/ucl_sha384.c
SRCS += $(rootDir)/fcl/src/ucl_sha3.c
SRCS += $(rootDir)/fcl/src/ucl_sha512.c
SRCS += $(rootDir)/fcl/src/ucl_sia256.c
SRCS += $(rootDir)/fcl/src/ucl_sys.c

# Where to find source files for this test
VPATH  = .
VPATH += fcl/src

# Where to find header files for this test
IPATH  = .
IPATH += ./fcl/include
IPATH += ./fcl/include/hash
IPATH += ./fcl/include/ucl

# Enable assertion checking for development
PROJ_CFLAGS+=-DMXC_ASSERT_ENABLE
PROJ_CFLAGS+=-DASYMMETRIC -DWORD32

#MXC_OPTIMIZE_CFLAGS=-Os

# Point this variable to a linker file to override the default file
#LINKERFILE=$(CMSIS_ROOT)/Device/Maxim/$(TARGET_UC)/Source/GCC/$(TARGET_LC).ld

################################################################################
# Include external library makefiles here

# Include the BSP
BOARD_DIR=$(LIBS_DIR)/Boards/$(BOARD)
include $(BOARD_DIR)/board.mk

# Include the Peripheral Driver Library
PERIPH_DRIVER_DIR=$(LIBS_DIR)/$(TARGET)PeriphDriver
include ${PERIPH_DRIVER_DIR}/periphdriver.mk

# Include Cordio BTLE Library
TRACE=1
CORDIO_DIR=$(LIBS_DIR)/../NDALibraries/BTLE
include ${CORDIO_DIR}/btle.mk

# Include MAXUSB library
MAXUSB_DIR=$(LIBS_DIR)/MAXUSB
include $(MAXUSB_DIR)/maxusb.mk

################################################################################
# Include the rules for building for this target. All other makefiles should be
# included before this one.
include $(CMSIS_ROOT)/Device/Maxim/$(TARGET_UC)/Source/$(COMPILER)/$(TARGET_LC).mk
