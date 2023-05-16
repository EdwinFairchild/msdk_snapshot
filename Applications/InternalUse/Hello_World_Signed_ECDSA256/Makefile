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
 # $Date: 2019-04-26 09:52:03 -0500 (Fri, 26 Apr 2019) $ 
 # $Revision: 42951 $
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

# Location of signing tools
SIGN_TOOLS_DIR=./PACKAGE_PAOLA_1.0.0
CA_SIGN_BUILD=$(SIGN_TOOLS_DIR)/bin/ca_sign_build
SESSION_BUILD=$(SIGN_TOOLS_DIR)/bin/session_build

TEST_KEY=./crk_ecdsa_angela_test.key

FLASH_DEST=0x10000000
# NOTE: LOAD_ADDRESS is FLASH_ADDRESS minus the 0x
LOAD_ADDRESS=10000000

SCP_PACKET_DIR=./scp_packets

# Create Target name variables
TARGET_UC:=$(shell echo $(TARGET) | tr a-z A-Z)
TARGET_LC:=$(shell echo $(TARGET) | tr A-Z a-z)

# Select 'GCC' or 'IAR' compiler
COMPILER=GCC

# Specify the board used
ifeq "$(BOARD)" ""
#BOARD=EvKit_V1
BOARD=Emulator
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
SRCS  += sla_header.c

# Where to find source files for this test
VPATH=.

# Where to find header files for this test
IPATH = .

# Enable assertion checking for development
PROJ_CFLAGS+=-DMXC_ASSERT_ENABLE 

# Point this variable to a linker file to override the default file
#LINKERFILE=$(CMSIS_ROOT)/Device/Maxim/$(TARGET_UC)/Source/GCC/$(TARGET_LC).ld
LINKERFILE=max32570_sla.ld

################################################################################
# Include external library makefiles here

# Include the BSP
BOARD_DIR=$(LIBS_DIR)/Boards/$(BOARD)
include $(BOARD_DIR)/board.mk

# Include the peripheral driver
PERIPH_DRIVER_DIR=$(LIBS_DIR)/$(TARGET_UC)PeriphDriver
include $(PERIPH_DRIVER_DIR)/periphdriver.mk

################################################################################
# Include the rules for building for this target. All other makefiles should be
# included before this one.
include $(CMSIS_ROOT)/Device/Maxim/$(TARGET_UC)/Source/$(COMPILER)/$(TARGET_LC).mk

# The rule to clean out all the build products.
distclean: clean
	$(MAKE) -C ${PERIPH_DRIVER_DIR} clean

all: 
	arm-none-eabi-objcopy $(BUILD_DIR)/$(PROJECT).elf -O binary $(BUILD_DIR)/$(PROJECT).bin --verbose

	mkdir -p $(SCP_PACKET_DIR)
	rm -rf $(SCP_PACKET_DIR)/*.packet
	rm -rf $(SCP_PACKET_DIR)/sb_script.txt

# Direct call of ca_sign_build executable (ECDSA)
	$(CA_SIGN_BUILD) ca=$(BUILD_DIR)/$(PROJECT).bin sca=$(BUILD_DIR)/$(PROJECT).sbin algo=ecdsa ecdsa_file=$(TEST_KEY) verbose=0 load_address=$(LOAD_ADDRESS)
	arm-none-eabi-objcopy -I binary $(BUILD_DIR)/$(PROJECT).sbin --change-addresses $(FLASH_DEST) -O elf32-littlearm $(BUILD_DIR)/$(PROJECT).signed.elf

# Direct call of session_build (ECDSA)
	$(OBJCOPY) -I binary -O srec --srec-forceS3 --srec-len=128 --adjust-vma $(FLASH_DEST) $(BUILD_DIR)/$(PROJECT).sbin $(SCP_PACKET_DIR)/binary.s19
	# Write SLA to flash.
	echo "write-file $(SCP_PACKET_DIR)/binary.s19" >> $(SCP_PACKET_DIR)/sb_script.txt
	#NOTE: chunk_size is interesting
	# 8186 gives total packet size of 8250, header is 22 bytes, payload is 0x1fe0 (8160), signature is 64 bytes, 4 bytes of protocol goo
	# For ES17, payload must be a multiple of 16 bytes to write to encrypted flash.
	# Large packet size build
	$(SESSION_BUILD) session_mode=SCP_ANGELA_ECDSA  verbose=0 output_file=$(SCP_PACKET_DIR)/scp pp=ECDSA addr_offset=00000000 chunk_size=8186 script_file=$(SCP_PACKET_DIR)/sb_script.txt ecdsa_file=$(TEST_KEY)
	# Small packet size build
#	$(SESSION_BUILD) session_mode=SCP_ANGELA_ECDSA  verbose=0 output_file=$(SCP_PACKET_DIR)/scp pp=ECDSA addr_offset=00000000 chunk_size=282 script_file=$(SCP_PACKET_DIR)/sb_script.txt ecdsa_file=$(TEST_KEY)

