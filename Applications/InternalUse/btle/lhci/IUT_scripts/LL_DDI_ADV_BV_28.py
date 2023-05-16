#! /usr/bin/env python


filename="../common.py"
exec(open(filename).read())


#   5.017    [HCI - COM4] Reset()
#   5.017    COM4 -> 01 03 0c 00 
send_command("01030c00")

#   5.080    [HCI - COM4] HCI event received: OPCODE_RESET (0xE)
#                              Command_Opcode = 0xC03 (3075)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   5.080    COM4 <- 04 0e 04 01 03 0c 00 

#   5.095    [HCI - COM4] Reset()
#   5.095    COM4 -> 01 03 0c 00 
send_command("01030c00")

#   5.158    [HCI - COM4] HCI event received: OPCODE_RESET (0xE)
#                              Command_Opcode = 0xC03 (3075)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   5.158    COM4 <- 04 0e 04 01 03 0c 00 

#   6.033    [HCI - COM4] ReadBdAddr()
#   6.033    COM4 -> 01 09 10 00 
send_command("01091000")

#   6.095    [HCI - COM4] HCI event received: OPCODE_READ_BD_ADDR (0xE)
#                              BD_ADDR = 0x5F123456 (1595028566)
#                              Command_Opcode = 0x1009 (4105)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0xA (10)
#                              Status = 0x0 (0)
#   6.095    COM4 <- 04 0e 0a 01 09 10 00 56 34 12 5f 00 00 

#   6.173    [HCI - COM4] SetEventMask(Event_Mask=2305983746702149776L)
#   6.173    COM4 -> 01 01 0c 08 90 88 01 00 00 80 00 20 
send_command("01010c089088010000800020")

#   6.376    [HCI - COM4] HCI event received: OPCODE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0xC01 (3073)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.376    COM4 <- 04 0e 04 01 01 0c 00 

#   6.392    [HCI - COM4] SetEventMaskPage2(Event_Mask_Page_2=8388608)
#   6.392    COM4 -> 01 63 0c 08 00 00 80 00 00 00 00 00 
send_command("01630c080000800000000000")

#   6.580    [HCI - COM4] HCI event received: OPCODE_SET_EVENT_MASK_PAGE2 (0xE)
#                              Command_Opcode = 0xC63 (3171)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.580    COM4 <- 04 0e 04 01 63 0c 00 

#   6.861    [HCI - COM4] ReadLocalVerInfo()
#   6.861    COM4 -> 01 01 10 00 
send_command("01011000")

#   6.923    [HCI - COM4] HCI event received: OPCODE_READ_LOCAL_VER_INFO (0xE)
#                              Command_Opcode = 0x1001 (4097)
#                              Event_Code = 0xE (14)
#                              HCI_Revision = 0x2303 (8963)
#                              HCI_Version = 0xA (10)
#                              LMP_Subversion = 0x2303 (8963)
#                              LMP_Version = 0xA (10)
#                              Manufacturer_Name = 0x5F (95)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0xC (12)
#                              Status = 0x0 (0)
#   6.923    COM4 <- 04 0e 0c 01 01 10 00 0a 03 23 0a 5f 00 03 23 

#   7.079    [HCI - COM4] LeReadLocalSupFeat()
#   7.079    COM4 -> 01 03 20 00 
send_command("01032000")

#   7.142    [HCI - COM4] HCI event received: OPCODE_LE_READ_LOCAL_SUP_FEAT (0xE)
#                              Command_Opcode = 0x2003 (8195)
#                              Event_Code = 0xE (14)
#                              LE_Features = 0x80179FF (134314495)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0xC (12)
#                              Status = 0x0 (0)
#   7.142    COM4 <- 04 0e 0c 01 03 20 00 ff 79 01 08 00 00 00 00 

#   7.220    [HCI - COM4] LeReadBufSize()
#   7.220    COM4 -> 01 02 20 00 
send_command("01022000")

#   7.283    [HCI - COM4] HCI event received: OPCODE_LE_READ_BUF_SIZE (0xE)
#                              Command_Opcode = 0x2002 (8194)
#                              Event_Code = 0xE (14)
#                              HC_LE_ACL_Data_Packet_Length = 0x100 (256)
#                              HC_Total_Num_LE_ACL_Data_Packets = 0x10 (16)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x7 (7)
#                              Status = 0x0 (0)
#   7.283    COM4 <- 04 0e 07 01 02 20 00 00 01 10 

#   7.298    [HCI - COM4] SetEventMask(Event_Mask=2305983746702149776L)
#   7.298    COM4 -> 01 01 0c 08 90 88 01 00 00 80 00 20 
send_command("01010c089088010000800020")

#   7.486    [HCI - COM4] HCI event received: OPCODE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0xC01 (3073)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.486    COM4 <- 04 0e 04 01 01 0c 00 

#   7.564    [HCI - COM4] LeSetEventMask(LE_Event_Mask=0)
#   7.564    COM4 -> 01 01 20 08 00 00 00 00 00 00 00 00 
send_command("010120080000000000000000")

#   7.751    [HCI - COM4] HCI event received: OPCODE_LE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0x2001 (8193)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.751    COM4 <- 04 0e 04 01 01 20 00 

#   9.907    [HCI - COM4] LeSetExtAdvParam(Advertising_Handle=1,
#                        Advertising_Event_Properties=0,
#                        Primary_Advertising_Interval_Min=43,
#                        Primary_Advertising_Interval_Max=43,
#                        Primary_Advertising_Channel_Map=7,
#                        Own_Address_Type=0,
#                        Peer_Address_Type=0,
#                        Peer_Address='\xc8\x85\xf4\xf9*\xe4',
#                        Advertising_Filter_Policy=0,
#                        Advertising_Tx_Power=127,
#                        Primary_Advertising_PHY=1,
#                        Secondary_Advertising_Max_Skip=1,
#                        Secondary_Advertising_PHY=1,
#                        Advertising_SID=15,
#                        Scan_Request_Notification_Enable=0)
#   9.907    COM4 -> 01 36 20 19 01 00 00 2b 00 00 2b 00 00 07 00 00 c8 85 f4 f9 2a e4 00 7f 01 01 01 0f 00 
send_command("013620190100002b00002b0000070000c885f4f92ae4007f0101010f00")

#  10.360    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_PARAM (0xE)
#                              Command_Opcode = 0x2036 (8246)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x5 (5)
#                              Selected_Tx_Power = 0x0 (0)
#                              Status = 0x0 (0)
#  10.360    COM4 <- 04 0e 05 01 36 20 00 00 

#  10.376    [HCI - COM4] LeSetExtAdvData(Advertising_Handle=1,
#                       Operation=3,
#                       Fragment_Preference=1,
#                       Advertising_Data='P')
#  10.376    COM4 -> 01 37 20 05 01 03 01 01 50 
send_command("013720050103010150")

#  10.516    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_DATA (0xE)
#                              Command_Opcode = 0x2037 (8247)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  10.516    COM4 <- 04 0e 04 01 37 20 00 

#  10.532    [HCI - COM4] LeSetExtAdvEnable(Enable=True,
#                         Advertising_Handle=1,
#                         Duration=0,
#                         Max_Extended_Advertising_Events=0,
#                         enable_param=[{'Duration': 0,
#                         'Advertising_Handle': 1,
#                         'Max_Extended_Advertising_Events': 0}])
#  10.532    COM4 -> 01 39 20 06 01 01 01 00 00 00 
send_command("01392006010101000000")

read_events(seconds=5)

#  10.688    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  10.688    COM4 <- 04 0e 04 01 39 20 00 

#  15.859    [HCI - COM4] LeSetExtAdvEnable(Enable=False,
#                         Advertising_Handle=1,
#                         Duration=0,
#                         Max_Extended_Advertising_Events=0,
#                         enable_param=[{'Duration': 0,
#                         'Advertising_Handle': 1,
#                         'Max_Extended_Advertising_Events': 0}])
#  15.859    COM4 -> 01 39 20 06 00 01 01 00 00 00 
send_command("01392006000101000000")

read_events(seconds=2)


#  16.030    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  16.030    COM4 <- 04 0e 04 01 39 20 00 

#  16.124    [HCI - COM4] LeSetExtAdvParam(Advertising_Handle=1,
#                        Advertising_Event_Properties=0,
#                        Primary_Advertising_Interval_Min=43,
#                        Primary_Advertising_Interval_Max=43,
#                        Primary_Advertising_Channel_Map=7,
#                        Own_Address_Type=0,
#                        Peer_Address_Type=0,
#                        Peer_Address='\xc8\x85\xf4\xf9*\xe4',
#                        Advertising_Filter_Policy=0,
#                        Advertising_Tx_Power=127,
#                        Primary_Advertising_PHY=1,
#                        Secondary_Advertising_Max_Skip=15,
#                        Secondary_Advertising_PHY=1,
#                        Advertising_SID=15,
#                        Scan_Request_Notification_Enable=0)
#  16.124    COM4 -> 01 36 20 19 01 00 00 2b 00 00 2b 00 00 07 00 00 c8 85 f4 f9 2a e4 00 7f 01 0f 01 0f 00 
send_command("013620190100002b00002b0000070000c885f4f92ae4007f010f010f00")

#  16.577    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_PARAM (0xE)
#                              Command_Opcode = 0x2036 (8246)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x5 (5)
#                              Selected_Tx_Power = 0x0 (0)
#                              Status = 0x0 (0)
#  16.577    COM4 <- 04 0e 05 01 36 20 00 00 

#  16.593    [HCI - COM4] LeSetExtAdvData(Advertising_Handle=1,
#                       Operation=3,
#                       Fragment_Preference=1,
#                       Advertising_Data='}')
#  16.593    COM4 -> 01 37 20 05 01 03 01 01 7d 
send_command("01372005010301017d")

#  16.733    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_DATA (0xE)
#                              Command_Opcode = 0x2037 (8247)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  16.733    COM4 <- 04 0e 04 01 37 20 00 

#  16.749    [HCI - COM4] LeSetExtAdvEnable(Enable=True,
#                         Advertising_Handle=1,
#                         Duration=0,
#                         Max_Extended_Advertising_Events=0,
#                         enable_param=[{'Duration': 0,
#                         'Advertising_Handle': 1,
#                         'Max_Extended_Advertising_Events': 0}])
#  16.749    COM4 -> 01 39 20 06 01 01 01 00 00 00 
send_command("01392006010101000000")

#  16.905    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  16.905    COM4 <- 04 0e 04 01 39 20 00 



while(1):
  rcv_event()


#  38.057    [HCI - COM4] LeSetExtAdvEnable(Enable=False,
#                         Advertising_Handle=1,
#                         Duration=0,
#                         Max_Extended_Advertising_Events=0,
#                         enable_param=[{'Duration': 0,
#                         'Advertising_Handle': 1,
#                         'Max_Extended_Advertising_Events': 0}])
#  38.057    COM4 -> 01 39 20 06 00 01 01 00 00 00 
send_command("01392006000101000000")

#  38.213    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  38.213    COM4 <- 04 0e 04 01 39 20 00 

#  38.822    [HCI - COM4] LeSetExtAdvParam(Advertising_Handle=1,
#                        Advertising_Event_Properties=0,
#                        Primary_Advertising_Interval_Min=43,
#                        Primary_Advertising_Interval_Max=43,
#                        Primary_Advertising_Channel_Map=7,
#                        Own_Address_Type=0,
#                        Peer_Address_Type=0,
#                        Peer_Address='\xc8\x85\xf4\xf9*\xe4',
#                        Advertising_Filter_Policy=0,
#                        Advertising_Tx_Power=127,
#                        Primary_Advertising_PHY=1,
#                        Secondary_Advertising_Max_Skip=255,
#                        Secondary_Advertising_PHY=1,
#                        Advertising_SID=15,
#                        Scan_Request_Notification_Enable=0)
#  38.822    COM4 -> 01 36 20 19 01 00 00 2b 00 00 2b 00 00 07 00 00 c8 85 f4 f9 2a e4 00 7f 01 ff 01 0f 00 
send_command("013620190100002b00002b0000070000c885f4f92ae4007f01ff010f00")

#  39.275    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_PARAM (0xE)
#                              Command_Opcode = 0x2036 (8246)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x5 (5)
#                              Selected_Tx_Power = 0x0 (0)
#                              Status = 0x0 (0)
#  39.275    COM4 <- 04 0e 05 01 36 20 00 00 

#  39.291    [HCI - COM4] LeSetExtAdvData(Advertising_Handle=1,
#                       Operation=3,
#                       Fragment_Preference=1,
#                       Advertising_Data='\x0b')
#  39.291    COM4 -> 01 37 20 05 01 03 01 01 0b 
send_command("01372005010301010b")

#  39.432    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_DATA (0xE)
#                              Command_Opcode = 0x2037 (8247)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  39.432    COM4 <- 04 0e 04 01 37 20 00 

#  39.447    [HCI - COM4] LeSetExtAdvEnable(Enable=True,
#                         Advertising_Handle=1,
#                         Duration=0,
#                         Max_Extended_Advertising_Events=0,
#                         enable_param=[{'Duration': 0,
#                         'Advertising_Handle': 1,
#                         'Max_Extended_Advertising_Events': 0}])
#  39.447    COM4 -> 01 39 20 06 01 01 01 00 00 00 
send_command("01392006010101000000")

#  39.619    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  39.619    COM4 <- 04 0e 04 01 39 20 00 

#  72.315    [HCI - COM4] LeSetExtAdvEnable(Enable=False,
#                         Advertising_Handle=1,
#                         Duration=0,
#                         Max_Extended_Advertising_Events=0,
#                         enable_param=[{'Duration': 0,
#                         'Advertising_Handle': 1,
#                         'Max_Extended_Advertising_Events': 0}])
#  72.315    COM4 -> 01 39 20 06 00 01 01 00 00 00 
send_command("01392006000101000000")

#  72.471    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  72.471    COM4 <- 04 0e 04 01 39 20 00 

#  81.938    [HCI - COM4] LeSetExtAdvParam(Advertising_Handle=1,
#                        Advertising_Event_Properties=1,
#                        Primary_Advertising_Interval_Min=43,
#                        Primary_Advertising_Interval_Max=43,
#                        Primary_Advertising_Channel_Map=7,
#                        Own_Address_Type=0,
#                        Peer_Address_Type=0,
#                        Peer_Address='\xc8\x85\xf4\xf9*\xe4',
#                        Advertising_Filter_Policy=0,
#                        Advertising_Tx_Power=127,
#                        Primary_Advertising_PHY=1,
#                        Secondary_Advertising_Max_Skip=8,
#                        Secondary_Advertising_PHY=1,
#                        Advertising_SID=15,
#                        Scan_Request_Notification_Enable=0)
#  81.938    COM4 -> 01 36 20 19 01 01 00 2b 00 00 2b 00 00 07 00 00 c8 85 f4 f9 2a e4 00 7f 01 08 01 0f 00 
send_command("013620190101002b00002b0000070000c885f4f92ae4007f0108010f00")

#  82.391    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_PARAM (0xE)
#                              Command_Opcode = 0x2036 (8246)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x5 (5)
#                              Selected_Tx_Power = 0x0 (0)
#                              Status = 0x0 (0)
#  82.391    COM4 <- 04 0e 05 01 36 20 00 00 

#  82.407    [HCI - COM4] LeSetExtAdvData(Advertising_Handle=1,
#                       Operation=3,
#                       Fragment_Preference=1,
#                       Advertising_Data='\x10')
#  82.407    COM4 -> 01 37 20 05 01 03 01 01 10 
send_command("013720050103010110")

#  82.547    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_DATA (0xE)
#                              Command_Opcode = 0x2037 (8247)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  82.547    COM4 <- 04 0e 04 01 37 20 00 

#  82.563    [HCI - COM4] LeSetExtAdvEnable(Enable=True,
#                         Advertising_Handle=1,
#                         Duration=0,
#                         Max_Extended_Advertising_Events=0,
#                         enable_param=[{'Duration': 0,
#                         'Advertising_Handle': 1,
#                         'Max_Extended_Advertising_Events': 0}])
#  82.563    COM4 -> 01 39 20 06 01 01 01 00 00 00 
send_command("01392006010101000000")

#  82.719    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  82.719    COM4 <- 04 0e 04 01 39 20 00 

#  94.310    [HCI - COM4] LeSetExtAdvEnable(Enable=False,
#                         Advertising_Handle=1,
#                         Duration=0,
#                         Max_Extended_Advertising_Events=0,
#                         enable_param=[{'Duration': 0,
#                         'Advertising_Handle': 1,
#                         'Max_Extended_Advertising_Events': 0}])
#  94.310    COM4 -> 01 39 20 06 00 01 01 00 00 00 
send_command("01392006000101000000")

#  94.497    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  94.497    COM4 <- 04 0e 04 01 39 20 00 

#  94.857    [HCI - COM4] LeSetExtAdvParam(Advertising_Handle=1,
#                        Advertising_Event_Properties=4,
#                        Primary_Advertising_Interval_Min=43,
#                        Primary_Advertising_Interval_Max=43,
#                        Primary_Advertising_Channel_Map=7,
#                        Own_Address_Type=0,
#                        Peer_Address_Type=0,
#                        Peer_Address='\xc8\x85\xf4\xf9*\xe4',
#                        Advertising_Filter_Policy=0,
#                        Advertising_Tx_Power=127,
#                        Primary_Advertising_PHY=1,
#                        Secondary_Advertising_Max_Skip=8,
#                        Secondary_Advertising_PHY=1,
#                        Advertising_SID=15,
#                        Scan_Request_Notification_Enable=0)
#  94.857    COM4 -> 01 36 20 19 01 04 00 2b 00 00 2b 00 00 07 00 00 c8 85 f4 f9 2a e4 00 7f 01 08 01 0f 00 
send_command("013620190104002b00002b0000070000c885f4f92ae4007f0108010f00")

#  95.310    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_PARAM (0xE)
#                              Command_Opcode = 0x2036 (8246)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x5 (5)
#                              Selected_Tx_Power = 0x0 (0)
#                              Status = 0x0 (0)
#  95.310    COM4 <- 04 0e 05 01 36 20 00 00 

#  95.325    [HCI - COM4] LeSetExtAdvData(Advertising_Handle=1,
#                       Operation=3,
#                       Fragment_Preference=1,
#                       Advertising_Data='\x83')
#  95.325    COM4 -> 01 37 20 05 01 03 01 01 83 
send_command("013720050103010183")

#  95.466    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_DATA (0xE)
#                              Command_Opcode = 0x2037 (8247)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  95.466    COM4 <- 04 0e 04 01 37 20 00 

#  95.482    [HCI - COM4] LeSetExtAdvEnable(Enable=True,
#                         Advertising_Handle=1,
#                         Duration=0,
#                         Max_Extended_Advertising_Events=0,
#                         enable_param=[{'Duration': 0,
#                         'Advertising_Handle': 1,
#                         'Max_Extended_Advertising_Events': 0}])
#  95.482    COM4 -> 01 39 20 06 01 01 01 00 00 00 
send_command("01392006010101000000")

#  95.638    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  95.638    COM4 <- 04 0e 04 01 39 20 00 

# 107.229    [HCI - COM4] LeSetExtAdvEnable(Enable=False,
#                         Advertising_Handle=1,
#                         Duration=0,
#                         Max_Extended_Advertising_Events=0,
#                         enable_param=[{'Duration': 0,
#                         'Advertising_Handle': 1,
#                         'Max_Extended_Advertising_Events': 0}])
# 107.229    COM4 -> 01 39 20 06 00 01 01 00 00 00 
send_command("01392006000101000000")

# 107.401    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
# 107.401    COM4 <- 04 0e 04 01 39 20 00 

# 107.760    [HCI - COM4] LeSetExtAdvParam(Advertising_Handle=1,
#                        Advertising_Event_Properties=5,
#                        Primary_Advertising_Interval_Min=43,
#                        Primary_Advertising_Interval_Max=43,
#                        Primary_Advertising_Channel_Map=7,
#                        Own_Address_Type=0,
#                        Peer_Address_Type=0,
#                        Peer_Address='\xc8\x85\xf4\xf9*\xe4',
#                        Advertising_Filter_Policy=0,
#                        Advertising_Tx_Power=127,
#                        Primary_Advertising_PHY=1,
#                        Secondary_Advertising_Max_Skip=8,
#                        Secondary_Advertising_PHY=1,
#                        Advertising_SID=15,
#                        Scan_Request_Notification_Enable=0)
# 107.760    COM4 -> 01 36 20 19 01 05 00 2b 00 00 2b 00 00 07 00 00 c8 85 f4 f9 2a e4 00 7f 01 08 01 0f 00 
send_command("013620190105002b00002b0000070000c885f4f92ae4007f0108010f00")

# 108.213    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_PARAM (0xE)
#                              Command_Opcode = 0x2036 (8246)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x5 (5)
#                              Selected_Tx_Power = 0x0 (0)
#                              Status = 0x0 (0)
# 108.213    COM4 <- 04 0e 05 01 36 20 00 00 

# 108.229    [HCI - COM4] LeSetExtAdvData(Advertising_Handle=1,
#                       Operation=3,
#                       Fragment_Preference=1,
#                       Advertising_Data='\xdc')
# 108.229    COM4 -> 01 37 20 05 01 03 01 01 dc 
send_command("0137200501030101dc")

# 108.370    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_DATA (0xE)
#                              Command_Opcode = 0x2037 (8247)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
# 108.370    COM4 <- 04 0e 04 01 37 20 00 

# 108.385    [HCI - COM4] LeSetExtAdvEnable(Enable=True,
#                         Advertising_Handle=1,
#                         Duration=0,
#                         Max_Extended_Advertising_Events=0,
#                         enable_param=[{'Duration': 0,
#                         'Advertising_Handle': 1,
#                         'Max_Extended_Advertising_Events': 0}])
# 108.385    COM4 -> 01 39 20 06 01 01 01 00 00 00 
send_command("01392006010101000000")

# 108.541    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
# 108.541    COM4 <- 04 0e 04 01 39 20 00 

# 120.133    [HCI - COM4] LeSetExtAdvEnable(Enable=False,
#                         Advertising_Handle=1,
#                         Duration=0,
#                         Max_Extended_Advertising_Events=0,
#                         enable_param=[{'Duration': 0,
#                         'Advertising_Handle': 1,
#                         'Max_Extended_Advertising_Events': 0}])
# 120.133    COM4 -> 01 39 20 06 00 01 01 00 00 00 
send_command("01392006000101000000")

# 120.304    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
# 120.304    COM4 <- 04 0e 04 01 39 20 00 

# 120.789    [HCI - COM4] Reset()
# 120.789    COM4 -> 01 03 0c 00 
send_command("01030c00")

# 120.851    [HCI - COM4] HCI event received: OPCODE_RESET (0xE)
#                              Command_Opcode = 0xC03 (3075)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
# 120.851    COM4 <- 04 0e 04 01 03 0c 00 

