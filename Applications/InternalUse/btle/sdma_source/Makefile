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
# $Id: Makefile 58927 2021-08-03 15:16:23Z nathan.goldstick $
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
LIBS_DIR=$(realpath ../../../../Libraries)
NDALIBS_DIR=$(realpath ../../../../NDALibraries)
else
LIBS_DIR=/$(subst \,/,$(subst :,,$(MAXIM_PATH))/Firmware/$(TARGET_UC)/Libraries)
endif
CMSIS_ROOT=$(LIBS_DIR)/CMSIS

# Source files for this test (add path to VPATH below)
SRCS  = main.c
SRCS += chci_drv.c
SRCS += stack_setup.c
SRCS += wakeup_sdma.c

VPATH += .
VPATH += $(abspath $(LIBS_DIR)/MAX32665PeriphDriver/Source)

# Where to find header files for this test
IPATH  = .

IPATH += $(abspath $(LIBS_DIR)/MAX32665PeriphDriver/Include)
IPATH += $(abspath $(LIBS_DIR)/Boards/$(BOARD)/Include)

# Enable assertion checking for development
# PROJ_CFLAGS+=-DMXC_ASSERT_ENABLE=TRUE
# PROJ_CFLAGS+=-DWSF_TRACE_ENABLED=TRUE
# PROJ_CFLAGS+=-DLL_TRACE_ENABLED=TRUE
# PROJ_CFLAGS+=-DLL_DBG_PIN_ENABLED=TRUE
# PROJ_CFLAGS+=-DSMP_EXTRA_TRACE=TRUE

ifneq "$(SCN_IMAGE)$(ADV_IMAGE)" ""
ifneq "$(SCN_IMAGE)" ""
##### SCN Image #####
# PROJ_CFLAGS+=-DINIT_BROADCASTER
PROJ_CFLAGS+=-DINIT_OBSERVER
PROJ_CFLAGS+=-DINIT_CENTRAL
# PROJ_CFLAGS+=-DINIT_PERIPHERAL
# PROJ_CFLAGS+=-DINIT_ENCRYPTED
PROJ_CFLAGS+=-DINIT_PHY
PROJ_CFLAGS+=-DLL_ENABLE_TESTER=TRUE
ifeq "$(ENABLE_LL_TESTER)" ""
ENABLE_LL_TESTER=1
endif

SDMASHCODE_INC=sdma_scn_shcode.inc
SDMACODE_INC=sdma_scn_code.inc
SDMADATA_INC=sdma_scn_data.inc
SDMAMAP_INC=sdma_scn_map.inc

endif

ifneq "$(ADV_IMAGE)" ""
##### ADV Image #####
PROJ_CFLAGS+=-DINIT_BROADCASTER
# PROJ_CFLAGS+=-DINIT_OBSERVER
# PROJ_CFLAGS+=-DINIT_CENTRAL
PROJ_CFLAGS+=-DINIT_PERIPHERAL
# PROJ_CFLAGS+=-DINIT_ENCRYPTED
PROJ_CFLAGS+=-DINIT_PHY
PROJ_CFLAGS+=-DLL_ENABLE_TESTER=TRUE
ifeq "$(ENABLE_LL_TESTER)" ""
ENABLE_LL_TESTER=1
endif

SDMASHCODE_INC=sdma_adv_shcode.inc
SDMACODE_INC=sdma_adv_code.inc
SDMADATA_INC=sdma_adv_data.inc
SDMAMAP_INC=sdma_adv_map.inc
endif
else

##### Full Image #####
PROJ_CFLAGS+=-DINIT_BROADCASTER
PROJ_CFLAGS+=-DINIT_OBSERVER
PROJ_CFLAGS+=-DINIT_CENTRAL
PROJ_CFLAGS+=-DINIT_PERIPHERAL
PROJ_CFLAGS+=-DINIT_ENCRYPTED
PROJ_CFLAGS+=-DINIT_PHY
PROJ_CFLAGS+=-DLL_ENABLE_TESTER=TRUE
ifeq "$(ENABLE_LL_TESTER)" ""
ENABLE_LL_TESTER=1
endif

SDMASHCODE_INC=sdma_adv_shcode.inc
SDMACODE_INC=sdma_code.inc
SDMADATA_INC=sdma_data.inc
SDMAMAP_INC=sdma_map.inc
endif

PROJ_CFLAGS+=-DENABLE_RESTART_FROM_SLEEP=1

# Enable software AES
PROJ_CFLAGS+=-DENABLE_SOFT_AES=1

PROJ_CFLAGS+=-DCONSOLE_UART=1
PROJ_AFLAGS+=-DCONSOLE_UART=1

################################################################################
# Include external library makefiles here

# Include the Wireless Library
CORDIO_DIR=$(NDALIBS_DIR)/BTLE
include ${CORDIO_DIR}/btle.mk

################################################################################
# Include the rules for building for this target. All other makefiles should be
# included before this one.
include $(CMSIS_ROOT)/Device/Maxim/$(TARGET_UC)/Source/$(COMPILER)/$(TARGET_LC).mk

SDMASHCODE_TXT=$(BUILD_DIR)/$(PROJECT)shcode.txt
SDMACODE_TXT=$(BUILD_DIR)/$(PROJECT)code.txt
SDMADATA_TXT=$(BUILD_DIR)/$(PROJECT)data.txt
SDMAMAP=$(BUILD_DIR)/$(PROJECT).map
SDMAMAP_HEADER=$(BUILD_DIR)/$(PROJECT)map.h

mem: all
	@grep '^[0-9A-F][0-9A-F]\ ' ${SDMASHCODE_TXT} > ${SDMASHCODE_INC} # Remove the address lines
	@grep '^[0-9A-F][0-9A-F]\ ' ${SDMACODE_TXT} > ${SDMACODE_INC} # Remove the address lines
	@grep '^[0-9A-F][0-9A-F]\ ' ${SDMADATA_TXT} > ${SDMADATA_INC} # Remove the address lines
	@sed -i -e 's/  / /g' $(SDMASHCODE_INC) $(SDMACODE_INC) $(SDMADATA_INC)  # Replace double space with single
	@sed -i -e 's/ /,0x/g' $(SDMASHCODE_INC) $(SDMACODE_INC) $(SDMADATA_INC)  # Replace ' ' with ',0x'
	@sed -i -e 's_.*_0x&_' $(SDMASHCODE_INC) $(SDMACODE_INC) $(SDMADATA_INC)  # Add 0x to start of every line
	@sed -i -e 's/.*/&,/g' $(SDMASHCODE_INC) $(SDMACODE_INC) $(SDMADATA_INC)  # Add ',' to the end of every line
	@cp '${SDMAMAP_HEADER}' '${SDMAMAP_INC}'

clean: myclean

myclean:
	@rm -rf *.inc
