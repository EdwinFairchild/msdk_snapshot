#! /usr/bin/env python

filename="../common.py"
exec(open(filename).read())

#   4.984    [HCI - COM19] Reset()
#   4.984    COM19 -> 01 03 0c 00 
send_command("01030c00")

#   5.047    [HCI - COM19] HCI event received: OPCODE_RESET (0xE)
#                              Command_Opcode = 0xC03 (3075)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   5.047    COM19 <- 04 0e 04 01 03 0c 00 

#   5.063    [HCI - COM19] Reset()
#   5.063    COM19 -> 01 03 0c 00 
send_command("01030c00")

#   5.125    [HCI - COM19] HCI event received: OPCODE_RESET (0xE)
#                              Command_Opcode = 0xC03 (3075)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   5.125    COM19 <- 04 0e 04 01 03 0c 00 

#   5.938    [HCI - COM19] ReadBdAddr()
#   5.938    COM19 -> 01 09 10 00 
send_command("01091000")

#   6.000    [HCI - COM19] HCI event received: OPCODE_READ_BD_ADDR (0xE)
#                              BD_ADDR = 0x5F123456 (1595028566)
#                              Command_Opcode = 0x1009 (4105)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0xA (10)
#                              Status = 0x0 (0)
#   6.000    COM19 <- 04 0e 0a 01 09 10 00 56 34 12 5f 00 00 

#   6.110    [HCI - COM19] SetEventMask(Event_Mask=2305983746702149776L)
#   6.110    COM19 -> 01 01 0c 08 90 88 01 00 00 80 00 20 
send_command("01010c089088010000800020")

#   6.297    [HCI - COM19] HCI event received: OPCODE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0xC01 (3073)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.297    COM19 <- 04 0e 04 01 01 0c 00 

#   6.313    [HCI - COM19] SetEventMaskPage2(Event_Mask_Page_2=8388608)
#   6.313    COM19 -> 01 63 0c 08 00 00 80 00 00 00 00 00 
send_command("01630c080000800000000000")

#   6.500    [HCI - COM19] HCI event received: OPCODE_SET_EVENT_MASK_PAGE2 (0xE)
#                              Command_Opcode = 0xC63 (3171)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.500    COM19 <- 04 0e 04 01 63 0c 00 

#   6.766    [HCI - COM19] ReadLocalVerInfo()
#   6.766    COM19 -> 01 01 10 00 
send_command("01011000")

#   6.829    [HCI - COM19] HCI event received: OPCODE_READ_LOCAL_VER_INFO (0xE)
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
#   6.829    COM19 <- 04 0e 0c 01 01 10 00 09 03 23 09 5f 00 03 23 

#   6.954    [HCI - COM19] LeReadLocalSupFeat()
#   6.954    COM19 -> 01 03 20 00 
send_command("01032000")

#   7.016    [HCI - COM19] HCI event received: OPCODE_LE_READ_LOCAL_SUP_FEAT (0xE)
#                              Command_Opcode = 0x2003 (8195)
#                              Event_Code = 0xE (14)
#                              LE_Features = 0x80179FF (134314495)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0xC (12)
#                              Status = 0x0 (0)
#   7.016    COM19 <- 04 0e 0c 01 03 20 00 ff 79 01 08 00 00 00 00 

#   7.110    [HCI - COM19] LeReadBufSize()
#   7.110    COM19 -> 01 02 20 00 
send_command("01022000")

#   7.173    [HCI - COM19] HCI event received: OPCODE_LE_READ_BUF_SIZE (0xE)
#                              Command_Opcode = 0x2002 (8194)
#                              Event_Code = 0xE (14)
#                              HC_LE_ACL_Data_Packet_Length = 0x200 (512)
#                              HC_Total_Num_LE_ACL_Data_Packets = 0x10 (16)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x7 (7)
#                              Status = 0x0 (0)
#   7.173    COM19 <- 04 0e 07 01 02 20 00 00 02 10 

#   7.203    [HCI - COM19] SetEventMask(Event_Mask=2305983746702149776L)
#   7.203    COM19 -> 01 01 0c 08 90 88 01 00 00 80 00 20 
send_command("01010c089088010000800020")

#   7.391    [HCI - COM19] HCI event received: OPCODE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0xC01 (3073)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.391    COM19 <- 04 0e 04 01 01 0c 00 

#   7.469    [HCI - COM19] LeSetEventMask(LE_Event_Mask=61440)
#   7.469    COM19 -> 01 01 20 08 00 f0 00 00 00 00 00 00 
send_command("0101200800f0000000000000")

#   7.672    [HCI - COM19] HCI event received: OPCODE_LE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0x2001 (8193)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.672    COM19 <- 04 0e 04 01 01 20 00 

#   8.657    [HCI - COM19] LeSetExtScanEnable(Enable=True,
#                           Filter_Duplicates=0,
#                           Duration=0,
#                           Period=0)
#   8.657    COM19 -> 01 42 20 06 01 00 00 00 00 00 
send_command("01422006010000000000")

#   8.812    [HCI - COM19] HCI event received: OPCODE_LE_SET_EXT_SCAN_ENABLE (0xE)
#                              Command_Opcode = 0x2042 (8258)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.812    COM19 <- 04 0e 04 01 42 20 00 

#   8.828    [HCI - COM19] LePeriodicAdvCreateSync(Options=0,
#                                Advertising_SID=15,
#                                Advertising_Address_Type=0,
#                                Advertiser_Address='\xc8\x85\xf4\xf9*\xe4',
#                                Skip=0,
#                                Sync_Timeout=37.5,
#                                CTE_Type=0)
#   8.828    COM19 -> 01 44 20 0e 00 0f 00 c8 85 f4 f9 2a e4 00 00 25 00 00 
send_command("0144200e000f00c885f4f92ae40000250000")

#   9.109    [HCI - COM19] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x02l\xdb\x1bY\x97;~\xfc\x0c\xe4\xc7\xe3T\xf6\x07\xf8\x92I\x86\x95\xeep\xf0', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 9841346318646L, 'Direct_Address': 0, 'RSSI': -88, 'Address_Type': 1, 'Advertising_SID': 255}]"
#   9.109    COM19 <- 04 3e 39 0d 01 10 00 01 36 91 f3 5d f3 08 01 00 ff 7f a8 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 6c db 1b 59 97 3b 7e fc 0c e4 c7 e3 54 f6 07 f8 92 49 86 95 ee 70 f0 

#   9.109    [HCI - COM19] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x02\\\x82\xba\xfa\t\xa3\xe2S\n1\xff\xcd\x10qvC\xa9\xb1\xe2\xc8\xa0\x9bG', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 121051219133230L, 'Direct_Address': 0, 'RSSI': -87, 'Address_Type': 1, 'Advertising_SID': 255}]"
#   9.109    COM19 <- 04 3e 39 0d 01 10 00 01 2e 83 ea 6e 18 6e 01 00 ff 7f a9 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 5c 82 ba fa 09 a3 e2 53 0a 31 ff cd 10 71 76 43 a9 b1 e2 c8 a0 9b 47 

#   9.109    [HCI - COM19] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x1A (26)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 1, 'Event_Type': 0, 'Data': '', 'Data_Length': 0, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 100, 'Tx_Power': 127, 'Address': 250873233311176L, 'Direct_Address': 0, 'RSSI': -47, 'Address_Type': 0, 'Advertising_SID': 15}]"
#   9.109    COM19 <- 04 3e 1a 0d 01 00 00 00 c8 85 f4 f9 2a e4 01 01 0f 7f d1 64 00 00 00 00 00 00 00 00 00 

#   9.109    [HCI - COM19] HCI event received: EVT_CMD_STATUS (0xF)
#                              Command_Opcode = 0x2044 (8260)
#                              Event_Code = 0xF (15)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   9.109    COM19 <- 04 0f 04 00 01 44 20 

#  11.547    [HCI - COM19] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': "\x1e\xff\x06\x00\x01\t \x02R\x8b\x98?\x81a\xd0|d\x9e\x0f\xe8\x96\xdc\xe9HE*'\xbd\xdd\x85\x97", 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 40736703808548L, 'Direct_Address': 0, 'RSSI': -85, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  11.547    COM19 <- 04 3e 39 0d 01 10 00 01 24 84 c2 c0 0c 25 01 00 ff 7f ab 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 52 8b 98 3f 81 61 d0 7c 64 9e 0f e8 96 dc e9 48 45 2a 27 bd dd 85 97 

#  14.953    [HCI - COM19] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x02l\xdb\x1bY\x97;~\xfc\x0c\xe4\xc7\xe3T\xf6\x07\xf8\x92I\x86\x95\xeep\xf0', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 9841346318646L, 'Direct_Address': 0, 'RSSI': -83, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  14.953    COM19 <- 04 3e 39 0d 01 10 00 01 36 91 f3 5d f3 08 01 00 ff 7f ad 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 6c db 1b 59 97 3b 7e fc 0c e4 c7 e3 54 f6 07 f8 92 49 86 95 ee 70 f0 

#  14.953    [HCI - COM19] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x02\x8d\xf9\x06U\xb3`\x07\x04\x92]\xa1\x94\xf59\x9a\x18\xacR\xf1E\x1e\x87\xd0', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 67366974162558L, 'Direct_Address': 0, 'RSSI': -76, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  14.953    COM19 <- 04 3e 39 0d 01 10 00 01 7e 86 90 18 45 3d 01 00 ff 7f b4 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 8d f9 06 55 b3 60 07 04 92 5d a1 94 f5 39 9a 18 ac 52 f1 45 1e 87 d0 

#  17.391    [HCI - COM19] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x02\\\x82\xba\xfa\t\xa3\xe2S\n1\xff\xcd\x10qvC\xa9\xb1\xe2\xc8\xa0\x9bG', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 121051219133230L, 'Direct_Address': 0, 'RSSI': -95, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  17.391    COM19 <- 04 3e 39 0d 01 10 00 01 2e 83 ea 6e 18 6e 01 00 ff 7f a1 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 5c 82 ba fa 09 a3 e2 53 0a 31 ff cd 10 71 76 43 a9 b1 e2 c8 a0 9b 47 

while(1):
  rcv_event()
