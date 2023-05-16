*************** Building OpenOCD ***************
cd openocd-max32xxx
./bootstrap
./configure --enable-cmsis-dap
make


*************** Flashing ***************
cd openocd-max32xxx
./src/openocd -f tcl/interface/cmsis-dap.cfg -f tcl/target/max32665.cfg -s tcl -c "program ../max32665_hci.elf verify reset exit"

Green LED will flash slowly when device is operating properly. Green LED will stop flashing
and RED LED may flash quickly when lockup is detected. 

*************** UART Settings ***************
Baud: 115200
Flow Control: Disabled
Parity: None
Bits/character: 8


*************** Caveats ***************
o Most extended scanner features still being developed.
o Periodic advertiser list commands not yet implemented.

*************** Action Items ***************

*************** Chagne Log ***************
Version 0: 
    Initial submission.
    5 November 2018

Version 1: 
    Update HCI/LL version.
    Correctly reply to Read Local Supported Commands.
    Update Feature support.
    Accept valid Data Length commands.
    Implement the following commands in lhci interface:
        o HCI_Read_Local_P-256_Public_Key
        o LE_Set_Extended_Advertising_Params
    20 November 2018

Version 2: 
    Fixed the return event code for LE Meta events.
    Added Channel Selection Algorithm events.
    Implement the following commands in lhci interface:
        o LE_Read_Transmit_Power
        o LE_Write_Path_Compensation
        o LE_Read_RF_Path_Compensation
    Fix LE Set Advertising Parameters parameter checking.
    Disallow extended and legacy command mixing.
    30 November 2018

Version 3:
    Finished implenentation of PHY Update procedure.
    Checking for correct remote public key in DH key generation.
    Fixing event notification with Adv/Scan enable commands.
    Fixed extended advertising parameter checking.
    Filtering invalid LLCP PDUs.
    Implemented stubbs for Periodic Advertiser List commands.
    11 December 2018

Version 4:
    Fixed AES byte ordering.
    Added LE Set Advertising Parameters command complete event.
    Fixed OPCODE of LE Set Exted Scan Enable command complete
    Fixed the handling of legacy PDUs in extended commands.
    Fixed the handling of the All PHY settings in LE Set PHY commands
    Fixed the handling of null scan response data sets.
    Fixed the length checking for Encryption request/response PDUs.
    Returning error on asymmetric connection PHY settings.
    Fixed the default TX octets/time default settings.
    20 December 2018

Version 5:
    Fixed formatting for LE Extended Create Connection Command Complete event.
    Fixed translation bug from advertising handle to array index.
    Swaped the TxOctetes and TxTime parameters.
    Enabled legacy scan reports when using extended advertising commands.
    Fixed translation of PHY settings.
    14 January 2019

Version 6:
    Checking for asymmetric PHY settings in the LE Set PHY commands.
    15 January 2019

Version 7:
    Fixing critical section bug with SPIM read/write procedure.
    Fixing return events for extended advertising/scanning.
    18 January 2019

Version 8:
    Fixing advertising bug to allow restarts.
    Sending PHY requests based only on local preferences, not caching remote.
    Fixing overflow issue with advertising duration calculation.
    Fixed the calculation of path compensation.
    Disabled Coded PHY feature.
    30 January 2019

Version 9:
    Rejecting Le Set PHY commands that attempt to use an unsupported PHY.
    Fixing advertising report event types.
    Filtering directed advertisements that are not directed at our scanner. 
    Set periodic advertisements on their own period.
    Switching PHY to secondary phy while extended advertising. 
    6 Februrary 2019
