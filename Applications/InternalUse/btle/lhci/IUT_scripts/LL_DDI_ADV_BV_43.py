#! /usr/bin/env python

filename="../common.py"
exec(open(filename).read())

#   5.000    [HCI - COM4] Reset()
#   5.000    COM4 -> 01 03 0c 00 
send_command("01030c00")

#   5.063    [HCI - COM4] HCI event received: OPCODE_RESET (0xE)
#                              Command_Opcode = 0xC03 (3075)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   5.063    COM4 <- 04 0e 04 01 03 0c 00 

#   5.078    [HCI - COM4] Reset()
#   5.078    COM4 -> 01 03 0c 00 
send_command("01030c00")

#   5.140    [HCI - COM4] HCI event received: OPCODE_RESET (0xE)
#                              Command_Opcode = 0xC03 (3075)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   5.140    COM4 <- 04 0e 04 01 03 0c 00 

#   5.985    [HCI - COM4] ReadBdAddr()
#   5.985    COM4 -> 01 09 10 00 
send_command("01091000")

#   6.047    [HCI - COM4] HCI event received: OPCODE_READ_BD_ADDR (0xE)
#                              BD_ADDR = 0x5F123456 (1595028566)
#                              Command_Opcode = 0x1009 (4105)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0xA (10)
#                              Status = 0x0 (0)
#   6.047    COM4 <- 04 0e 0a 01 09 10 00 56 34 12 5f 00 00 

#   6.141    [HCI - COM4] SetEventMask(Event_Mask=2305983746702149776L)
#   6.141    COM4 -> 01 01 0c 08 90 88 01 00 00 80 00 20 
send_command("01010c089088010000800020")

#   6.328    [HCI - COM4] HCI event received: OPCODE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0xC01 (3073)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.328    COM4 <- 04 0e 04 01 01 0c 00 

#   6.344    [HCI - COM4] SetEventMaskPage2(Event_Mask_Page_2=8388608)
#   6.344    COM4 -> 01 63 0c 08 00 00 80 00 00 00 00 00 
send_command("01630c080000800000000000")

#   6.531    [HCI - COM4] HCI event received: OPCODE_SET_EVENT_MASK_PAGE2 (0xE)
#                              Command_Opcode = 0xC63 (3171)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.531    COM4 <- 04 0e 04 01 63 0c 00 

#   6.812    [HCI - COM4] ReadLocalVerInfo()
#   6.812    COM4 -> 01 01 10 00 
send_command("01011000")

#   6.875    [HCI - COM4] HCI event received: OPCODE_READ_LOCAL_VER_INFO (0xE)
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
#   6.875    COM4 <- 04 0e 0c 01 01 10 00 09 03 23 09 5f 00 03 23 

#   7.047    [HCI - COM4] LeReadLocalSupFeat()
#   7.047    COM4 -> 01 03 20 00 
send_command("01032000")

#   7.110    [HCI - COM4] HCI event received: OPCODE_LE_READ_LOCAL_SUP_FEAT (0xE)
#                              Command_Opcode = 0x2003 (8195)
#                              Event_Code = 0xE (14)
#                              LE_Features = 0x80179FF (134314495)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0xC (12)
#                              Status = 0x0 (0)
#   7.110    COM4 <- 04 0e 0c 01 03 20 00 ff 79 01 08 00 00 00 00 

#   7.187    [HCI - COM4] LeReadBufSize()
#   7.187    COM4 -> 01 02 20 00 
send_command("01022000")

#   7.250    [HCI - COM4] HCI event received: OPCODE_LE_READ_BUF_SIZE (0xE)
#                              Command_Opcode = 0x2002 (8194)
#                              Event_Code = 0xE (14)
#                              HC_LE_ACL_Data_Packet_Length = 0x200 (512)
#                              HC_Total_Num_LE_ACL_Data_Packets = 0x10 (16)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x7 (7)
#                              Status = 0x0 (0)
#   7.250    COM4 <- 04 0e 07 01 02 20 00 00 02 10 

#   7.266    [HCI - COM4] SetEventMask(Event_Mask=2305983746702149776L)
#   7.266    COM4 -> 01 01 0c 08 90 88 01 00 00 80 00 20 
send_command("01010c089088010000800020")

#   7.453    [HCI - COM4] HCI event received: OPCODE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0xC01 (3073)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.453    COM4 <- 04 0e 04 01 01 0c 00 

#   7.500    [HCI - COM4] LeSetEventMask(LE_Event_Mask=536870399)
#   7.500    COM4 -> 01 01 20 08 ff fd ff 1f 00 00 00 00 
send_command("01012008fffdff1f00000000")

#   7.687    [HCI - COM4] HCI event received: OPCODE_LE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0x2001 (8193)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.687    COM4 <- 04 0e 04 01 01 20 00 

#   9.704    [HCI - COM4] LeSetExtAdvParam(Advertising_Handle=15,
#                        Advertising_Event_Properties=0,
#                        Primary_Advertising_Interval_Min=64,
#                        Primary_Advertising_Interval_Max=64,
#                        Primary_Advertising_Channel_Map=7,
#                        Own_Address_Type=0,
#                        Peer_Address_Type=0,
#                        Peer_Address='\xc8\x85\xf4\xf9*\xe4',
#                        Advertising_Filter_Policy=0,
#                        Advertising_Tx_Power=127,
#                        Primary_Advertising_PHY=1,
#                        Secondary_Advertising_Max_Skip=0,
#                        Secondary_Advertising_PHY=1,
#                        Advertising_SID=15,
#                        Scan_Request_Notification_Enable=0)
#   9.704    COM4 -> 01 36 20 19 0f 00 00 40 00 00 40 00 00 07 00 00 c8 85 f4 f9 2a e4 00 7f 01 00 01 0f 00 
send_command("013620190f0000400000400000070000c885f4f92ae4007f0100010f00")

#  10.156    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_PARAM (0xE)
#                              Command_Opcode = 0x2036 (8246)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x5 (5)
#                              Selected_Tx_Power = 0x0 (0)
#                              Status = 0x0 (0)
#  10.156    COM4 <- 04 0e 05 01 36 20 00 00 

#  10.172    [HCI - COM4] LeSetPeriodicAdvParams(Advertising_Handle=15,
#                              Periodic_Advertising_Interval_Min=40,
#                              Periodic_Advertising_Interval_Max=60,
#                              Periodic_Advertising_Properties=0)
#  10.172    COM4 -> 01 3e 20 07 0f 28 00 3c 00 00 00 
send_command("013e20070f28003c000000")

#  10.344    [HCI - COM4] HCI event received: OPCODE_LE_SET_PER_ADV_PARAM (0xE)
#                              Command_Opcode = 0x203E (8254)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  10.344    COM4 <- 04 0e 04 01 3e 20 00 

#  10.360    [HCI - COM4] LeSetPeriodicAdvData(Advertising_Handle=1,
#                            Operation=3,
#                            Advertising_Data='\x00')
#  10.360    COM4 -> 01 3f 20 04 01 03 01 00 
send_command("013f200401030100")

#  10.485    [HCI - COM4] HCI event received: OPCODE_LE_SET_PER_ADV_DATA (0xE)
#                              Command_Opcode = 0x203F (8255)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x42 (66)
#  10.485    COM4 <- 04 0e 04 01 3f 20 42 

#  10.516    [HCI - COM4] LeSetPeriodicAdvEnable(Enable=True,
#                              Advertising_Handle=15)
#  10.516    COM4 -> 01 40 20 02 01 0f 
send_command("01402002010f")

#  10.609    [HCI - COM4] HCI event received: OPCODE_LE_SET_PER_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2040 (8256)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  10.609    COM4 <- 04 0e 04 01 40 20 00 

#  10.625    [HCI - COM4] LeSetExtAdvEnable(Enable=True,
#                         Advertising_Handle=15,
#                         Duration=0,
#                         Max_Extended_Advertising_Events=0,
#                         enable_param=[{'Duration': 0,
#                         'Advertising_Handle': 15,
#                         'Max_Extended_Advertising_Events': 0}])
#  10.625    COM4 -> 01 39 20 06 01 01 0f 00 00 00 
send_command("0139200601010f000000")

#  10.782    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  10.782    COM4 <- 04 0e 04 01 39 20 00 

#  11.203    [HCI - COM4] LeSetExtAdvEnable(Enable=False,
#                         Advertising_Handle=15,
#                         Duration=0,
#                         Max_Extended_Advertising_Events=0,
#                         enable_param=[{'Duration': 0,
#                         'Advertising_Handle': 15,
#                         'Max_Extended_Advertising_Events': 0}])
#  11.203    COM4 -> 01 39 20 06 00 01 0f 00 00 00 
send_command("0139200600010f000000")

#  11.375    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  11.375    COM4 <- 04 0e 04 01 39 20 00 

#  11.398    [HCI - COM4] LeRemoveAdvSet(Advertising_Handle=15)
#  11.398    COM4 -> 01 3c 20 01 0f 
send_command("013c20010f")

#  11.469    [HCI - COM4] HCI event received: OPCODE_LE_REMOVE_ADV_SET (0xE)
#                              Command_Opcode = 0x203C (8252)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  11.469    COM4 <- 04 0e 04 01 3c 20 00 

#  11.484    [HCI - COM4] LeSetPeriodicAdvEnable(Enable=False,
#                              Advertising_Handle=15)
#  11.484    COM4 -> 01 40 20 02 00 0f 
send_command("01402002000f")

#  11.578    [HCI - COM4] HCI event received: OPCODE_LE_SET_PER_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2040 (8256)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x42 (66)
#  11.578    COM4 <- 04 0e 04 01 40 20 42 


while(1):
  rcv_event()
