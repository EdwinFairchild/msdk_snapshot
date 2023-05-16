#! /usr/bin/env python
filename="../common.py"
exec(open(filename).read())

# [HCI] Reset()
send_command("01030c00")
#input("Press ENTER key to continue...")

# [HCI] HCI event received: OPCODE_RESET (0xE)
#                   Command_Opcode = 0xC03 (3075)
#                   Event_Code = 0xE (14)
#                   Num_HCI_Command_Packets = 0x1 (1)
#                   Parameter_Length = 0x4 (4)
#                   Status = 0x0 (0)
# COM <- 04 0e 04 01 03 0c 00 

# [HCI] VsSetBdAddr(addr='\xc8\x85\xf4\xf9*\xe4')
# COM -> 01 f0 ff 06 c8 85 f4 f9 2a e4 
send_command("01f0ff06c885f4f92ae4")
#input("Press ENTER key to continue...")

# [HCI] HCI event received: OPCODE_VS_SET_BD_ADDR (0xE)
#                   Command_Opcode = 0xFFF0 (65520)
#                   Num_HCI_Command_Packets = 0x1 (1)
#                   Status = 0x0 (0)
# COM <- 04 0E 03 01 F0 FF

# [HCI] ReadBdAddr()
# COM -> 01 09 10 00 
send_command("01091000")
#input("Press ENTER key to continue...")

# [HCI] HCI event received: OPCODE_READ_BD_ADDR (0xE)
#                   BD_ADDR = 0xE42AF9F485C8 (250873233311176)
#                   Command_Opcode = 0x1009 (4105)
#                   Event_Code = 0xE (14)
#                   Num_HCI_Command_Packets = 0x1 (1)
#                   Parameter_Length = 0xA (10)
#                   Status = 0x0 (0)
# COM <- 04 0e 0a 01 09 10 00 c8 85 f4 f9 2a e4 

#########################################
# TODO: Verify whether these are in fact necessary.
## [HCI] SetEventMask(Event_Mask=2305983746702149776L)
## COM -> 01 01 0c 08 90 88 01 00 00 80 00 20 
#send_command("01010c089088010000800020")
#
## [HCI] HCI event received: OPCODE_SET_EVENT_MASK (0xE)
##                   Command_Opcode = 0xC01 (3073)
##                   Event_Code = 0xE (14)
##                   Num_HCI_Command_Packets = 0x1 (1)
##                   Parameter_Length = 0x4 (4)
##                   Status = 0x0 (0)
## COM <- 04 0e 04 01 01 0c 00 
#
## [HCI] SetEventMaskPage2(Event_Mask_Page_2=8388608)
## COM -> 01 63 0c 08 00 00 80 00 00 00 00 00 
#send_command("01630c080000800000000000")
#
## [HCI] HCI event received: OPCODE_SET_EVENT_MASK_PAGE2 (0xE)
##                   Command_Opcode = 0xC63 (3171)
##                   Event_Code = 0xE (14)
##                   Num_HCI_Command_Packets = 0x1 (1)
##                   Parameter_Length = 0x4 (4)
##                   Status = 0x0 (0)
## COM <- 04 0e 04 01 63 0c 00 
#
## [HCI] LeSetEventMask(LE_Event_Mask=536870399)
## COM -> 01 01 20 08 ff fd ff 1f 00 00 00 00 
#send_command("01012008fffdff1f00000000")
#
## [HCI] HCI event received: OPCODE_LE_SET_EVENT_MASK (0xE)
##                   Command_Opcode = 0x2001 (8193)
##                   Event_Code = 0xE (14)
##                   Num_HCI_Command_Packets = 0x1 (1)
##                   Parameter_Length = 0x4 (4)
##                   Status = 0x0 (0)
## COM <- 04 0e 04 01 01 20 00 
#
## [HCI] VsSetEventMask(eventMask=0)
## COM -> 01 e1 ff 08 00 00 00 00 00 00 00 00 
#send_command("01e1ff080000000000000000")
#
## [HCI] HCI event received: OPCODE_VS_SET_EVENT_MASK (0xE)
##                   Command_Opcode = 0xFFE1 (65505)
##                   Num_HCI_Command_Packets = 0x1 (1)
##                   Status = 0x0 (0)
## COM <- 04 0e 04 01 e1 ff 00 
#########################################

# [HCI] LeSetDefaultPhy(ALL_PHYS=0,
#             TX_PHYS=1,
#             RX_PHYS=1)
# COM -> 01 31 20 03 00 01 01 
send_command("01312003000101")
#input("Press ENTER key to continue...")

# [HCI] HCI event received: OPCODE_LE_SET_DEF_PHY (0xE)
#                   Command_Opcode = 0x2031 (8241)
#                   Event_Code = 0xE (14)
#                   Num_HCI_Command_Packets = 0x1 (1)
#                   Parameter_Length = 0x4 (4)
#                   Status = 0x0 (0)
# COM <- 04 0e 04 01 31 20 00 

# [HCI] LeSetExtAdvParam(Advertising_Handle=1,
#              Advertising_Event_Properties=2, # Scannable
#              Primary_Advertising_Interval_Min=32,
#              Primary_Advertising_Interval_Max=16384,
#              Primary_Advertising_Channel_Map=7,
#              Own_Address_Type=0,
#              Peer_Address_Type=0,
#              Peer_Address='\x00\x00\x00\x00\x00\x00',
#              Advertising_Filter_Policy=0,
#              Advertising_Tx_Power=127,
#              Primary_Advertising_PHY=1,
#              Secondary_Advertising_Max_Skip=0,
#              Secondary_Advertising_PHY=1,
#              Advertising_SID=15,
#              Scan_Request_Notification_Enable=False)
# COM -> 01 36 20 19 01 02 00 20 00 00 00 40 00 07 00 00 00 00 00 00 00 00 00 7f 01 00 01 0f 00 
send_command("01362019010200200000004000070000000000000000007f0100010f00")
#input("Press ENTER key to continue...")

# [HCI] HCI event received: OPCODE_LE_SET_EXT_ADV_PARAM (0xE)
#                   Command_Opcode = 0x2036 (8246)
#                   Event_Code = 0xE (14)
#                   Num_HCI_Command_Packets = 0x1 (1)
#                   Parameter_Length = 0x5 (5)
#                   Selected_Tx_Power = 0x9 (9)
#                   Status = 0x0 (0)
# COM <- 04 0e 05 01 36 20 00 09 

# [HCI] LeSetExtAdvData(Advertising_Handle=1,
#                  Operation=3,
#                  Fragment_Preference=0,
#                  Advertising_Data='')
# COM -> 01 37 20 03 01 03 00 00
send_command("0137200401030000")
#input("Press ENTER key to continue...")

# [HCI] HCI event received: OPCODE_LE_SET_EXT_ADV_DATA (0xE)
#                   Command_Opcode = 0x???? (????)
#                   Event_Code = 0x? (??)
#                   Num_HCI_Command_Packets = 0x1 (1)
#                   Parameter_Length = 0x4 (4)
#                   Status = 0x0 (0)
# COM <- 04 0E 04 01 37 20 00 

# [HCI] LeSetExtScanResponseData(Advertising_Handle=1,
#                  Operation=3,
#                  Fragment_Preference=0,
#                  Scan_Response_Data='')
# COM -> 01 38 20 03 01 03 00 00
#send_command("0138200401030000")
send_command("013820040103001F3031323334353637383965666768696A6B6C6D6E6F70717273747576777879")
#input("Press ENTER key to continue...")

# [HCI] HCI event received: OPCODE_LE_SET_EXT_SCAN_RESP_DATA (0xE)
#                   Command_Opcode = 0x???? (????)
#                   Event_Code = 0x? (??)
#                   Num_HCI_Command_Packets = 0x1 (1)
#                   Parameter_Length = 0x4 (4)
#                   Status = 0x0 (0)
# COM <- 04 0E 04 01 38 20 00 

# [HCI] LeSetExtAdvEnable(Enable=True,
#               Advertising_Handle=1,
#               Duration=0,
#               Max_Extended_Advertising_Events=0,
#               enable_param=[{'Duration': 0,
#               'Advertising_Handle': 1,
#               'Max_Extended_Advertising_Events': 0}])
# COM -> 01 39 20 06 01 01 01 00 00 00 
send_command("01392006010101000000")
#input("Press ENTER key to continue...")

# [HCI] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                   Command_Opcode = 0x2039 (8249)
#                   Event_Code = 0xE (14)
#                   Num_HCI_Command_Packets = 0x1 (1)
#                   Parameter_Length = 0x4 (4)
#                   Status = 0x0 (0)
# COM <- 04 0e 04 01 39 20 00 

# [HCI] LeSetExtScanResponseData(Advertising_Handle=1,
#                  Operation=3,
#                  Fragment_Preference=0,
#                  Scan_Response_Data='')
# COM -> 01 38 20 03 01 03 00 00
#send_command("0138200401030000")
#send_command("01382004010300203031323334353637383965666768696A6B6C6D6E6F707172737475767778797A")
send_command("01382004810300203031323334353637383965666768696A6B6C6D6E6F707172737475767778797A")
#input("Press ENTER key to continue...")

# [HCI] HCI event received: OPCODE_LE_SET_EXT_SCAN_RESP_DATA (0xE)
#                   Command_Opcode = 0x???? (????)
#                   Event_Code = 0x? (??)
#                   Num_HCI_Command_Packets = 0x1 (1)
#                   Parameter_Length = 0x4 (4)
#                   Status != 0x0 (0)
# COM <- 04 0E 04 01 38 20 ?? 

while(1):
  rcv_event()

