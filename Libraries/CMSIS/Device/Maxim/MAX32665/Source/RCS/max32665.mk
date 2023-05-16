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

CMSIS_ROOT := $(abspath $(CMSIS_ROOT))

# The build directory
ifeq "$(BUILD_DIR)" ""
BUILD_DIR=$(CURDIR)/build
endif

ifeq "$(STARTUPFILE)" ""
STARTUPFILE=max32665_startup.asm
endif

ifeq "$(ENTRY)" ""
ENTRY=entry
endif

# SDMA Memory map
# 0x20000000
# 		OTP
# 		Shared memory
# 0x20004000
#     Startup Code
# 0x20008000
#     Data
#
#      ^
#     Stack
# 0x20010000
#     Code
#
# SDMA_LEN

# Limit of the code memory
ifeq "$(SDMA_LEN)" ""
# 0x4000 for the data,
# 0x8000 for the data,
# 0x20000 for the code
# Upper limmit isn't significant, GCC linker will trim the top section to match
# The length of the code section size
SDMA_LEN    = 0x2C000
endif

OTP_ADDR = 0x20000000

SHARED_ADDR = 0x20000040
SHARED_ADDR_SEG_SIZE=512

SHCODE_ADDR = 0x20004000
SHCODE_MEM_LIM = 0x20008000

DATA_ADDR = 0x20008000
DATA_MEM_LIM = 0x20010000

CODE_ADDR = 0x20010000
#CODE_MEM_LIM = 0x20030000 # Determined via SDMA_LEN

# Convert the memory addresses for the different sections to decimal
OTP_ADDR_DEC=$(shell printf "%d" $(OTP_ADDR))
SHARED_ADDR_DEC=$(shell printf "%d" $(SHARED_ADDR))
SHARED_ADDR_ARM_DEC=$(SHARED_ADDR_DEC)
SHARED_ADDR_SDMA_DEC=$(shell expr $(SHARED_ADDR_DEC) \+ $(SHARED_ADDR_SEG_SIZE))
SHCODE_ADDR_DEC=$(shell printf "%d" $(SHCODE_ADDR))
DATA_ADDR_DEC=$(shell printf "%d" $(DATA_ADDR))
CODE_ADDR_DEC=$(shell printf "%d" $(CODE_ADDR))

# Convert the memory limits for the different sections to decimal
SHCODE_MEM_LIM_DEC=$(shell printf "%d" $(SHCODE_MEM_LIM))
DATA_MEM_LIM_DEC=$(shell printf "%d" $(DATA_MEM_LIM))
#CODE_MEM_LIM_DEC=$(shell printf "%d" $(CODE_MEM_LIM)) # Determined via SDMA_LEN

# Convert sdma size to decimal
SDMA_LEN_DEC=$(shell printf "%d" $(SDMA_LEN))

# Calculate section sizes.
SHCODE_MEM_LEN_DEC=$(shell expr $(SHCODE_MEM_LIM_DEC) - $(SHCODE_ADDR_DEC))
DATA_MEM_LEN_DEC=$(shell expr $(DATA_MEM_LIM_DEC) - $(DATA_ADDR_DEC))
#CODE_MEM_LEN_DEC=$(shell expr $(CODE_MEM_LIM_DEC) - $(CODE_ADDR_DEC))

# Convert sizes to hex.
SHCODE_MEM_LEN=$(shell printf "0x%x" $(SHCODE_MEM_LEN_DEC))
DATA_MEM_LEN=$(shell printf "0x%x" $(DATA_MEM_LEN_DEC))

# Calculate code size using SDMA_LEN.
CODE_MEM_LEN_DEC=$(shell expr $(SDMA_LEN_DEC) - $(SHCODE_MEM_LEN_DEC) - $(DATA_MEM_LEN_DEC))

# Convert code size to hex.
CODE_MEM_LEN=$(shell printf "0x%x" $(CODE_MEM_LEN_DEC))

# Generate code memory limits using SDMA_LEN.
CODE_MEM_LIM_DEC=$(shell expr $(CODE_ADDR_DEC) + $(CODE_MEM_LEN_DEC))
CODE_MEM_LIM=$(shell printf "0x%x" $(CODE_MEM_LIM_DEC))

# Convert addresses back to hex
#SHARED_ADDR=$(shell printf "0x%x" $(SHARED_ADDR_DEC))
SHARED_ADDR_ARM=$(shell printf "0x%x" $(SHARED_ADDR_ARM_DEC))
SHARED_ADDR_SDMA=$(shell printf "0x%x" $(SHARED_ADDR_SDMA_DEC))

STACK_START=$(shell printf "0x%x" $(DATA_MEM_LIM))

SHCODE_MEM_END=$(shell printf "0x%x" $(shell expr $(SHCODE_MEM_LIM_DEC) \- 1))
CODE_MEM_END=$(shell printf "0x%x" $(shell expr $(CODE_MEM_LIM_DEC) \- 1))
DATA_MEM_END=$(shell printf "0x%x" $(shell expr $(DATA_MEM_LIM_DEC) \- 1))

SHCODE_MEMRANGE=$(SHCODE_ADDR)-$(SHCODE_MEM_END)
DATA_MEMRANGE=$(DATA_ADDR)-$(DATA_MEM_END)
CODE_MEMRANGE=$(CODE_ADDR)-$(CODE_MEM_END)

export SHCODE_MEMRANGE
export DATA_MEMRANGE
export CODE_MEMRANGE

export SHARED_ADDR_ARM
export SHARED_ADDR_SDMA

# Default TARGET_REVISION
# "A1" in ASCII
ifeq "$(TARGET_REV)" ""
TARGET_REV=0x4131
endif

# Add target specific CMSIS source files
ifneq (${MAKECMDGOALS},lib)
SRCS += ${STARTUPFILE}
SRCS += max32665_sdma.c
endif

# Add target specific CMSIS source directories
VPATH+=$(CMSIS_ROOT)/Device/Maxim/MAX32665/Source/RCS

# Add target specific CMSIS include directories
IPATH+=$(CMSIS_ROOT)/Device/Maxim/MAX32665/Include
IPATH+=$(CMSIS_ROOT)/Device/Maxim/MAX32665/Source/RCS

LIBPATH=$(CMSIS_ROOT)/Lib/RCS

# Include the rules and goals for building
include $(CMSIS_ROOT)/Device/Maxim/MAX32665/Source/RCS/rcs.mk
