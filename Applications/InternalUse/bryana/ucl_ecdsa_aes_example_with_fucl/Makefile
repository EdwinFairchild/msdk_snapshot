################################################################################
 # Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
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
 # $Date: 2016-09-09 11:58:34 -0500 (Fri, 09 Sep 2016) $ 
 # $Revision: 24340 $
 #
 ###############################################################################

UCL=FREEUCL

# This is the name of the build output file
PROJECT=max32665

# Specify the target processor
TARGET=MAX32665

# Create Target name variables
TARGET_UC:=$(shell echo $(TARGET) | tr a-z A-Z)
TARGET_LC:=$(shell echo $(TARGET) | tr A-Z a-z)

# Select 'GCC' or 'IAR' compiler
COMPILER=GCC

# Specify the board used
ifeq "$(BOARD)" ""
#BOARD=Emulator
BOARD=EvKit_V1
endif

# This is the path to the CMSIS root directory
ifeq "$(MAXIM_PATH)" ""
#LIBS_DIR=../../../Libraries
LIBS_DIR=../../../../Libraries
else
LIBS_DIR=/$(subst \,/,$(subst :,,$(MAXIM_PATH))/Firmware/$(TARGET_UC)/Libraries)
endif
CMSIS_ROOT=$(LIBS_DIR)/CMSIS

# Source files for this test (add path to VPATH below)
# find fcl/src -iname "*.c" -printf "SRCS += \$(rootDir)/%p\n" | sort
SRCS  = main.c
ifeq ($(UCL),FREEUCL)
SRCS += $(rootDir)/fcl/src/aes.c
#SRCS += $(rootDir)/fcl/src/aestest.c
SRCS += $(rootDir)/fcl/src/bignum.c
SRCS += $(rootDir)/fcl/src/ecdsa_generic_api.c
SRCS += $(rootDir)/fcl/src/ecdsa_high.c
#SRCS += $(rootDir)/fcl/src/ecdsatest.c
#SRCS += $(rootDir)/fcl/src/main.c
SRCS += $(rootDir)/fcl/src/sha256_stone.c
SRCS += $(rootDir)/fcl/src/sha512_stone.c
#SRCS += $(rootDir)/fcl/src/shatest.c
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
endif

# Where to find source files for this test
VPATH=.
ifeq ($(UCL),FREEUCL)
VPATH +=./fcl/src
endif

# Where to find header files for this test
IPATH = .
ifeq ($(UCL),FREEUCL)
IPATH += ./fcl/include
IPATH += ./fcl/include/hash
IPATH += ./fcl/include/ucl
endif

# Enable assertion checking for development
PROJ_CFLAGS+=-DMXC_ASSERT_ENABLE 
PROJ_CFLAGS+=-DASYMMETRIC -DWORD32

# Specify the target revision to override default
# "A2" in ASCII
# TARGET_REV=0x4132

# Use this variables to specify and alternate tool path
#TOOL_DIR=/opt/gcc-arm-none-eabi-4_8-2013q4/bin

# Use these variables to add project specific tool options
#PROJ_CFLAGS+=--specs=nano.specs
#PROJ_LDFLAGS+=--specs=nano.specs

# Point this variable to a startup file to override the default file
#STARTUPFILE=start.S

# Set MXC_OPTIMIZE to override the default optimization level 
#MXC_OPTIMIZE_CFLAGS=-Og

# Point this variable to a linker file to override the default file
# LINKERFILE=$(CMSIS_ROOT)/Device/Maxim/$(TARGET_UC)/Source/GCC/$(TARGET_LC).ld
# LINKERFILE=$(CMSIS_ROOT)/Device/Maxim/$(TARGET_UC)/Source/GCC/$(TARGET_LC)_emulator.ld
# LINKERFILE=$(CMSIS_ROOT)/Device/Maxim/$(TARGET_UC)/Source/GCC/$(TARGET_LC)_emulator_xip.ld

################################################################################
# Include external library makefiles here

# Include the BSP
BOARD_DIR=$(LIBS_DIR)/Boards/$(BOARD)
include $(BOARD_DIR)/board.mk

# Include the peripheral driver
PERIPH_DRIVER_DIR=$(LIBS_DIR)/$(TARGET_UC)PeriphDriver
include $(PERIPH_DRIVER_DIR)/periphdriver.mk

# Add UCL library to library list
#LIBS += $(rootDir)/libucl-2.5.15-max32652.a
ifneq ($(UCL),FREEUCL)
LIBUCL_DIR=./libucl
IPATH += $(LIBUCL_DIR)/include
IPATH += $(LIBUCL_DIR)/include/ucl
LIBS += $(LIBUCL_DIR)/libucl-2.5.15_max32652.a
endif

################################################################################
# Include the rules for building for this target. All other makefiles should be
# included before this one.
include $(CMSIS_ROOT)/Device/Maxim/$(TARGET_UC)/Source/$(COMPILER)/$(TARGET_LC).mk

test:
	@echo $(UCL)
ifeq ($(UCL),FREEUCL)
	@echo free UCL
else
	@echo not free UCL
endif
ifneq ($(UCL),FREEUCL)
	@echo not free UCL
else
	@echo free UCL
endif

# The rule to clean out all the build products.
distclean: clean
	$(MAKE) -C ${PERIPH_DRIVER_DIR} clean
