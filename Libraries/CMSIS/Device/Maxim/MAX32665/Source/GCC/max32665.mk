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
 # $Date: 2021-08-03 10:16:23 -0500 (Tue, 03 Aug 2021) $ 
 # $Revision: 58927 $
 #
 ###############################################################################

ifeq "$(CMSIS_ROOT)" ""
$(error CMSIS_ROOT must be specified)
endif

# The build directory
ifeq "$(BUILD_DIR)" ""
BUILD_DIR=$(CURDIR)/build
endif

ifeq "$(STARTUPFILE)" ""
STARTUPFILE=startup_max32665.S
endif

ifeq "$(LINKERFILE)" ""
LINKERFILE=$(CMSIS_ROOT)/Device/Maxim/MAX32665/Source/GCC/max32665.ld
endif

ifeq "$(ENTRY)" ""
ENTRY=Reset_Handler
endif

ifneq "$(ENABLE_ECC)" ""
ifneq "$(ENABLE_ECC)" "0"
# Enable ECC by initializing. Refer to startup_max32665.S
PROJ_AFLAGS+=-DECC_INIT_ATSTARTUP
PROJ_CFLAGS+=-DENABLE_ECC

# Tell the compiler to use smaller SRAM size when ECC is enabled
SRAM_SIZE=0x70000

ifneq "$(ENABLE_SDMA)" ""
# Keep the SRAM banks separate when using SDMA

# SCN and ADV images are smaller so we can conserve some SRAM for the ARM
ARM_SRAM_START=0x20030000
ifeq "$(SDMA_ADV)" ""
ifeq "$(SDMA_SCN)" ""
# Full SDMA image is too large, need additional SRAM bank
ARM_SRAM_START=0x20050000
endif
endif
# Pass through definition to linker file. Refer to startup_max32665.S
PROJ_AFLAGS+=-D__ARM_SRAM_START=$(ARM_SRAM_START)
endif
endif
endif

# Default is 560kB, use 448kB if ECC is enabled
ifeq "$(SRAM_SIZE)" ""
SRAM_SIZE=0x8C000
endif

# Default TARGET_REVISION
# "A1" in ASCII
ifeq "$(TARGET_REV)" ""
TARGET_REV=0x4131
endif

# Add target specific CMSIS source files
ifneq (${MAKECMDGOALS},lib)
SRCS += ${STARTUPFILE}
SRCS += backup_max32665.S
SRCS += heap.c
SRCS += system_max32665.c
endif

# Add target specific CMSIS source directories
VPATH+=$(CMSIS_ROOT)/Device/Maxim/MAX32665/Source/GCC
VPATH+=$(CMSIS_ROOT)/Device/Maxim/MAX32665/Source

# Add target specific CMSIS include directories
IPATH+=$(CMSIS_ROOT)/Device/Maxim/MAX32665/Include
IPATH+=$(CMSIS_ROOT)/Include

# Add directory with linker include file
LIBPATH+=$(CMSIS_ROOT)/Device/Maxim/MAX32665/Source/GCC

# Include the rules and goals for building
include $(CMSIS_ROOT)/Device/Maxim/MAX32665/Source/GCC/gcc.mk
