#! /usr/bin/env python

filename="../common.py"
exec(open(filename).read())

#   4.937    [HCI - COM51] Reset()
#   4.937    COM51 -> 01 03 0c 00 
send_command("01030c00")

#   4.969    [HCI - COM51] HCI event received: OPCODE_RESET (0xE)
#                              Command_Opcode = 0xC03 (3075)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   4.969    COM51 <- 04 0e 04 01 03 0c 00 

#   5.141    [HCI - COM51] Reset()
#   5.141    COM51 -> 01 03 0c 00 
send_command("01030c00")


#   5.719    [HCI - COM51] HCI event received: <unknown> (0xE)
#                              Command_Opcode = 0xFC00 (64512)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Signature = "(155, 244, 1, 102, 162, 245, 56, 72, 110, 106, 248, 103, 220, 22, 246, 91, 176, 33, 181, 216, 249, 141, 153, 42, 252, 187, 203, 210, 218, 86, 223, 251, 38, 0, 136, 70, 133, 1, 254, 124, 28, 249, 132, 201, 18, 183, 142, 254, 210, 150, 19, 126, 252, 146, 206, 7, 74, 179, 93, 47, 65, 132, 193, 47)"
#                              Status = 0x0 (0)
#   5.719    COM51 <- 04 0e 44 01 00 fc 00 9b f4 01 66 a2 f5 38 48 6e 6a f8 67 dc 16 f6 5b b0 21 b5 d8 f9 8d 99 2a fc bb cb d2 da 56 df fb 26 00 88 46 85 01 fe 7c 1c f9 84 c9 12 b7 8e fe d2 96 13 7e fc 92 ce 07 4a b3 5d 2f 41 84 c1 2f 

#   5.875    [HCI - COM51] VsSetBdAddr(addr='\xc8\x85\xf4\xf9*\xe4')
#   5.875    COM51 -> 01 f0 ff 06 c8 85 f4 f9 2a e4 
send_command("01f0ff06c885f4f92ae4")

#   5.891    [HCI - COM51] HCI event received: OPCODE_VS_SET_BD_ADDR (0xE)
#                              Command_Opcode = 0xFFF0 (65520)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Status = 0x0 (0)
#   5.891    COM51 <- 04 0e 04 01 f0 ff 00 

#   6.032    [HCI - COM51] ReadBdAddr()
#   6.032    COM51 -> 01 09 10 00 
send_command("01091000")

#   6.047    [HCI - COM51] HCI event received: OPCODE_READ_BD_ADDR (0xE)
#                              BD_ADDR = 0xE42AF9F485C8 (250873233311176)
#                              Command_Opcode = 0x1009 (4105)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0xA (10)
#                              Status = 0x0 (0)
#   6.047    COM51 <- 04 0e 0a 01 09 10 00 c8 85 f4 f9 2a e4 

#   6.531    [HCI - COM51] SetEventMask(Event_Mask=2305983746702149776L)
#   6.531    COM51 -> 01 01 0c 08 90 88 01 00 00 80 00 20 
send_command("01010c089088010000800020")

#   6.547    [HCI - COM51] HCI event received: OPCODE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0xC01 (3073)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.547    COM51 <- 04 0e 04 01 01 0c 00 

#   6.594    [HCI - COM51] SetEventMaskPage2(Event_Mask_Page_2=8388608)
#   6.594    COM51 -> 01 63 0c 08 00 00 80 00 00 00 00 00 
send_command("01630c080000800000000000")

#   6.610    [HCI - COM51] HCI event received: OPCODE_SET_EVENT_MASK_PAGE2 (0xE)
#                              Command_Opcode = 0xC63 (3171)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.610    COM51 <- 04 0e 04 01 63 0c 00 

#   6.657    [HCI - COM51] LeSetEventMask(LE_Event_Mask=536870399)
#   6.657    COM51 -> 01 01 20 08 ff fd ff 1f 00 00 00 00 
send_command("01012008fffdff1f00000000")

#   6.673    [HCI - COM51] HCI event received: OPCODE_LE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0x2001 (8193)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.673    COM51 <- 04 0e 04 01 01 20 00 

#   6.735    [HCI - COM51] VsSetEventMask(eventMask=0)
#   6.735    COM51 -> 01 e1 ff 08 00 00 00 00 00 00 00 00 
send_command("01e1ff080000000000000000")

#   6.751    [HCI - COM51] HCI event received: OPCODE_VS_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0xFFE1 (65505)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Status = 0x0 (0)
#   6.751    COM51 <- 04 0e 04 01 e1 ff 00 

#   7.406    [HCI - COM51] LeSetDefaultPhy(ALL_PHYS=0,
#                        TX_PHYS=1,
#                        RX_PHYS=1)
#   7.406    COM51 -> 01 31 20 03 00 01 01 
send_command("01312003000101")

#   7.422    [HCI - COM51] HCI event received: OPCODE_LE_SET_DEF_PHY (0xE)
#                              Command_Opcode = 0x2031 (8241)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.422    COM51 <- 04 0e 04 01 31 20 00 

#   7.704    [HCI - COM51] LeSetExtAdvParam(Advertising_Handle=1,
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
#   7.704    COM51 -> 01 36 20 19 01 00 00 20 00 00 00 40 00 07 00 00 00 00 00 00 00 00 00 7f 01 00 01 0f 00 
send_command("01362019010000200000004000070000000000000000007f0100010f00")

#   7.719    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_PARAM (0xE)
#                              Command_Opcode = 0x2036 (8246)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x5 (5)
#                              Selected_Tx_Power = 0x9 (9)
#                              Status = 0x0 (0)
#   7.719    COM51 <- 04 0e 05 01 36 20 00 09 

#   7.766    [HCI - COM51] LeSetPeriodicAdvParams(Advertising_Handle=1,
#                               Periodic_Advertising_Interval_Min=100,
#                               Periodic_Advertising_Interval_Max=100,
#                               Periodic_Advertising_Properties=0)
#   7.766    COM51 -> 01 3e 20 07 01 64 00 64 00 00 00 
send_command("013e200701640064000000")

#   7.782    [HCI - COM51] HCI event received: OPCODE_LE_SET_PER_ADV_PARAM (0xE)
#                              Command_Opcode = 0x203E (8254)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.782    COM51 <- 04 0e 04 01 3e 20 00 

#   7.829    [HCI - COM51] LeSetPeriodicAdvData(Advertising_Handle=1,
#                             Operation=3,
#                             Advertising_Data='')
#   7.829    COM51 -> 01 3f 20 03 01 03 00 
send_command("013f2003010300")

#   7.844    [HCI - COM51] HCI event received: OPCODE_LE_SET_PER_ADV_DATA (0xE)
#                              Command_Opcode = 0x203F (8255)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.844    COM51 <- 04 0e 04 01 3f 20 00 

#   7.891    [HCI - COM51] LeSetPeriodicAdvEnable(Enable=True,
#                               Advertising_Handle=1)
#   7.891    COM51 -> 01 40 20 02 01 01 
send_command("014020020101")

#   7.907    [HCI - COM51] HCI event received: OPCODE_LE_SET_PER_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2040 (8256)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.907    COM51 <- 04 0e 04 01 40 20 00 

#   7.969    [HCI - COM51] LeSetExtAdvEnable(Enable=True,
#                          Advertising_Handle=1,
#                          Duration=0,
#                          Max_Extended_Advertising_Events=0,
#                          enable_param=[{'Duration': 0,
#                          'Advertising_Handle': 1,
#                          'Max_Extended_Advertising_Events': 0}])
#   7.969    COM51 -> 01 39 20 06 01 01 01 00 00 00 
send_command("01392006010101000000")

#   7.985    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.985    COM51 <- 04 0e 04 01 39 20 00 

#   8.047    [HCI - COM51] LeSetExtAdvParam(Advertising_Handle=2,
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
#   8.047    COM51 -> 01 36 20 19 02 00 00 20 00 00 00 40 00 07 00 00 00 00 00 00 00 00 00 7f 01 00 01 0e 00 
send_command("01362019020000200000004000070000000000000000007f0100010e00")

#   8.063    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_PARAM (0xE)
#                              Command_Opcode = 0x2036 (8246)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x5 (5)
#                              Selected_Tx_Power = 0x9 (9)
#                              Status = 0x0 (0)
#   8.063    COM51 <- 04 0e 05 01 36 20 00 09 

#   8.110    [HCI - COM51] LeSetPeriodicAdvParams(Advertising_Handle=2,
#                               Periodic_Advertising_Interval_Min=100,
#                               Periodic_Advertising_Interval_Max=100,
#                               Periodic_Advertising_Properties=0)
#   8.110    COM51 -> 01 3e 20 07 02 64 00 64 00 00 00 
send_command("013e200702640064000000")

#   8.125    [HCI - COM51] HCI event received: OPCODE_LE_SET_PER_ADV_PARAM (0xE)
#                              Command_Opcode = 0x203E (8254)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.125    COM51 <- 04 0e 04 01 3e 20 00 

#   8.173    [HCI - COM51] LeSetPeriodicAdvData(Advertising_Handle=2,
#                             Operation=3,
#                             Advertising_Data='')
#   8.173    COM51 -> 01 3f 20 03 02 03 00 
send_command("013f2003020300")

#   8.188    [HCI - COM51] HCI event received: OPCODE_LE_SET_PER_ADV_DATA (0xE)
#                              Command_Opcode = 0x203F (8255)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.188    COM51 <- 04 0e 04 01 3f 20 00 

#   8.235    [HCI - COM51] LeSetPeriodicAdvEnable(Enable=True,
#                               Advertising_Handle=2)
#   8.235    COM51 -> 01 40 20 02 01 02 
send_command("014020020102")

#   8.250    [HCI - COM51] HCI event received: OPCODE_LE_SET_PER_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2040 (8256)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.250    COM51 <- 04 0e 04 01 40 20 00 

#   8.298    [HCI - COM51] LeSetExtAdvEnable(Enable=True,
#                          Advertising_Handle=2,
#                          Duration=0,
#                          Max_Extended_Advertising_Events=0,
#                          enable_param=[{'Duration': 0,
#                          'Advertising_Handle': 2,
#                          'Max_Extended_Advertising_Events': 0}])
#   8.298    COM51 -> 01 39 20 06 01 01 02 00 00 00 
send_command("01392006010102000000")

#   8.313    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.313    COM51 <- 04 0e 04 01 39 20 00 

#   8.375    [HCI - COM51] LeSetExtAdvParam(Advertising_Handle=3,
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
#   8.375    COM51 -> 01 36 20 19 03 00 00 20 00 00 00 40 00 07 00 00 00 00 00 00 00 00 00 7f 01 00 01 0f 00 
send_command("01362019030000200000004000070000000000000000007f0100010f00")

#   8.391    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_PARAM (0xE)
#                              Command_Opcode = 0x2036 (8246)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x5 (5)
#                              Selected_Tx_Power = 0x9 (9)
#                              Status = 0x0 (0)
#   8.391    COM51 <- 04 0e 05 01 36 20 00 09 

#   8.438    [HCI - COM51] LeSetPeriodicAdvParams(Advertising_Handle=3,
#                               Periodic_Advertising_Interval_Min=100,
#                               Periodic_Advertising_Interval_Max=100,
#                               Periodic_Advertising_Properties=0)
#   8.438    COM51 -> 01 3e 20 07 03 64 00 64 00 00 00 
send_command("013e200703640064000000")

#   8.454    [HCI - COM51] HCI event received: OPCODE_LE_SET_PER_ADV_PARAM (0xE)
#                              Command_Opcode = 0x203E (8254)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.454    COM51 <- 04 0e 04 01 3e 20 00 

#   8.500    [HCI - COM51] LeSetPeriodicAdvData(Advertising_Handle=3,
#                             Operation=3,
#                             Advertising_Data='')
#   8.500    COM51 -> 01 3f 20 03 03 03 00 
send_command("013f2003030300")

#   8.516    [HCI - COM51] HCI event received: OPCODE_LE_SET_PER_ADV_DATA (0xE)
#                              Command_Opcode = 0x203F (8255)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.516    COM51 <- 04 0e 04 01 3f 20 00 

#   8.563    [HCI - COM51] LeSetPeriodicAdvEnable(Enable=True,
#                               Advertising_Handle=3)
#   8.563    COM51 -> 01 40 20 02 01 03 
send_command("014020020103")

#   8.579    [HCI - COM51] HCI event received: OPCODE_LE_SET_PER_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2040 (8256)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.579    COM51 <- 04 0e 04 01 40 20 00 

#   8.594    [HCI - COM51] LeSetExtAdvEnable(Enable=True,
#                          Advertising_Handle=3,
#                          Duration=0,
#                          Max_Extended_Advertising_Events=0,
#                          enable_param=[{'Duration': 0,
#                          'Advertising_Handle': 3,
#                          'Max_Extended_Advertising_Events': 0}])
#   8.594    COM51 -> 01 39 20 06 01 01 03 00 00 00 
send_command("01392006010103000000")

#   8.610    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.610    COM51 <- 04 0e 04 01 39 20 00 

while(1):
  rcv_event()

