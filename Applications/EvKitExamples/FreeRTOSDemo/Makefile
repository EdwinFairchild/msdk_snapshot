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
###############################################################################

# This is the name of the build output file
ifeq "$(PROJECT)" ""
PROJECT=max32665
endif

# Specify the target processor
ifeq "$(TARGET)" ""
TARGET=MAX32665
endif

# Create Target name variables
TARGET_UC:=$(shell echo $(TARGET) | tr a-z A-Z)
TARGET_LC:=$(shell echo $(TARGET) | tr A-Z a-z)

# Select 'GCC' or 'IAR' compiler
COMPILER=GCC

# Specify the board used
ifeq "$(BOARD)" ""
BOARD=EvKit_V1
endif

# This is the path to the CMSIS root directory
ifeq "$(MAXIM_PATH)" ""
LIBS_DIR=../../../Libraries
else
LIBS_DIR=/$(subst \,/,$(subst :,,$(MAXIM_PATH))/Firmware/$(TARGET_UC)/Libraries)
endif
CMSIS_ROOT=$(LIBS_DIR)/CMSIS

# Source files for this test (add path to VPATH below)
SRCS  = main.c
SRCS += FreeRTOS_CLI.c
SRCS += CLI-commands.c
SRCS += freertos_tickless.c

# Where to find source files for this test
VPATH = .

# Where to find header files for this test
IPATH = .

# Make sure that application is build with hardfp
ifeq "$(MFLOAT_FLAGS)" "soft"
$(error -mfloat-abi=soft not supported by FreeRTOS.)
else
ifeq "$(MFLOAT_FLAGS)" "hard"
else
ifeq "$(MFLOAT_FLAGS)" "hardfp"
else
export MFLOAT_FLAGS=hard
endif
endif
endif

# Enable assertion checking for development
PROJ_CFLAGS+=-DMXC_ASSERT_ENABLE

# Prevent warnings for strncat when source buffer is the exact length of the 
# length parameter
PROJ_CFLAGS+=-Wno-stringop-overflow

# Point this variable to a linker file to override the default file
# LINKERFILE=$(CMSIS_ROOT)/Device/Maxim/$(TARGET_UC)/Source/GCC/$(TARGET_LC).ld

################################################################################
# Include external library makefiles here

# Include the BSP
BOARD_DIR=$(LIBS_DIR)/Boards/$(BOARD)
include $(BOARD_DIR)/board.mk

# Include the peripheral driver
PERIPH_DRIVER_DIR=$(LIBS_DIR)/$(TARGET_UC)PeriphDriver
include $(PERIPH_DRIVER_DIR)/periphdriver.mk

# Include the FreeRTOS library, and specify a local FreeRTOSConfig.h file
RTOS_CONFIG_DIR=.
RTOS_DIR=$(LIBS_DIR)/FreeRTOS
include $(RTOS_DIR)/freertos.mk
# Include the FreeRTOS-Plus-CLI (please read license file before using commercially)
IPATH +=$(LIBS_DIR)/FreeRTOS-Plus/Source/FreeRTOS-Plus-CLI
VPATH +=$(LIBS_DIR)/FreeRTOS-Plus/Source/FreeRTOS-Plus-CLI

################################################################################
# Include the rules for building for this target. All other makefiles should be
# included before this one.
include $(CMSIS_ROOT)/Device/Maxim/$(TARGET_UC)/Source/$(COMPILER)/$(TARGET_LC).mk

# The rule to clean out all the build products.
distclean: clean
	$(MAKE) -C ${PERIPH_DRIVER_DIR} clean
