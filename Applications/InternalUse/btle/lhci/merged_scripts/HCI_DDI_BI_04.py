#! /usr/bin/env python

filename="../common.py"
exec(open(filename).read())

#   5.030    [HCI - COM51] Reset()
#   5.030    COM51 -> 01 03 0c 00 
send_command("01030c00", port = port0)

#   5.062    [HCI - COM51] HCI event received: OPCODE_RESET (0xE)
#                              Command_Opcode = 0xC03 (3075)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   5.062    COM51 <- 04 0e 04 01 03 0c 00 

#   5.077    [HCI - COM3] Reset()
#   5.077    COM3 -> 01 03 0c 00 
send_command("01030c00", port = port1)

#   5.140    [HCI - COM3] HCI event received: OPCODE_RESET (0xE)
#                              Command_Opcode = 0xC03 (3075)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   5.140    COM3 <- 04 0e 04 01 03 0c 00 

#   5.155    [HCI - COM3] Reset()
#   5.155    COM3 -> 01 03 0c 00 
send_command("01030c00", port = port1)

#   5.218    [HCI - COM3] HCI event received: OPCODE_RESET (0xE)
#                              Command_Opcode = 0xC03 (3075)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   5.218    COM3 <- 04 0e 04 01 03 0c 00 

#   5.249    [HCI - COM51] Reset()
#   5.249    COM51 -> 01 03 0c 00 
send_command("01030c00", port = port0)

#   5.265    [HCI - COM51] HCI event received: OPCODE_RESET (0xE)
#                              Command_Opcode = 0xC03 (3075)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   5.265    COM51 <- 04 0e 04 01 03 0c 00 

#   5.999    [HCI - COM51] VsSetBdAddr(addr='\xc8\x85\xf4\xf9*\xe4')
#   5.999    COM51 -> 01 f0 ff 06 c8 85 f4 f9 2a e4 
send_command("01f0ff06c885f4f92ae4", port = port0)

#   6.015    [HCI - COM51] HCI event received: OPCODE_VS_SET_BD_ADDR (0xE)
#                              Command_Opcode = 0xFFF0 (65520)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Status = 0x0 (0)
#   6.015    COM51 <- 04 0e 04 01 f0 ff 00 

#   6.031    [HCI - COM3] ReadBdAddr()
#   6.031    COM3 -> 01 09 10 00 
send_command("01091000", port = port1)

#   6.072    [HCI - COM3] HCI event received: OPCODE_READ_BD_ADDR (0xE)
#                              BD_ADDR = 0x5F123456 (1595028566)
#                              Command_Opcode = 0x1009 (4105)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0xA (10)
#                              Status = 0x0 (0)
#   6.072    COM3 <- 04 0e 0a 01 09 10 00 56 34 12 5f 00 00 

#   6.088    [HCI - COM51] ReadBdAddr()
#   6.088    COM51 -> 01 09 10 00 
send_command("01091000", port = port0)

#   6.103    [HCI - COM51] HCI event received: OPCODE_READ_BD_ADDR (0xE)
#                              BD_ADDR = 0xE42AF9F485C8 (250873233311176)
#                              Command_Opcode = 0x1009 (4105)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0xA (10)
#                              Status = 0x0 (0)
#   6.103    COM51 <- 04 0e 0a 01 09 10 00 c8 85 f4 f9 2a e4 

#   6.150    [HCI - COM3] SetEventMask(Event_Mask=2305983746702149776L)
#   6.150    COM3 -> 01 01 0c 08 90 88 01 00 00 80 00 20 
send_command("01010c089088010000800020", port = port1)

#   6.338    [HCI - COM3] HCI event received: OPCODE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0xC01 (3073)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.338    COM3 <- 04 0e 04 01 01 0c 00 

#   6.354    [HCI - COM3] SetEventMaskPage2(Event_Mask_Page_2=8388608)
#   6.354    COM3 -> 01 63 0c 08 00 00 80 00 00 00 00 00 
send_command("01630c080000800000000000", port = port1)

#   6.541    [HCI - COM3] HCI event received: OPCODE_SET_EVENT_MASK_PAGE2 (0xE)
#                              Command_Opcode = 0xC63 (3171)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.541    COM3 <- 04 0e 04 01 63 0c 00 

#   6.557    [HCI - COM51] SetEventMask(Event_Mask=2305983746702149776L)
#   6.557    COM51 -> 01 01 0c 08 90 88 01 00 00 80 00 20 
send_command("01010c089088010000800020", port = port0)

#   6.572    [HCI - COM51] HCI event received: OPCODE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0xC01 (3073)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.572    COM51 <- 04 0e 04 01 01 0c 00 

#   6.619    [HCI - COM51] SetEventMaskPage2(Event_Mask_Page_2=8388608)
#   6.619    COM51 -> 01 63 0c 08 00 00 80 00 00 00 00 00 
send_command("01630c080000800000000000", port = port0)

#   6.635    [HCI - COM51] HCI event received: OPCODE_SET_EVENT_MASK_PAGE2 (0xE)
#                              Command_Opcode = 0xC63 (3171)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.635    COM51 <- 04 0e 04 01 63 0c 00 

#   6.682    [HCI - COM51] LeSetEventMask(LE_Event_Mask=536870399)
#   6.682    COM51 -> 01 01 20 08 ff fd ff 1f 00 00 00 00 
send_command("01012008fffdff1f00000000", port = port0)

#   6.697    [HCI - COM51] HCI event received: OPCODE_LE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0x2001 (8193)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.697    COM51 <- 04 0e 04 01 01 20 00 

#   6.775    [HCI - COM3] ReadLocalVerInfo()
#   6.775    COM3 -> 01 01 10 00 
send_command("01011000", port = port1)

#   6.838    [HCI - COM3] HCI event received: OPCODE_READ_LOCAL_VER_INFO (0xE)
#                              Command_Opcode = 0x1001 (4097)
#                              Event_Code = 0xE (14)
#                              HCI_Revision = 0x2303 (8963)
#                              HCI_Version = 0x9 (9)
#                              LMP_Subversion = 0x2303 (8963)
#                              LMP_Version = 0x9 (9)
#                              Manufacturer_Name = 0x5F (95)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0xC (12)
#                              Status = 0x0 (0)
#   6.838    COM3 <- 04 0e 0c 01 01 10 00 09 03 23 09 5f 00 03 23 

#   6.853    [HCI - COM51] ReadLocalVerInfo()
#   6.853    COM51 -> 01 01 10 00 
send_command("01011000", port = port0)

#   6.869    [HCI - COM51] HCI event received: OPCODE_READ_LOCAL_VER_INFO (0xE)
#                              Command_Opcode = 0x1001 (4097)
#                              Event_Code = 0xE (14)
#                              HCI_Revision = 0x0 (0)
#                              HCI_Version = 0xB (11)
#                              LMP_Subversion = 0x0 (0)
#                              LMP_Version = 0xB (11)
#                              Manufacturer_Name = 0x59A (1434)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0xC (12)
#                              Status = 0x0 (0)
#   6.869    COM51 <- 04 0e 0c 01 01 10 00 0b 00 00 0b 9a 05 00 00 

#   6.978    [HCI - COM3] LeReadLocalSupFeat()
#   6.978    COM3 -> 01 03 20 00 
send_command("01032000", port = port1)

#   7.041    [HCI - COM3] HCI event received: OPCODE_LE_READ_LOCAL_SUP_FEAT (0xE)
#                              Command_Opcode = 0x2003 (8195)
#                              Event_Code = 0xE (14)
#                              LE_Features = 0x80179FF (134314495)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0xC (12)
#                              Status = 0x0 (0)
#   7.041    COM3 <- 04 0e 0c 01 03 20 00 ff 79 01 08 00 00 00 00 

#   7.057    [HCI - COM51] LeReadLocalSupFeat()
#   7.057    COM51 -> 01 03 20 00 
send_command("01032000", port = port0)

#   7.072    [HCI - COM51] HCI event received: OPCODE_LE_READ_LOCAL_SUP_FEAT (0xE)
#                              Command_Opcode = 0x2003 (8195)
#                              Event_Code = 0xE (14)
#                              LE_Features = 0x3FFF7FFF (1073709055)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0xC (12)
#                              Status = 0x0 (0)
#   7.072    COM51 <- 04 0e 0c 01 03 20 00 ff 7f ff 3f 00 00 00 00 

#   7.119    [HCI - COM3] LeReadBufSize()
#   7.134    COM3 -> 01 02 20 00 
send_command("01022000", port = port1)

#   7.197    [HCI - COM3] HCI event received: OPCODE_LE_READ_BUF_SIZE (0xE)
#                              Command_Opcode = 0x2002 (8194)
#                              Event_Code = 0xE (14)
#                              HC_LE_ACL_Data_Packet_Length = 0x200 (512)
#                              HC_Total_Num_LE_ACL_Data_Packets = 0x10 (16)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x7 (7)
#                              Status = 0x0 (0)
#   7.197    COM3 <- 04 0e 07 01 02 20 00 00 02 10 

#   7.228    [HCI - COM3] SetEventMask(Event_Mask=2305983746702149776L)
#   7.228    COM3 -> 01 01 0c 08 90 88 01 00 00 80 00 20 
send_command("01010c089088010000800020", port = port1)

#   7.416    [HCI - COM3] HCI event received: OPCODE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0xC01 (3073)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.416    COM3 <- 04 0e 04 01 01 0c 00 

#   7.431    [HCI - COM51] LeSetDefaultPhy(ALL_PHYS=0,
#                        TX_PHYS=1,
#                        RX_PHYS=1)
#   7.431    COM51 -> 01 31 20 03 00 01 01 
send_command("01312003000101", port = port0)

#   7.447    [HCI - COM51] HCI event received: OPCODE_LE_SET_DEF_PHY (0xE)
#                              Command_Opcode = 0x2031 (8241)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.447    COM51 <- 04 0e 04 01 31 20 00 

#   7.494    [HCI - COM3] LeSetEventMask(LE_Event_Mask=61440)
#   7.494    COM3 -> 01 01 20 08 00 f0 00 00 00 00 00 00 
send_command("0101200800F0000000000000", port = port1)

#   7.681    [HCI - COM3] HCI event received: OPCODE_LE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0x2001 (8193)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.681    COM3 <- 04 0e 04 01 01 20 00 

#   7.697    [HCI - COM51] LeSetExtAdvParam(Advertising_Handle=1,
#                         Advertising_Event_Properties=0,
#                         Primary_Advertising_Interval_Min=32,
#                         Primary_Advertising_Interval_Max=16384,
#                         Primary_Advertising_Channel_Map=7,
#                         Own_Address_Type=0,
#                         Peer_Address_Type=0,
#                         Peer_Address='\x00\x00\x00\x00\x00\x00',
#                         Advertising_Filter_Policy=0,
#                         Advertising_Tx_Power=127,
#                         Primary_Advertising_PHY=1,
#                         Secondary_Advertising_Max_Skip=0,
#                         Secondary_Advertising_PHY=1,
#                         Advertising_SID=15,
#                         Scan_Request_Notification_Enable=False)
#   7.697    COM51 -> 01 36 20 19 01 00 00 20 00 00 00 40 00 07 00 00 00 00 00 00 00 00 00 7f 01 00 01 0f 00 
send_command("01362019010000200000004000070000000000000000007f0100010f00", port = port0)

#   7.712    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_PARAM (0xE)
#                              Command_Opcode = 0x2036 (8246)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x5 (5)
#                              Selected_Tx_Power = 0x9 (9)
#                              Status = 0x0 (0)
#   7.712    COM51 <- 04 0e 05 01 36 20 00 09 

#   7.759    [HCI - COM51] LeSetPeriodicAdvParams(Advertising_Handle=1,
#                               Periodic_Advertising_Interval_Min=100,
#                               Periodic_Advertising_Interval_Max=100,
#                               Periodic_Advertising_Properties=0)
#   7.759    COM51 -> 01 3e 20 07 01 64 00 64 00 00 00 
send_command("013e200701640064000000", port = port0)

#   7.775    [HCI - COM51] HCI event received: OPCODE_LE_SET_PER_ADV_PARAM (0xE)
#                              Command_Opcode = 0x203E (8254)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.775    COM51 <- 04 0e 04 01 3e 20 00 

#   7.822    [HCI - COM51] LeSetPeriodicAdvData(Advertising_Handle=1,
#                             Operation=3,
#                             Advertising_Data='')
#   7.822    COM51 -> 01 3f 20 03 01 03 00 
send_command("013f2003010300", port = port0)

#   7.838    [HCI - COM51] HCI event received: OPCODE_LE_SET_PER_ADV_DATA (0xE)
#                              Command_Opcode = 0x203F (8255)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.838    COM51 <- 04 0e 04 01 3f 20 00 

#   7.884    [HCI - COM51] LeSetPeriodicAdvEnable(Enable=True,
#                               Advertising_Handle=1)
#   7.884    COM51 -> 01 40 20 02 01 01 
send_command("014020020101", port = port0)

#   7.900    [HCI - COM51] HCI event received: OPCODE_LE_SET_PER_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2040 (8256)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.900    COM51 <- 04 0e 04 01 40 20 00 

#   7.947    [HCI - COM51] LeSetExtAdvEnable(Enable=True,
#                          Advertising_Handle=1,
#                          Duration=0,
#                          Max_Extended_Advertising_Events=0,
#                          enable_param=[{'Duration': 0,
#                          'Advertising_Handle': 1,
#                          'Max_Extended_Advertising_Events': 0}])
#   7.947    COM51 -> 01 39 20 06 01 01 01 00 00 00 
send_command("01392006010101000000", port = port0)

#   7.962    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.962    COM51 <- 04 0e 04 01 39 20 00 

#   7.978    [HCI - COM51] LeSetExtAdvParam(Advertising_Handle=2,
#                         Advertising_Event_Properties=0,
#                         Primary_Advertising_Interval_Min=32,
#                         Primary_Advertising_Interval_Max=16384,
#                         Primary_Advertising_Channel_Map=7,
#                         Own_Address_Type=0,
#                         Peer_Address_Type=0,
#                         Peer_Address='\x00\x00\x00\x00\x00\x00',
#                         Advertising_Filter_Policy=0,
#                         Advertising_Tx_Power=127,
#                         Primary_Advertising_PHY=1,
#                         Secondary_Advertising_Max_Skip=0,
#                         Secondary_Advertising_PHY=1,
#                         Advertising_SID=14,
#                         Scan_Request_Notification_Enable=False)
#   7.978    COM51 -> 01 36 20 19 02 00 00 20 00 00 00 40 00 07 00 00 00 00 00 00 00 00 00 7f 01 00 01 0e 00 
send_command("01362019020000200000004000070000000000000000007f0100010e00", port = port0)

#   7.994    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_PARAM (0xE)
#                              Command_Opcode = 0x2036 (8246)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x5 (5)
#                              Selected_Tx_Power = 0x9 (9)
#                              Status = 0x0 (0)
#   7.994    COM51 <- 04 0e 05 01 36 20 00 09 

#   8.041    [HCI - COM51] LeSetPeriodicAdvParams(Advertising_Handle=2,
#                               Periodic_Advertising_Interval_Min=100,
#                               Periodic_Advertising_Interval_Max=100,
#                               Periodic_Advertising_Properties=0)
#   8.041    COM51 -> 01 3e 20 07 02 64 00 64 00 00 00 
send_command("013e200702640064000000", port = port0)

#   8.056    [HCI - COM51] HCI event received: OPCODE_LE_SET_PER_ADV_PARAM (0xE)
#                              Command_Opcode = 0x203E (8254)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.056    COM51 <- 04 0e 04 01 3e 20 00 

#   8.119    [HCI - COM51] LeSetPeriodicAdvData(Advertising_Handle=2,
#                             Operation=3,
#                             Advertising_Data='')
#   8.119    COM51 -> 01 3f 20 03 02 03 00 
send_command("013f2003020300", port = port0)

#   8.134    [HCI - COM51] HCI event received: OPCODE_LE_SET_PER_ADV_DATA (0xE)
#                              Command_Opcode = 0x203F (8255)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.134    COM51 <- 04 0e 04 01 3f 20 00 

#   8.197    [HCI - COM51] LeSetPeriodicAdvEnable(Enable=True,
#                               Advertising_Handle=2)
#   8.197    COM51 -> 01 40 20 02 01 02 
send_command("014020020102", port = port0)

#   8.213    [HCI - COM51] HCI event received: OPCODE_LE_SET_PER_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2040 (8256)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.213    COM51 <- 04 0e 04 01 40 20 00 

#   8.259    [HCI - COM51] LeSetExtAdvEnable(Enable=True,
#                          Advertising_Handle=2,
#                          Duration=0,
#                          Max_Extended_Advertising_Events=0,
#                          enable_param=[{'Duration': 0,
#                          'Advertising_Handle': 2,
#                          'Max_Extended_Advertising_Events': 0}])
#   8.259    COM51 -> 01 39 20 06 01 01 02 00 00 00 
send_command("01392006010102000000", port = port0)

#   8.291    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.291    COM51 <- 04 0e 04 01 39 20 00 

#   8.337    [HCI - COM51] LeSetExtAdvParam(Advertising_Handle=3,
#                         Advertising_Event_Properties=0,
#                         Primary_Advertising_Interval_Min=32,
#                         Primary_Advertising_Interval_Max=16384,
#                         Primary_Advertising_Channel_Map=7,
#                         Own_Address_Type=0,
#                         Peer_Address_Type=0,
#                         Peer_Address='\x00\x00\x00\x00\x00\x00',
#                         Advertising_Filter_Policy=0,
#                         Advertising_Tx_Power=127,
#                         Primary_Advertising_PHY=1,
#                         Secondary_Advertising_Max_Skip=0,
#                         Secondary_Advertising_PHY=1,
#                         Advertising_SID=15,
#                         Scan_Request_Notification_Enable=False)
#   8.337    COM51 -> 01 36 20 19 03 00 00 20 00 00 00 40 00 07 00 00 00 00 00 00 00 00 00 7f 01 00 01 0f 00 
send_command("01362019030000200000004000070000000000000000007f0100010f00", port = port0)

#   8.353    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_PARAM (0xE)
#                              Command_Opcode = 0x2036 (8246)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x5 (5)
#                              Selected_Tx_Power = 0x9 (9)
#                              Status = 0x0 (0)
#   8.353    COM51 <- 04 0e 05 01 36 20 00 09 

#   8.415    [HCI - COM51] LeSetPeriodicAdvParams(Advertising_Handle=3,
#                               Periodic_Advertising_Interval_Min=100,
#                               Periodic_Advertising_Interval_Max=100,
#                               Periodic_Advertising_Properties=0)
#   8.415    COM51 -> 01 3e 20 07 03 64 00 64 00 00 00 
send_command("013e200703640064000000", port = port0)

#   8.431    [HCI - COM51] HCI event received: OPCODE_LE_SET_PER_ADV_PARAM (0xE)
#                              Command_Opcode = 0x203E (8254)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.431    COM51 <- 04 0e 04 01 3e 20 00 

#   8.478    [HCI - COM51] LeSetPeriodicAdvData(Advertising_Handle=3,
#                             Operation=3,
#                             Advertising_Data='')
#   8.478    COM51 -> 01 3f 20 03 03 03 00 
send_command("013f2003030300", port = port0)

#   8.494    [HCI - COM51] HCI event received: OPCODE_LE_SET_PER_ADV_DATA (0xE)
#                              Command_Opcode = 0x203F (8255)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.494    COM51 <- 04 0e 04 01 3f 20 00 

#   8.541    [HCI - COM51] LeSetPeriodicAdvEnable(Enable=True,
#                               Advertising_Handle=3)
#   8.541    COM51 -> 01 40 20 02 01 03 
send_command("014020020103", port = port0)

#   8.556    [HCI - COM51] HCI event received: OPCODE_LE_SET_PER_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2040 (8256)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.556    COM51 <- 04 0e 04 01 40 20 00 

#   8.603    [HCI - COM51] LeSetExtAdvEnable(Enable=True,
#                          Advertising_Handle=3,
#                          Duration=0,
#                          Max_Extended_Advertising_Events=0,
#                          enable_param=[{'Duration': 0,
#                          'Advertising_Handle': 3,
#                          'Max_Extended_Advertising_Events': 0}])
#   8.603    COM51 -> 01 39 20 06 01 01 03 00 00 00 
send_command("01392006010103000000", port = port0)

#   8.619    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.619    COM51 <- 04 0e 04 01 39 20 00 

#   8.665    [HCI - COM3] LeSetExtScanEnable(Enable=True,
#                          Filter_Duplicates=0,
#                          Duration=0,
#                          Period=0)
#   8.665    COM3 -> 01 42 20 06 01 00 00 00 00 00 
send_command("01422006010000000000", port = port1)

#   8.853    [HCI - COM3] LePeriodicAdvCreateSync(Options=0,
#                               Advertising_SID=15,
#                               Advertising_Address_Type=0,
#                               Advertiser_Address='\xc8\x85\xf4\xf9*\xe4',
#                               Skip=0,
#                               Sync_Timeout=37.5,
#                               CTE_Type=0)
#   8.853    COM3 -> 01 44 20 0e 00 0f 00 c8 85 f4 f9 2a e4 00 00 25 00 00 
send_command("0144200e000f00c885f4f92ae40000250000", port = port1)


# TODO: Delay until Sync established

read_events(seconds = 3, port = port1)



#   9.134    [HCI - COM3] HCI event received: EVT_CMD_STATUS (0xF)
#                              Command_Opcode = 0x2044 (8260)
#                              Event_Code = 0xF (15)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   9.134    COM3 <- 04 0f 04 00 01 44 20 

#   9.522    [HCI - COM3] HCI event received: SUBEVT_LE_PER_ADV_SYNC_EST_EVT (0x3E)
#                              Address = 0xE42AF9F485C8 (250873233311176)
#                              Advertiser_Address_Type = 0x0 (0)
#                              Advertiser_Clock_Accuracy = 0x7 (7)
#                              Advertiser_PHY = 0x1 (1)
#                              Advertising_Interval = 0x64 (100)
#                              Advertising_SID = 0xF (15)
#                              Event_Code = 0x3E (62)
#                              Parameter_Length = 0x10 (16)
#                              Status = 0x0 (0)
#                              Subevent_Code = 0xE (14)
#                              Sync_Handle = 0x0 (0)
#   9.522    COM3 <- 04 3e 10 0e 00 00 00 0f 00 c8 85 f4 f9 2a e4 01 64 00 07 

#   9.538    [HCI - COM3] LePeriodicAdvCreateSync(Options=0,
#                               Advertising_SID=15,
#                               Advertising_Address_Type=0,
#                               Advertiser_Address='\xc8\x85\xf4\xf9*\xe4',
#                               Skip=0,
#                               Sync_Timeout=37.5,
#                               CTE_Type=0)
#   9.538    COM3 -> 01 44 20 0e 00 0f 00 c8 85 f4 f9 2a e4 00 00 25 00 00 
send_command("0144200e000f00c885f4f92ae40000250000", port = port1)

# TODO: Delay to check return code == 0xB
read_events(seconds = 3, port = port1)

#   9.819    [HCI - COM3] HCI event received: EVT_CMD_STATUS (0xF)
#                              Command_Opcode = 0x2044 (8260)
#                              Event_Code = 0xF (15)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0xB (11)
#   9.819    COM3 <- 04 0f 04 0b 01 44 20 

#   9.835    [HCI - COM3] LePeriodicAdvCreateSync(Options=0,
#                               Advertising_SID=14,
#                               Advertising_Address_Type=0,
#                               Advertiser_Address='\xc8\x85\xf4\xf9*\xe4',
#                               Skip=0,
#                               Sync_Timeout=37.5,
#                               CTE_Type=0)
#   9.835    COM3 -> 01 44 20 0e 00 0e 00 c8 85 f4 f9 2a e4 00 00 25 00 00 
send_command("0144200e000e00c885f4f92ae40000250000", port = port1)

# TODO: Delay to view the sync
read_events(seconds = 10, port = port1)

#  19.331    [HCI - COM3] HCI event received: SUBEVT_LE_PER_ADV_SYNC_EST_EVT (0x3E)
#                              Address = 0xE42AF9F485C8 (250873233311176)
#                              Advertiser_Address_Type = 0x0 (0)
#                              Advertiser_Clock_Accuracy = 0x7 (7)
#                              Advertiser_PHY = 0x1 (1)
#                              Advertising_Interval = 0x64 (100)
#                              Advertising_SID = 0xE (14)
#                              Event_Code = 0x3E (62)
#                              Parameter_Length = 0x10 (16)
#                              Status = 0x0 (0)
#                              Subevent_Code = 0xE (14)
#                              Sync_Handle = 0x1 (1)
#  19.331    COM3 <- 04 3e 10 0e 00 01 00 0e 00 c8 85 f4 f9 2a e4 01 64 00 07 

#  19.347    [HCI - COM51] LeSetPeriodicAdvEnable(Enable=False,
#                               Advertising_Handle=1)
#  19.347    COM51 -> 01 40 20 02 00 01 
send_command("014020020001", port = port0)

#  19.363    [HCI - COM51] HCI event received: OPCODE_LE_SET_PER_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2040 (8256)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  19.363    COM3 <- 04 3e 1a 0d 01 00 00 00 c8 85 f4 f9 2a e4 01 01 0e 7f cf 64 00 00 00 00 00 00 00 00 00 

#  19.378    [HCI - COM3] LePeriodicAdvCreateSync(Options=0,
#                               Advertising_SID=15,
#                               Advertising_Address_Type=0,
#                               Advertiser_Address='\xc8\x85\xf4\xf9*\xe4',
#                               Skip=0,
#                               Sync_Timeout=37.5,
#                               CTE_Type=0)
#  19.378    COM3 -> 01 44 20 0e 00 0f 00 c8 85 f4 f9 2a e4 00 00 25 00 00 
send_command("0144200e000f00c885f4f92ae40000250000", port = port1)

# TODO: Delay to check the events received
read_events(seconds = 10, port=port1)

#  19.659    [HCI - COM3] HCI event received: SUBEVT_LE_PER_ADV_SYNC_LOST_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Parameter_Length = 0x3 (3)
#                              Subevent_Code = 0x10 (16)
#                              Sync_Handle = 0x0 (0)
#  19.659    COM3 <- 04 3e 03 10 00 00 

#  19.659    [HCI - COM3] HCI event received: EVT_CMD_STATUS (0xF)
#                              Command_Opcode = 0x2044 (8260)
#                              Event_Code = 0xF (15)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  19.659    COM3 <- 04 0f 04 00 01 44 20 

#  19.675    [HCI - COM3] Reset()
#  19.675    COM3 -> 01 03 0c 00 
send_command("01030c00", port = port1)

#  19.722    [HCI - COM3] HCI event received: SUBEVT_LE_PER_ADV_REPORT_EVT (0x3E)
#                              CTE_Type = 0xFF (255)
#                              Data = ""
#                              Data_Length = 0x0 (0)
#                              Data_Status = 0x0 (0)
#                              Event_Code = 0x3E (62)
#                              Parameter_Length = 0x8 (8)
#                              RSSI = 0xCD (205)
#                              Subevent_Code = 0xF (15)
#                              Sync_Handle = 0x1 (1)
#                              Tx_Power = 0x0 (0)
#  19.722    COM3 <- 04 3e 08 0f 01 00 00 cd ff 00 00 

#  19.737    [HCI - COM3] HCI event received: OPCODE_RESET (0xE)
#                              Command_Opcode = 0xC03 (3075)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  19.737    COM3 <- 04 0e 04 01 03 0c 00 

#  19.753    [HCI - COM51] Reset()
#  19.753    COM51 -> 01 03 0c 00 
send_command("01030c00", port = port0)

