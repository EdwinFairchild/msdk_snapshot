filename="../common.py"
exec(open(filename).read())

#   5.058    [HCI - COM4] Reset()
#   5.058    COM4 -> 01 03 0c 00 
send_command("01030c00")

#   5.121    [HCI - COM4] HCI event received: OPCODE_RESET (0xE)
#                              Command_Opcode = 0xC03 (3075)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   5.121    COM4 <- 04 0e 04 01 03 0c 00 

#   5.136    [HCI - COM4] Reset()
#   5.136    COM4 -> 01 03 0c 00 
send_command("01030c00")

#   5.199    [HCI - COM4] HCI event received: OPCODE_RESET (0xE)
#                              Command_Opcode = 0xC03 (3075)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   5.199    COM4 <- 04 0e 04 01 03 0c 00 

#   5.949    [HCI - COM4] ReadBdAddr()
#   5.949    COM4 -> 01 09 10 00 
send_command("01091000")

#   6.011    [HCI - COM4] HCI event received: OPCODE_READ_BD_ADDR (0xE)
#                              BD_ADDR = 0x5F123456 (1595028566)
#                              Command_Opcode = 0x1009 (4105)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0xA (10)
#                              Status = 0x0 (0)
#   6.011    COM4 <- 04 0e 0a 01 09 10 00 56 34 12 5f 00 00 

#   6.089    [HCI - COM4] SetEventMask(Event_Mask=2305983746702149776L)
#   6.089    COM4 -> 01 01 0c 08 90 88 01 00 00 80 00 20 
send_command("01010c089088010000800020")

#   6.277    [HCI - COM4] HCI event received: OPCODE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0xC01 (3073)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.277    COM4 <- 04 0e 04 01 01 0c 00 

#   6.292    [HCI - COM4] SetEventMaskPage2(Event_Mask_Page_2=8388608)
#   6.292    COM4 -> 01 63 0c 08 00 00 80 00 00 00 00 00 
send_command("01630c080000800000000000")

#   6.480    [HCI - COM4] HCI event received: OPCODE_SET_EVENT_MASK_PAGE2 (0xE)
#                              Command_Opcode = 0xC63 (3171)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.480    COM4 <- 04 0e 04 01 63 0c 00 

#   6.776    [HCI - COM4] ReadLocalVerInfo()
#   6.776    COM4 -> 01 01 10 00 
send_command("01011000")

#   6.839    [HCI - COM4] HCI event received: OPCODE_READ_LOCAL_VER_INFO (0xE)
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
#   6.839    COM4 <- 04 0e 0c 01 01 10 00 0a 03 23 0a 5f 00 03 23 

#   6.964    [HCI - COM4] LeReadLocalSupFeat()
#   6.964    COM4 -> 01 03 20 00 
send_command("01032000")

#   7.027    [HCI - COM4] HCI event received: OPCODE_LE_READ_LOCAL_SUP_FEAT (0xE)
#                              Command_Opcode = 0x2003 (8195)
#                              Event_Code = 0xE (14)
#                              LE_Features = 0x80179FF (134314495)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0xC (12)
#                              Status = 0x0 (0)
#   7.027    COM4 <- 04 0e 0c 01 03 20 00 ff 79 01 08 00 00 00 00 

#   7.105    [HCI - COM4] LeReadBufSize()
#   7.105    COM4 -> 01 02 20 00 
send_command("01022000")

#   7.167    [HCI - COM4] HCI event received: OPCODE_LE_READ_BUF_SIZE (0xE)
#                              Command_Opcode = 0x2002 (8194)
#                              Event_Code = 0xE (14)
#                              HC_LE_ACL_Data_Packet_Length = 0x100 (256)
#                              HC_Total_Num_LE_ACL_Data_Packets = 0x10 (16)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x7 (7)
#                              Status = 0x0 (0)
#   7.167    COM4 <- 04 0e 07 01 02 20 00 00 01 10 

#   7.198    [HCI - COM4] SetEventMask(Event_Mask=2305983746702149776L)
#   7.198    COM4 -> 01 01 0c 08 90 88 01 00 00 80 00 20 
send_command("01010c089088010000800020")

#   7.386    [HCI - COM4] HCI event received: OPCODE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0xC01 (3073)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.386    COM4 <- 04 0e 04 01 01 0c 00 

#   7.464    [HCI - COM4] LeSetEventMask(LE_Event_Mask=4096)
#   7.464    COM4 -> 01 01 20 08 00 10 00 00 00 00 00 00 
send_command("010120080010000000000000")

#   7.651    [HCI - COM4] HCI event received: OPCODE_LE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0x2001 (8193)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.651    COM4 <- 04 0e 04 01 01 20 00 

#  16.212    [HCI - COM4] LeSetExtScanParam(Own_Address_Type=0,
#                         Scanning_Filter_Policy=0,
#                         Scanning_PHYs=5,
#                         Scan_Type=None,
#                         Scan_Interval=None,
#                         Scan_Window=None,
#                         scan_param=[{'Scan_Interval': 32,
#                         'Scan_Window': 16,
#                         'Scan_Type': 1},
#                         {'Scan_Interval': 32,
#                         'Scan_Window': 16,
#                         'Scan_Type': 1}])
#  16.212    COM4 -> 01 41 20 0d 00 00 05 01 20 00 10 00 01 20 00 10 00 
send_command("0141200d00000501200010000120001000")

#  16.478    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_SCAN_PARAM (0xE)
#                              Command_Opcode = 0x2041 (8257)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  16.478    COM4 <- 04 0e 04 01 41 20 00 

#  16.493    [HCI - COM4] LeSetExtScanEnable(Enable=True,
#                          Filter_Duplicates=0,
#                          Duration=0,
#                          Period=0)
#  16.493    COM4 -> 01 42 20 06 01 00 00 00 00 00 
send_command("01422006010000000000")

#  16.665    [HCI - COM4] HCI event received: OPCODE_LE_SET_EXT_SCAN_ENABLE (0xE)
#                              Command_Opcode = 0x2042 (8258)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  16.665    COM4 <- 04 0e 04 01 42 20 00 

#  16.681    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x02#\xd9\x07~\x90\xa4\xf2y\xdaa\xbf\xdd\x07\xde$\t\x80\xd88\x96hJj', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 32737509319702L, 'Direct_Address': 0, 'RSSI': -92, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  16.681    COM4 <- 04 3e 39 0d 01 10 00 01 16 24 9d 4b c6 1d 01 00 ff 7f a4 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 23 d9 07 7e 90 a4 f2 79 da 61 bf dd 07 de 24 09 80 d8 38 96 68 4a 6a 

#  16.915    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x024>\x0c\xc7X\xdf\x15>K\xfd\x08\r\x14%\xf6\xb0\x06\xb8\x94\xd9\x1bj\\', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 64807690209562L, 'Direct_Address': 0, 'RSSI': -76, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  16.915    COM4 <- 04 3e 39 0d 01 10 00 01 1a 89 5a 37 f1 3a 01 00 ff 7f b4 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 34 3e 0c c7 58 df 15 3e 4b fd 08 0d 14 25 f6 b0 06 b8 94 d9 1b 6a 5c 

#  16.931    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x03\x03\x9c\x07\x1a\xff\x05\x00\x00\x04\\\x007SG179D00274\x00\x00\x00\x00\x00\x00\x00', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 185381861424323L, 'Direct_Address': 0, 'RSSI': -77, 'Address_Type': 0, 'Advertising_SID': 255}]"
#  16.931    COM4 <- 04 3e 39 0d 01 10 00 00 c3 7c ff 93 9a a8 01 00 ff 7f b3 00 00 00 00 00 00 00 00 00 1f 03 03 9c 07 1a ff 05 00 00 04 5c 00 37 53 47 31 37 39 44 30 30 32 37 34 00 00 00 00 00 00 00 

#  17.149    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x024>\x0c\xc7X\xdf\x15>K\xfd\x08\r\x14%\xf6\xb0\x06\xb8\x94\xd9\x1bj\\', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 64807690209562L, 'Direct_Address': 0, 'RSSI': -82, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  17.149    COM4 <- 04 3e 39 0d 01 10 00 01 1a 89 5a 37 f1 3a 01 00 ff 7f ae 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 34 3e 0c c7 58 df 15 3e 4b fd 08 0d 14 25 f6 b0 06 b8 94 d9 1b 6a 5c 

#  17.149    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x00.\x14b\x81q\xec@\xd7z\x15\xd9l\xf7@:!\x87k\xa6V\x13x\xa9', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 44730301865822L, 'Direct_Address': 0, 'RSSI': -66, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  17.149    COM4 <- 04 3e 39 0d 01 10 00 01 5e df c0 95 ae 28 01 00 ff 7f be 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 00 2e 14 62 81 71 ec 40 d7 7a 15 d9 6c f7 40 3a 21 87 6b a6 56 13 78 a9 

#  17.181    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0xFF (255)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 1, 'Event_Type': 42, 'Data': '\xc1\x13\xc8P\x1102mH\x9a\xd2\x8c\x82\xfd\x92\x0b\x0b|\xdb>l\x0f\xf3u\x8cg4\xd6\x9f\x96 \xe4\xfe\x17\x0e\xce\xb5\x1e\xe6K\x1da\xca\xe4\x12}\xc3\xa2\xae\xf0\xb0i\xeb\xc6/\xb1Go\xf60\xe5\x05\x95\xbb\xf3\xcc\x08\x05\x0cP\xe1B\xf0)l\xc3(\xccZ\xeaLa\xcc\xdb\xe7`\xadh!\xbb\xa1\xc0\xdfy\xb7\xa9U\xb4I\xb8e\xf9\xb8\x10\x1f\'\x18\xdbM\x96\xaf\xeb:\xed"\x9f\x11\x12\xec\x95\x90\x1ea"\xb8\xe4=\x0f\xe5\x8d\x9c\x1b\x1bG\xfc\xf9\xbd\xf3 \xe7\xc4\xde\xc25\xda\x87.\x97\x1c\x89w\x0c\x8e\x88\xe9`\xe0$:\xca\x8e\x1b\x17!\x0e\xf6\xb5\xb6nf\x95&\x1d/Z.\xb2g;\xfe\xff\x88\x87\xe5\xf4b\xeb\xf0zh\xcd\x06\xa4\xa4\xe9_Yx\x13Lf8\x94Nj\xc6\xbeIG\xc8\xe1\xb2\xb0@\xd0J|09Wp/\xf2\xd0\xc2\x14\xe3U}', 'Data_Length': 229, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 250873233311176L, 'Direct_Address': 0, 'RSSI': -51, 'Address_Type': 0, 'Advertising_SID': 15}]"
#  17.181    COM4 <- 04 3e ff 0d 01 2a 00 00 c8 85 f4 f9 2a e4 01 01 0f 7f cd 00 00 00 00 00 00 00 00 00 e5 c1 13 c8 50 11 30 32 6d 48 9a d2 8c 82 fd 92 0b 0b 7c db 3e 6c 0f f3 75 8c 67 34 d6 9f 96 20 e4 fe 17 0e ce b5 1e e6 4b 1d 61 ca e4 12 7d c3 a2 ae f0 b0 69 eb c6 2f b1 47 6f f6 30 e5 05 95 bb f3 cc 08 05 0c 50 e1 42 f0 29 6c c3 28 cc 5a ea 4c 61 cc db e7 60 ad 68 21 bb a1 c0 df 79 b7 a9 55 b4 49 b8 65 f9 b8 10 1f 27 18 db 4d 96 af eb 3a ed 22 9f 11 12 ec 95 90 1e 61 22 b8 e4 3d 0f e5 8d 9c 1b 1b 47 fc f9 bd f3 20 e7 c4 de c2 35 da 87 2e 97 1c 89 77 0c 8e 88 e9 60 e0 24 3a ca 8e 1b 17 21 0e f6 b5 b6 6e 66 95 26 1d 2f 5a 2e b2 67 3b fe ff 88 87 e5 f4 62 eb f0 7a 68 cd 06 a4 a4 e9 5f 59 78 13 4c 66 38 94 4e 6a c6 be 49 47 c8 e1 b2 b0 40 d0 4a 7c 30 39 57 70 2f f2 d0 c2 14 e3 55 7d 

#  17.181    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x35 (53)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 1, 'Event_Type': 10, 'Data': '. \\\xacR\xf1s\x84\x87\xc4S\x85\xb5\xb6\xa7\xdbTy\x99\x87\xd8\xb4\x94\xfc\xc9r\xc4', 'Data_Length': 27, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 250873233311176L, 'Direct_Address': 0, 'RSSI': -51, 'Address_Type': 0, 'Advertising_SID': 15}]"
#  17.181    COM4 <- 04 3e 35 0d 01 0a 00 00 c8 85 f4 f9 2a e4 01 01 0f 7f cd 00 00 00 00 00 00 00 00 00 1b 2e 20 5c ac 52 f1 73 84 87 c4 53 85 b5 b6 a7 db 54 79 99 87 d8 b4 94 fc c9 72 c4 

#  17.181    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x02z\xec\xc7\x91\x91L\xf2\xf5\x0f\x08\x9c\xc2&\xbaI\xb2\x9d\x06\xd1\xcb\x9d\r|', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 30747452361117L, 'Direct_Address': 0, 'RSSI': -92, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  17.181    COM4 <- 04 3e 39 0d 01 10 00 01 9d c9 f9 f2 f6 1b 01 00 ff 7f a4 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 7a ec c7 91 91 4c f2 f5 0f 08 9c c2 26 ba 49 b2 9d 06 d1 cb 9d 0d 7c 

#  17.196    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x02#\xd9\x07~\x90\xa4\xf2y\xdaa\xbf\xdd\x07\xde$\t\x80\xd88\x96hJj', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 32737509319702L, 'Direct_Address': 0, 'RSSI': -90, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  17.196    COM4 <- 04 3e 39 0d 01 10 00 01 16 24 9d 4b c6 1d 01 00 ff 7f a6 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 23 d9 07 7e 90 a4 f2 79 da 61 bf dd 07 de 24 09 80 d8 38 96 68 4a 6a 

#  17.212    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x02/e\xb7\xd5\x1e\t\xc2j\xe2\xc7\t5\x19 .\x02\xa1\x80\xc2j\xfd\xb3^', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 24556672076571L, 'Direct_Address': 0, 'RSSI': -81, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  17.212    COM4 <- 04 3e 39 0d 01 10 00 01 1b 4b b7 8b 55 16 01 00 ff 7f af 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 2f 65 b7 d5 1e 09 c2 6a e2 c7 09 35 19 20 2e 02 a1 80 c2 6a fd b3 5e 

#  17.290    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x02#\xd9\x07~\x90\xa4\xf2y\xdaa\xbf\xdd\x07\xde$\t\x80\xd88\x96hJj', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 32737509319702L, 'Direct_Address': 0, 'RSSI': -78, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  17.290    COM4 <- 04 3e 39 0d 01 10 00 01 16 24 9d 4b c6 1d 01 00 ff 7f b2 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 23 d9 07 7e 90 a4 f2 79 da 61 bf dd 07 de 24 09 80 d8 38 96 68 4a 6a 

#  17.306    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x02\xa0\xdd\xd7\xe6\x04\xc1\xdf\r6?\xadcd\xe8\x7fKhrO\x97u\xe6\xf4', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 90398327500093L, 'Direct_Address': 0, 'RSSI': -92, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  17.306    COM4 <- 04 3e 39 0d 01 10 00 01 3d ed 32 80 37 52 01 00 ff 7f a4 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 a0 dd d7 e6 04 c1 df 0d 36 3f ad 63 64 e8 7f 4b 68 72 4f 97 75 e6 f4 

#  17.430    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x02/e\xb7\xd5\x1e\t\xc2j\xe2\xc7\t5\x19 .\x02\xa1\x80\xc2j\xfd\xb3^', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 24556672076571L, 'Direct_Address': 0, 'RSSI': -78, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  17.430    COM4 <- 04 3e 39 0d 01 10 00 01 1b 4b b7 8b 55 16 01 00 ff 7f b2 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 2f 65 b7 d5 1e 09 c2 6a e2 c7 09 35 19 20 2e 02 a1 80 c2 6a fd b3 5e 

#  17.446    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x03\x03\x9c\x07\x1a\xff\x05\x00\x00\x04\\\x007SG179D00274\x00\x00\x00\x00\x00\x00\x00', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 185381861424323L, 'Direct_Address': 0, 'RSSI': -76, 'Address_Type': 0, 'Advertising_SID': 255}]"
#  17.446    COM4 <- 04 3e 39 0d 01 10 00 00 c3 7c ff 93 9a a8 01 00 ff 7f b4 00 00 00 00 00 00 00 00 00 1f 03 03 9c 07 1a ff 05 00 00 04 5c 00 37 53 47 31 37 39 44 30 30 32 37 34 00 00 00 00 00 00 00 

#  17.462    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x34 (52)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x19\xff\x06\x00\x01\t!\n1\x08\x02\xb0\xd3\x04LT12-05006-R', 'Data_Length': 26, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 38460051352148L, 'Direct_Address': 0, 'RSSI': -91, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  17.462    COM4 <- 04 3e 34 0d 01 10 00 01 54 ea b2 ad fa 22 01 00 ff 7f a5 00 00 00 00 00 00 00 00 00 1a 19 ff 06 00 01 09 21 0a 31 08 02 b0 d3 04 4c 54 31 32 2d 30 35 30 30 36 2d 52 

#  17.571    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x024>\x0c\xc7X\xdf\x15>K\xfd\x08\r\x14%\xf6\xb0\x06\xb8\x94\xd9\x1bj\\', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 64807690209562L, 'Direct_Address': 0, 'RSSI': -82, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  17.571    COM4 <- 04 3e 39 0d 01 10 00 01 1a 89 5a 37 f1 3a 01 00 ff 7f ae 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 34 3e 0c c7 58 df 15 3e 4b fd 08 0d 14 25 f6 b0 06 b8 94 d9 1b 6a 5c 

#  17.571    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x00.\x14b\x81q\xec@\xd7z\x15\xd9l\xf7@:!\x87k\xa6V\x13x\xa9', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 44730301865822L, 'Direct_Address': 0, 'RSSI': -65, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  17.571    COM4 <- 04 3e 39 0d 01 10 00 01 5e df c0 95 ae 28 01 00 ff 7f bf 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 00 2e 14 62 81 71 ec 40 d7 7a 15 d9 6c f7 40 3a 21 87 6b a6 56 13 78 a9 

#  17.602    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0xFF (255)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 1, 'Event_Type': 42, 'Data': 'L7D6g<K\xe7\xe4_\r\x9d\xc0\x9c\x8d\x7f\xda\x80Q\x90\xf4G\xef\xe8\xf9U\xd9\x97isFt\xc5\xd8\x02\x96\xa3\xf8\tdn0*\xdel\x16\xe8\xe1\xc5\xf1\xf6\xb5$\xa8\xfd\xbd\x82,v/\xa5\x94\xabJ\x98\xffLw\x99\xe9\xae\xc7\x16\x02\xd8B\xba\x07\xb3d&Z"fB\xfb\x88T\x94\xfaG\xf5\xc1\xa74b{\xcd\xd4\xff<\xe5\xb9\xf2\xf7\x07\xfa?\x03\xfe\xc8\xfa@\x8d\xb5\xf5[r\xe5\x7f\xe9Q\xb9\xdf\xbf\x8a\x02?07\xfd\xd7_\xe2\x84\x15\x1e;\xb5\x18\x9e\x85\xd5\xf5us3G\xcd9S\x883\xfb!N\xc9\x13Y\xef\x8f"U?s&X\xce\xbdn\x07\x1d1\x1eh\xb3\xf3Q\xd0I\xf1\x15\xeb\xfdn\x83w\xde\xf70z\x077?\xffOPr4\x92_\xf5\xa53\xd76r\x81i\xdf\xdeYj\xfeKI\x84\x90\xde\xcf%\xf0{\xfd0\xdc\x9c\xd6\xf1', 'Data_Length': 229, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 250873233311176L, 'Direct_Address': 0, 'RSSI': -53, 'Address_Type': 0, 'Advertising_SID': 14}]"
#  17.602    COM4 <- 04 3e ff 0d 01 2a 00 00 c8 85 f4 f9 2a e4 01 01 0e 7f cb 00 00 00 00 00 00 00 00 00 e5 4c 37 44 36 67 3c 4b e7 e4 5f 0d 9d c0 9c 8d 7f da 80 51 90 f4 47 ef e8 f9 55 d9 97 69 73 46 74 c5 d8 02 96 a3 f8 09 64 6e 30 2a de 6c 16 e8 e1 c5 f1 f6 b5 24 a8 fd bd 82 2c 76 2f a5 94 ab 4a 98 ff 4c 77 99 e9 ae c7 16 02 d8 42 ba 07 b3 64 26 5a 22 66 42 fb 88 54 94 fa 47 f5 c1 a7 34 62 7b cd d4 ff 3c e5 b9 f2 f7 07 fa 3f 03 fe c8 fa 40 8d b5 f5 5b 72 e5 7f e9 51 b9 df bf 8a 02 3f 30 37 fd d7 5f e2 84 15 1e 3b b5 18 9e 85 d5 f5 75 73 33 47 cd 39 53 88 33 fb 21 4e c9 13 59 ef 8f 22 55 3f 73 26 58 ce bd 6e 07 1d 31 1e 68 b3 f3 51 d0 49 f1 15 eb fd 6e 83 77 de f7 30 7a 07 37 3f ff 4f 50 72 34 92 5f f5 a5 33 d7 36 72 81 69 df de 59 6a fe 4b 49 84 90 de cf 25 f0 7b fd 30 dc 9c d6 f1 

#  17.602    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x35 (53)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 1, 'Event_Type': 10, 'Data': 'O^\x87uM\xe8\\ \xe33\\\xa8\xdbS\x11w\xbf\xdd9\x08\xa9T\xc8\xf3\xca\xe4-', 'Data_Length': 27, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 250873233311176L, 'Direct_Address': 0, 'RSSI': -53, 'Address_Type': 0, 'Advertising_SID': 14}]"
#  17.602    COM4 <- 04 3e 35 0d 01 0a 00 00 c8 85 f4 f9 2a e4 01 01 0e 7f cb 00 00 00 00 00 00 00 00 00 1b 4f 5e 87 75 4d e8 5c 20 e3 33 5c a8 db 53 11 77 bf dd 39 08 a9 54 c8 f3 ca e4 2d 

#  17.665    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x024>\x0c\xc7X\xdf\x15>K\xfd\x08\r\x14%\xf6\xb0\x06\xb8\x94\xd9\x1bj\\', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 64807690209562L, 'Direct_Address': 0, 'RSSI': -75, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  17.665    COM4 <- 04 3e 39 0d 01 10 00 01 1a 89 5a 37 f1 3a 01 00 ff 7f b5 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 34 3e 0c c7 58 df 15 3e 4b fd 08 0d 14 25 f6 b0 06 b8 94 d9 1b 6a 5c 

#  17.743    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x02\xa0\xdd\xd7\xe6\x04\xc1\xdf\r6?\xadcd\xe8\x7fKhrO\x97u\xe6\xf4', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 90398327500093L, 'Direct_Address': 0, 'RSSI': -91, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  17.743    COM4 <- 04 3e 39 0d 01 10 00 01 3d ed 32 80 37 52 01 00 ff 7f a5 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 a0 dd d7 e6 04 c1 df 0d 36 3f ad 63 64 e8 7f 4b 68 72 4f 97 75 e6 f4 

#  17.774    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x34 (52)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x19\xff\x06\x00\x01\t!\n1\x08\x02\xb0\xd3\x04LT12-05006-R', 'Data_Length': 26, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 38460051352148L, 'Direct_Address': 0, 'RSSI': -81, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  17.774    COM4 <- 04 3e 34 0d 01 10 00 01 54 ea b2 ad fa 22 01 00 ff 7f af 00 00 00 00 00 00 00 00 00 1a 19 ff 06 00 01 09 21 0a 31 08 02 b0 d3 04 4c 54 31 32 2d 30 35 30 30 36 2d 52 

#  17.837    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x02/e\xb7\xd5\x1e\t\xc2j\xe2\xc7\t5\x19 .\x02\xa1\x80\xc2j\xfd\xb3^', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 24556672076571L, 'Direct_Address': 0, 'RSSI': -77, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  17.837    COM4 <- 04 3e 39 0d 01 10 00 01 1b 4b b7 8b 55 16 01 00 ff 7f b3 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 2f 65 b7 d5 1e 09 c2 6a e2 c7 09 35 19 20 2e 02 a1 80 c2 6a fd b3 5e 

#  17.868    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x34 (52)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x19\xff\x06\x00\x01\t!\n1\x08\x02\xb0\xd3\x04LT12-05006-R', 'Data_Length': 26, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 38460051352148L, 'Direct_Address': 0, 'RSSI': -91, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  17.868    COM4 <- 04 3e 34 0d 01 10 00 01 54 ea b2 ad fa 22 01 00 ff 7f a5 00 00 00 00 00 00 00 00 00 1a 19 ff 06 00 01 09 21 0a 31 08 02 b0 d3 04 4c 54 31 32 2d 30 35 30 30 36 2d 52 

#  17.915    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x02#\xd9\x07~\x90\xa4\xf2y\xdaa\xbf\xdd\x07\xde$\t\x80\xd88\x96hJj', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 32737509319702L, 'Direct_Address': 0, 'RSSI': -94, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  17.915    COM4 <- 04 3e 39 0d 01 10 00 01 16 24 9d 4b c6 1d 01 00 ff 7f a2 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 23 d9 07 7e 90 a4 f2 79 da 61 bf dd 07 de 24 09 80 d8 38 96 68 4a 6a 

#  17.946    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0xFF (255)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 1, 'Event_Type': 42, 'Data': 'L7D6g<K\xe7\xe4_\r\x9d\xc0\x9c\x8d\x7f\xda\x80Q\x90\xf4G\xef\xe8\xf9U\xd9\x97isFt\xc5\xd8\x02\x96\xa3\xf8\tdn0*\xdel\x16\xe8\xe1\xc5\xf1\xf6\xb5$\xa8\xfd\xbd\x82,v/\xa5\x94\xabJ\x98\xffLw\x99\xe9\xae\xc7\x16\x02\xd8B\xba\x07\xb3d&Z"fB\xfb\x88T\x94\xfaG\xf5\xc1\xa74b{\xcd\xd4\xff<\xe5\xb9\xf2\xf7\x07\xfa?\x03\xfe\xc8\xfa@\x8d\xb5\xf5[r\xe5\x7f\xe9Q\xb9\xdf\xbf\x8a\x02?07\xfd\xd7_\xe2\x84\x15\x1e;\xb5\x18\x9e\x85\xd5\xf5us3G\xcd9S\x883\xfb!N\xc9\x13Y\xef\x8f"U?s&X\xce\xbdn\x07\x1d1\x1eh\xb3\xf3Q\xd0I\xf1\x15\xeb\xfdn\x83w\xde\xf70z\x077?\xffOPr4\x92_\xf5\xa53\xd76r\x81i\xdf\xdeYj\xfeKI\x84\x90\xde\xcf%\xf0{\xfd0\xdc\x9c\xd6\xf1', 'Data_Length': 229, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 250873233311176L, 'Direct_Address': 0, 'RSSI': -41, 'Address_Type': 0, 'Advertising_SID': 14}]"
#  17.946    COM4 <- 04 3e ff 0d 01 2a 00 00 c8 85 f4 f9 2a e4 01 01 0e 7f d7 00 00 00 00 00 00 00 00 00 e5 4c 37 44 36 67 3c 4b e7 e4 5f 0d 9d c0 9c 8d 7f da 80 51 90 f4 47 ef e8 f9 55 d9 97 69 73 46 74 c5 d8 02 96 a3 f8 09 64 6e 30 2a de 6c 16 e8 e1 c5 f1 f6 b5 24 a8 fd bd 82 2c 76 2f a5 94 ab 4a 98 ff 4c 77 99 e9 ae c7 16 02 d8 42 ba 07 b3 64 26 5a 22 66 42 fb 88 54 94 fa 47 f5 c1 a7 34 62 7b cd d4 ff 3c e5 b9 f2 f7 07 fa 3f 03 fe c8 fa 40 8d b5 f5 5b 72 e5 7f e9 51 b9 df bf 8a 02 3f 30 37 fd d7 5f e2 84 15 1e 3b b5 18 9e 85 d5 f5 75 73 33 47 cd 39 53 88 33 fb 21 4e c9 13 59 ef 8f 22 55 3f 73 26 58 ce bd 6e 07 1d 31 1e 68 b3 f3 51 d0 49 f1 15 eb fd 6e 83 77 de f7 30 7a 07 37 3f ff 4f 50 72 34 92 5f f5 a5 33 d7 36 72 81 69 df de 59 6a fe 4b 49 84 90 de cf 25 f0 7b fd 30 dc 9c d6 f1 

#  17.946    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x35 (53)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 1, 'Event_Type': 10, 'Data': 'O^\x87uM\xe8\\ \xe33\\\xa8\xdbS\x11w\xbf\xdd9\x08\xa9T\xc8\xf3\xca\xe4-', 'Data_Length': 27, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 250873233311176L, 'Direct_Address': 0, 'RSSI': -41, 'Address_Type': 0, 'Advertising_SID': 14}]"
#  17.946    COM4 <- 04 3e 35 0d 01 0a 00 00 c8 85 f4 f9 2a e4 01 01 0e 7f d7 00 00 00 00 00 00 00 00 00 1b 4f 5e 87 75 4d e8 5c 20 e3 33 5c a8 db 53 11 77 bf dd 39 08 a9 54 c8 f3 ca e4 2d 

#  17.977    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x00.\x14b\x81q\xec@\xd7z\x15\xd9l\xf7@:!\x87k\xa6V\x13x\xa9', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 44730301865822L, 'Direct_Address': 0, 'RSSI': -65, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  17.977    COM4 <- 04 3e 39 0d 01 10 00 01 5e df c0 95 ae 28 01 00 ff 7f bf 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 00 2e 14 62 81 71 ec 40 d7 7a 15 d9 6c f7 40 3a 21 87 6b a6 56 13 78 a9 

#  17.977    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x34 (52)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x19\xff\x06\x00\x01\t!\n1\x08\x02\xb0\xd3\x04LT12-05006-R', 'Data_Length': 26, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 38460051352148L, 'Direct_Address': 0, 'RSSI': -94, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  17.977    COM4 <- 04 3e 34 0d 01 10 00 01 54 ea b2 ad fa 22 01 00 ff 7f a2 00 00 00 00 00 00 00 00 00 1a 19 ff 06 00 01 09 21 0a 31 08 02 b0 d3 04 4c 54 31 32 2d 30 35 30 30 36 2d 52 

#  18.024    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x02#\xd9\x07~\x90\xa4\xf2y\xdaa\xbf\xdd\x07\xde$\t\x80\xd88\x96hJj', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 32737509319702L, 'Direct_Address': 0, 'RSSI': -80, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  18.024    COM4 <- 04 3e 39 0d 01 10 00 01 16 24 9d 4b c6 1d 01 00 ff 7f b0 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 23 d9 07 7e 90 a4 f2 79 da 61 bf dd 07 de 24 09 80 d8 38 96 68 4a 6a 

#  18.196    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x34 (52)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x19\xff\x06\x00\x01\t!\n1\x08\x02\xb0\xd3\x04LT12-05006-R', 'Data_Length': 26, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 38460051352148L, 'Direct_Address': 0, 'RSSI': -81, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  18.196    COM4 <- 04 3e 34 0d 01 10 00 01 54 ea b2 ad fa 22 01 00 ff 7f af 00 00 00 00 00 00 00 00 00 1a 19 ff 06 00 01 09 21 0a 31 08 02 b0 d3 04 4c 54 31 32 2d 30 35 30 30 36 2d 52 

#  18.227    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0xFF (255)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 1, 'Event_Type': 42, 'Data': 'L7D6g<K\xe7\xe4_\r\x9d\xc0\x9c\x8d\x7f\xda\x80Q\x90\xf4G\xef\xe8\xf9U\xd9\x97isFt\xc5\xd8\x02\x96\xa3\xf8\tdn0*\xdel\x16\xe8\xe1\xc5\xf1\xf6\xb5$\xa8\xfd\xbd\x82,v/\xa5\x94\xabJ\x98\xffLw\x99\xe9\xae\xc7\x16\x02\xd8B\xba\x07\xb3d&Z"fB\xfb\x88T\x94\xfaG\xf5\xc1\xa74b{\xcd\xd4\xff<\xe5\xb9\xf2\xf7\x07\xfa?\x03\xfe\xc8\xfa@\x8d\xb5\xf5[r\xe5\x7f\xe9Q\xb9\xdf\xbf\x8a\x02?07\xfd\xd7_\xe2\x84\x15\x1e;\xb5\x18\x9e\x85\xd5\xf5us3G\xcd9S\x883\xfb!N\xc9\x13Y\xef\x8f"U?s&X\xce\xbdn\x07\x1d1\x1eh\xb3\xf3Q\xd0I\xf1\x15\xeb\xfdn\x83w\xde\xf70z\x077?\xffOPr4\x92_\xf5\xa53\xd76r\x81i\xdf\xdeYj\xfeKI\x84\x90\xde\xcf%\xf0{\xfd0\xdc\x9c\xd6\xf1', 'Data_Length': 229, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 250873233311176L, 'Direct_Address': 0, 'RSSI': -46, 'Address_Type': 0, 'Advertising_SID': 14}]"
#  18.227    COM4 <- 04 3e ff 0d 01 2a 00 00 c8 85 f4 f9 2a e4 01 01 0e 7f d2 00 00 00 00 00 00 00 00 00 e5 4c 37 44 36 67 3c 4b e7 e4 5f 0d 9d c0 9c 8d 7f da 80 51 90 f4 47 ef e8 f9 55 d9 97 69 73 46 74 c5 d8 02 96 a3 f8 09 64 6e 30 2a de 6c 16 e8 e1 c5 f1 f6 b5 24 a8 fd bd 82 2c 76 2f a5 94 ab 4a 98 ff 4c 77 99 e9 ae c7 16 02 d8 42 ba 07 b3 64 26 5a 22 66 42 fb 88 54 94 fa 47 f5 c1 a7 34 62 7b cd d4 ff 3c e5 b9 f2 f7 07 fa 3f 03 fe c8 fa 40 8d b5 f5 5b 72 e5 7f e9 51 b9 df bf 8a 02 3f 30 37 fd d7 5f e2 84 15 1e 3b b5 18 9e 85 d5 f5 75 73 33 47 cd 39 53 88 33 fb 21 4e c9 13 59 ef 8f 22 55 3f 73 26 58 ce bd 6e 07 1d 31 1e 68 b3 f3 51 d0 49 f1 15 eb fd 6e 83 77 de f7 30 7a 07 37 3f ff 4f 50 72 34 92 5f f5 a5 33 d7 36 72 81 69 df de 59 6a fe 4b 49 84 90 de cf 25 f0 7b fd 30 dc 9c d6 f1 

#  18.227    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x35 (53)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 1, 'Event_Type': 10, 'Data': 'O^\x87uM\xe8\\ \xe33\\\xa8\xdbS\x11w\xbf\xdd9\x08\xa9T\xc8\xf3\xca\xe4-', 'Data_Length': 27, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 250873233311176L, 'Direct_Address': 0, 'RSSI': -46, 'Address_Type': 0, 'Advertising_SID': 14}]"
#  18.227    COM4 <- 04 3e 35 0d 01 0a 00 00 c8 85 f4 f9 2a e4 01 01 0e 7f d2 00 00 00 00 00 00 00 00 00 1b 4f 5e 87 75 4d e8 5c 20 e3 33 5c a8 db 53 11 77 bf dd 39 08 a9 54 c8 f3 ca e4 2d 

#  18.258    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x02/e\xb7\xd5\x1e\t\xc2j\xe2\xc7\t5\x19 .\x02\xa1\x80\xc2j\xfd\xb3^', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 24556672076571L, 'Direct_Address': 0, 'RSSI': -78, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  18.258    COM4 <- 04 3e 39 0d 01 10 00 01 1b 4b b7 8b 55 16 01 00 ff 7f b2 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 2f 65 b7 d5 1e 09 c2 6a e2 c7 09 35 19 20 2e 02 a1 80 c2 6a fd b3 5e 

#  18.290    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x00.\x14b\x81q\xec@\xd7z\x15\xd9l\xf7@:!\x87k\xa6V\x13x\xa9', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 44730301865822L, 'Direct_Address': 0, 'RSSI': -100, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  18.290    COM4 <- 04 3e 39 0d 01 10 00 01 5e df c0 95 ae 28 01 00 ff 7f 9c 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 00 2e 14 62 81 71 ec 40 d7 7a 15 d9 6c f7 40 3a 21 87 6b a6 56 13 78 a9 

#  18.430    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x02#\xd9\x07~\x90\xa4\xf2y\xdaa\xbf\xdd\x07\xde$\t\x80\xd88\x96hJj', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 32737509319702L, 'Direct_Address': 0, 'RSSI': -78, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  18.430    COM4 <- 04 3e 39 0d 01 10 00 01 16 24 9d 4b c6 1d 01 00 ff 7f b2 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 23 d9 07 7e 90 a4 f2 79 da 61 bf dd 07 de 24 09 80 d8 38 96 68 4a 6a 

#  18.477    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0xFF (255)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 3, 'Secondary_PHY': 3, 'Event_Type': 42, 'Data': "\xabR\x9da\r\xc5\xe0\xe1N\x8d\xc3\x93\xba\xdey*\xa2V\x17\xd4S\x9e\x91\xd7\xbbTV\xfd}w\xc3\x8f\x85v\xac\xb7ciLX.87\x90`]\xa4n\xffu66\xe6\xa0\xf6\x7f\xf9\x03\x90\x94\x02\x9b%p\xcc\x05W|\x13\x9a\x95\x07\x17\x8cVu\x06\xd8J\x89\x80\x13\xbd\xf3g\x9e\xfcZ['\xa6\xdc\xde\xfb\xd4\x18Xo\xe9 -\xdd\x1a.4\x8eS>\x82\xf1\xdd\xae(\xe4\xdd$\xd8\t\xd3\xbdaT\xe2\xd3D\xe2/\xef9\x96P\xbd\xecZsq\xf1z1S\x8dA\x86\xa6kV\xef\xa8\x11\xd1\xe44\xd8\xdb\x92\x1c\x87\xde?\xd3TK\xa6O\xac\xd0\xaa\x1aG\x95\xf7c<\x8d>I\xbbDX\xe2\xc8\xe8\x05\x8b\xdd\xbd\xff)\x81\x9c`\xbff0\x97\x88\x80\xb5\x90\x12/@\xf7F)\xcf\x81P\nb\xe5\x952\xab\xb36\rnl'\n,\x03\xd6\xab{H|Q", 'Data_Length': 229, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 250873233311176L, 'Direct_Address': 0, 'RSSI': 0, 'Address_Type': 0, 'Advertising_SID': 12}]"
#  18.477    COM4 <- 04 3e ff 0d 01 2a 00 00 c8 85 f4 f9 2a e4 03 03 0c 7f 00 00 00 00 00 00 00 00 00 00 e5 ab 52 9d 61 0d c5 e0 e1 4e 8d c3 93 ba de 79 2a a2 56 17 d4 53 9e 91 d7 bb 54 56 fd 7d 77 c3 8f 85 76 ac b7 63 69 4c 58 2e 38 37 90 60 5d a4 6e ff 75 36 36 e6 a0 f6 7f f9 03 90 94 02 9b 25 70 cc 05 57 7c 13 9a 95 07 17 8c 56 75 06 d8 4a 89 80 13 bd f3 67 9e fc 5a 5b 27 a6 dc de fb d4 18 58 6f e9 20 2d dd 1a 2e 34 8e 53 3e 82 f1 dd ae 28 e4 dd 24 d8 09 d3 bd 61 54 e2 d3 44 e2 2f ef 39 96 50 bd ec 5a 73 71 f1 7a 31 53 8d 41 86 a6 6b 56 ef a8 11 d1 e4 34 d8 db 92 1c 87 de 3f d3 54 4b a6 4f ac d0 aa 1a 47 95 f7 63 3c 8d 3e 49 bb 44 58 e2 c8 e8 05 8b dd bd ff 29 81 9c 60 bf 66 30 97 88 80 b5 90 12 2f 40 f7 46 29 cf 81 50 0a 62 e5 95 32 ab b3 36 0d 6e 6c 27 0a 2c 03 d6 ab 7b 48 7c 51 

#  18.477    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x35 (53)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 3, 'Secondary_PHY': 3, 'Event_Type': 10, 'Data': '\xf0\xa8\x06\x11c\x1a\x1f\xd2-mM\xb7\tD\xd1"\x95\x11b\x1e\xc5\xf2\xadd\xe50\x1d', 'Data_Length': 27, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 250873233311176L, 'Direct_Address': 0, 'RSSI': 0, 'Address_Type': 0, 'Advertising_SID': 12}]"
#  18.477    COM4 <- 04 3e 35 0d 01 0a 00 00 c8 85 f4 f9 2a e4 03 03 0c 7f 00 00 00 00 00 00 00 00 00 00 1b f0 a8 06 11 63 1a 1f d2 2d 6d 4d b7 09 44 d1 22 95 11 62 1e c5 f2 ad 64 e5 30 1d 

#  18.508    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x024>\x0c\xc7X\xdf\x15>K\xfd\x08\r\x14%\xf6\xb0\x06\xb8\x94\xd9\x1bj\\', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 64807690209562L, 'Direct_Address': 0, 'RSSI': -88, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  18.508    COM4 <- 04 3e 39 0d 01 10 00 01 1a 89 5a 37 f1 3a 01 00 ff 7f a8 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 34 3e 0c c7 58 df 15 3e 4b fd 08 0d 14 25 f6 b0 06 b8 94 d9 1b 6a 5c 

#  18.571    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x02/e\xb7\xd5\x1e\t\xc2j\xe2\xc7\t5\x19 .\x02\xa1\x80\xc2j\xfd\xb3^', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 24556672076571L, 'Direct_Address': 0, 'RSSI': -82, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  18.571    COM4 <- 04 3e 39 0d 01 10 00 01 1b 4b b7 8b 55 16 01 00 ff 7f ae 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 2f 65 b7 d5 1e 09 c2 6a e2 c7 09 35 19 20 2e 02 a1 80 c2 6a fd b3 5e 

#  18.618    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x024>\x0c\xc7X\xdf\x15>K\xfd\x08\r\x14%\xf6\xb0\x06\xb8\x94\xd9\x1bj\\', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 64807690209562L, 'Direct_Address': 0, 'RSSI': -80, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  18.618    COM4 <- 04 3e 39 0d 01 10 00 01 1a 89 5a 37 f1 3a 01 00 ff 7f b0 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 34 3e 0c c7 58 df 15 3e 4b fd 08 0d 14 25 f6 b0 06 b8 94 d9 1b 6a 5c 

#  18.774    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0xFF (255)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 1, 'Event_Type': 42, 'Data': '\xc1\x13\xc8P\x1102mH\x9a\xd2\x8c\x82\xfd\x92\x0b\x0b|\xdb>l\x0f\xf3u\x8cg4\xd6\x9f\x96 \xe4\xfe\x17\x0e\xce\xb5\x1e\xe6K\x1da\xca\xe4\x12}\xc3\xa2\xae\xf0\xb0i\xeb\xc6/\xb1Go\xf60\xe5\x05\x95\xbb\xf3\xcc\x08\x05\x0cP\xe1B\xf0)l\xc3(\xccZ\xeaLa\xcc\xdb\xe7`\xadh!\xbb\xa1\xc0\xdfy\xb7\xa9U\xb4I\xb8e\xf9\xb8\x10\x1f\'\x18\xdbM\x96\xaf\xeb:\xed"\x9f\x11\x12\xec\x95\x90\x1ea"\xb8\xe4=\x0f\xe5\x8d\x9c\x1b\x1bG\xfc\xf9\xbd\xf3 \xe7\xc4\xde\xc25\xda\x87.\x97\x1c\x89w\x0c\x8e\x88\xe9`\xe0$:\xca\x8e\x1b\x17!\x0e\xf6\xb5\xb6nf\x95&\x1d/Z.\xb2g;\xfe\xff\x88\x87\xe5\xf4b\xeb\xf0zh\xcd\x06\xa4\xa4\xe9_Yx\x13Lf8\x94Nj\xc6\xbeIG\xc8\xe1\xb2\xb0@\xd0J|09Wp/\xf2\xd0\xc2\x14\xe3U}', 'Data_Length': 229, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 250873233311176L, 'Direct_Address': 0, 'RSSI': -50, 'Address_Type': 0, 'Advertising_SID': 15}]"
#  18.774    COM4 <- 04 3e ff 0d 01 2a 00 00 c8 85 f4 f9 2a e4 01 01 0f 7f ce 00 00 00 00 00 00 00 00 00 e5 c1 13 c8 50 11 30 32 6d 48 9a d2 8c 82 fd 92 0b 0b 7c db 3e 6c 0f f3 75 8c 67 34 d6 9f 96 20 e4 fe 17 0e ce b5 1e e6 4b 1d 61 ca e4 12 7d c3 a2 ae f0 b0 69 eb c6 2f b1 47 6f f6 30 e5 05 95 bb f3 cc 08 05 0c 50 e1 42 f0 29 6c c3 28 cc 5a ea 4c 61 cc db e7 60 ad 68 21 bb a1 c0 df 79 b7 a9 55 b4 49 b8 65 f9 b8 10 1f 27 18 db 4d 96 af eb 3a ed 22 9f 11 12 ec 95 90 1e 61 22 b8 e4 3d 0f e5 8d 9c 1b 1b 47 fc f9 bd f3 20 e7 c4 de c2 35 da 87 2e 97 1c 89 77 0c 8e 88 e9 60 e0 24 3a ca 8e 1b 17 21 0e f6 b5 b6 6e 66 95 26 1d 2f 5a 2e b2 67 3b fe ff 88 87 e5 f4 62 eb f0 7a 68 cd 06 a4 a4 e9 5f 59 78 13 4c 66 38 94 4e 6a c6 be 49 47 c8 e1 b2 b0 40 d0 4a 7c 30 39 57 70 2f f2 d0 c2 14 e3 55 7d 

#  18.774    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x35 (53)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 1, 'Event_Type': 10, 'Data': '. \\\xacR\xf1s\x84\x87\xc4S\x85\xb5\xb6\xa7\xdbTy\x99\x87\xd8\xb4\x94\xfc\xc9r\xc4', 'Data_Length': 27, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 250873233311176L, 'Direct_Address': 0, 'RSSI': -50, 'Address_Type': 0, 'Advertising_SID': 15}]"
#  18.774    COM4 <- 04 3e 35 0d 01 0a 00 00 c8 85 f4 f9 2a e4 01 01 0f 7f ce 00 00 00 00 00 00 00 00 00 1b 2e 20 5c ac 52 f1 73 84 87 c4 53 85 b5 b6 a7 db 54 79 99 87 d8 b4 94 fc c9 72 c4 

#  18.836    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x1A (26)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 1, 'Event_Type': 2, 'Data': '', 'Data_Length': 0, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 250873233311176L, 'Direct_Address': 0, 'RSSI': -56, 'Address_Type': 0, 'Advertising_SID': 14}]"
#  18.836    COM4 <- 04 3e 1a 0d 01 02 00 00 c8 85 f4 f9 2a e4 01 01 0e 7f c8 00 00 00 00 00 00 00 00 00 00 

#  18.883    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x02\xa0\xdd\xd7\xe6\x04\xc1\xdf\r6?\xadcd\xe8\x7fKhrO\x97u\xe6\xf4', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 90398327500093L, 'Direct_Address': 0, 'RSSI': -92, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  18.883    COM4 <- 04 3e 39 0d 01 10 00 01 3d ed 32 80 37 52 01 00 ff 7f a4 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 a0 dd d7 e6 04 c1 df 0d 36 3f ad 63 64 e8 7f 4b 68 72 4f 97 75 e6 f4 

#  18.993    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x02/e\xb7\xd5\x1e\t\xc2j\xe2\xc7\t5\x19 .\x02\xa1\x80\xc2j\xfd\xb3^', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 24556672076571L, 'Direct_Address': 0, 'RSSI': -83, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  18.993    COM4 <- 04 3e 39 0d 01 10 00 01 1b 4b b7 8b 55 16 01 00 ff 7f ad 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 2f 65 b7 d5 1e 09 c2 6a e2 c7 09 35 19 20 2e 02 a1 80 c2 6a fd b3 5e 

#  19.086    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0xFF (255)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 2, 'Event_Type': 42, 'Data': "Y\xd6\x91\x16\xee\x83\xbes\xaf\xae$\x91\xc8\x9c\xe8\xf5\x19-\x8d\xc18\xa2\xcf\x839p\xebQ\xc9\xc7\xfa\xf9\x8as@\x1e\xef\x8f\x8c\xb0\xfed5!\xe32\xc4\x92\xfeJ\xb7<cZ.\xe8?\x1f\xad\xc7p\xd9\x86\x0f-C\xf0\x83\x18\xa9\xefr0\x11'*{\xf5\x18\xd9d<\xd2\xcf\xeb\x15\x8f\xda}0\x7f\x9b\xd7\x02\x90\xaa5\xdd\xe0\xe2\x1e\x16\xc0\xe9/[\x9ag\xfb\x1d\xd3\x01b~q\x02V\xc8\xa7\xfe\x1d\xc4Z\x0f}\xc77V\xb6\x12\x99\x86\x1f\xdbe\x89\xd8Ia?\x1f\x08\x01\xe9]i\xf8A\x1b\xf8\x15\xf9\xd2\x96\x92\x87x\xb4AL \xd6\xf9\xe8\xb77P`[\xd206\x80\xe9\x91\xda5\xcc\x0f\xbb\xfa\x87\xce!\x07\xb6P\xc4\x17s\xec^T7@\xfa\xb0P\xae\xff!x$\x04\xce\x9a\x13/j\xe5\x85\xd1\xb4\xdde#gH+\xd2\x8c\x19\xa5\xe4R\x01O(@", 'Data_Length': 229, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 250873233311176L, 'Direct_Address': 0, 'RSSI': -58, 'Address_Type': 0, 'Advertising_SID': 11}]"
#  19.086    COM4 <- 04 3e ff 0d 01 2a 00 00 c8 85 f4 f9 2a e4 01 02 0b 7f c6 00 00 00 00 00 00 00 00 00 e5 59 d6 91 16 ee 83 be 73 af ae 24 91 c8 9c e8 f5 19 2d 8d c1 38 a2 cf 83 39 70 eb 51 c9 c7 fa f9 8a 73 40 1e ef 8f 8c b0 fe 64 35 21 e3 32 c4 92 fe 4a b7 3c 63 5a 2e e8 3f 1f ad c7 70 d9 86 0f 2d 43 f0 83 18 a9 ef 72 30 11 27 2a 7b f5 18 d9 64 3c d2 cf eb 15 8f da 7d 30 7f 9b d7 02 90 aa 35 dd e0 e2 1e 16 c0 e9 2f 5b 9a 67 fb 1d d3 01 62 7e 71 02 56 c8 a7 fe 1d c4 5a 0f 7d c7 37 56 b6 12 99 86 1f db 65 89 d8 49 61 3f 1f 08 01 e9 5d 69 f8 41 1b f8 15 f9 d2 96 92 87 78 b4 41 4c 20 d6 f9 e8 b7 37 50 60 5b d2 30 36 80 e9 91 da 35 cc 0f bb fa 87 ce 21 07 b6 50 c4 17 73 ec 5e 54 37 40 fa b0 50 ae ff 21 78 24 04 ce 9a 13 2f 6a e5 85 d1 b4 dd 65 23 67 48 2b d2 8c 19 a5 e4 52 01 4f 28 40 

#  19.086    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x35 (53)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 2, 'Event_Type': 10, 'Data': 'x\xc4\xdbq;*\x8f\xe2/\xebB\xd7\x9c\xf5$8fw\xdf\xa5Q+\x0f\x19h$\x8c', 'Data_Length': 27, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 250873233311176L, 'Direct_Address': 0, 'RSSI': -58, 'Address_Type': 0, 'Advertising_SID': 11}]"
#  19.086    COM4 <- 04 3e 35 0d 01 0a 00 00 c8 85 f4 f9 2a e4 01 02 0b 7f c6 00 00 00 00 00 00 00 00 00 1b 78 c4 db 71 3b 2a 8f e2 2f eb 42 d7 9c f5 24 38 66 77 df a5 51 2b 0f 19 68 24 8c 

#  19.149    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x34 (52)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x19\xff\x06\x00\x01\t!\n1\x08\x02\xb0\xd3\x04LT12-05006-R', 'Data_Length': 26, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 38460051352148L, 'Direct_Address': 0, 'RSSI': -81, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  19.149    COM4 <- 04 3e 34 0d 01 10 00 01 54 ea b2 ad fa 22 01 00 ff 7f af 00 00 00 00 00 00 00 00 00 1a 19 ff 06 00 01 09 21 0a 31 08 02 b0 d3 04 4c 54 31 32 2d 30 35 30 30 36 2d 52 

#  19.149    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x00.\x14b\x81q\xec@\xd7z\x15\xd9l\xf7@:!\x87k\xa6V\x13x\xa9', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 44730301865822L, 'Direct_Address': 0, 'RSSI': -69, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  19.149    COM4 <- 04 3e 39 0d 01 10 00 01 5e df c0 95 ae 28 01 00 ff 7f bb 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 00 2e 14 62 81 71 ec 40 d7 7a 15 d9 6c f7 40 3a 21 87 6b a6 56 13 78 a9 

#  19.149    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x39 (57)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 0, 'Event_Type': 16, 'Data': '\x1e\xff\x06\x00\x01\t \x024>\x0c\xc7X\xdf\x15>K\xfd\x08\r\x14%\xf6\xb0\x06\xb8\x94\xd9\x1bj\\', 'Data_Length': 31, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 64807690209562L, 'Direct_Address': 0, 'RSSI': -76, 'Address_Type': 1, 'Advertising_SID': 255}]"
#  19.149    COM4 <- 04 3e 39 0d 01 10 00 01 1a 89 5a 37 f1 3a 01 00 ff 7f b4 00 00 00 00 00 00 00 00 00 1f 1e ff 06 00 01 09 20 02 34 3e 0c c7 58 df 15 3e 4b fd 08 0d 14 25 f6 b0 06 b8 94 d9 1b 6a 5c 

#  19.164    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0xFF (255)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 1, 'Event_Type': 42, 'Data': 'L7D6g<K\xe7\xe4_\r\x9d\xc0\x9c\x8d\x7f\xda\x80Q\x90\xf4G\xef\xe8\xf9U\xd9\x97isFt\xc5\xd8\x02\x96\xa3\xf8\tdn0*\xdel\x16\xe8\xe1\xc5\xf1\xf6\xb5$\xa8\xfd\xbd\x82,v/\xa5\x94\xabJ\x98\xffLw\x99\xe9\xae\xc7\x16\x02\xd8B\xba\x07\xb3d&Z"fB\xfb\x88T\x94\xfaG\xf5\xc1\xa74b{\xcd\xd4\xff<\xe5\xb9\xf2\xf7\x07\xfa?\x03\xfe\xc8\xfa@\x8d\xb5\xf5[r\xe5\x7f\xe9Q\xb9\xdf\xbf\x8a\x02?07\xfd\xd7_\xe2\x84\x15\x1e;\xb5\x18\x9e\x85\xd5\xf5us3G\xcd9S\x883\xfb!N\xc9\x13Y\xef\x8f"U?s&X\xce\xbdn\x07\x1d1\x1eh\xb3\xf3Q\xd0I\xf1\x15\xeb\xfdn\x83w\xde\xf70z\x077?\xffOPr4\x92_\xf5\xa53\xd76r\x81i\xdf\xdeYj\xfeKI\x84\x90\xde\xcf%\xf0{\xfd0\xdc\x9c\xd6\xf1', 'Data_Length': 229, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 250873233311176L, 'Direct_Address': 0, 'RSSI': -60, 'Address_Type': 0, 'Advertising_SID': 14}]"
#  19.164    COM4 <- 04 3e ff 0d 01 2a 00 00 c8 85 f4 f9 2a e4 01 01 0e 7f c4 00 00 00 00 00 00 00 00 00 e5 4c 37 44 36 67 3c 4b e7 e4 5f 0d 9d c0 9c 8d 7f da 80 51 90 f4 47 ef e8 f9 55 d9 97 69 73 46 74 c5 d8 02 96 a3 f8 09 64 6e 30 2a de 6c 16 e8 e1 c5 f1 f6 b5 24 a8 fd bd 82 2c 76 2f a5 94 ab 4a 98 ff 4c 77 99 e9 ae c7 16 02 d8 42 ba 07 b3 64 26 5a 22 66 42 fb 88 54 94 fa 47 f5 c1 a7 34 62 7b cd d4 ff 3c e5 b9 f2 f7 07 fa 3f 03 fe c8 fa 40 8d b5 f5 5b 72 e5 7f e9 51 b9 df bf 8a 02 3f 30 37 fd d7 5f e2 84 15 1e 3b b5 18 9e 85 d5 f5 75 73 33 47 cd 39 53 88 33 fb 21 4e c9 13 59 ef 8f 22 55 3f 73 26 58 ce bd 6e 07 1d 31 1e 68 b3 f3 51 d0 49 f1 15 eb fd 6e 83 77 de f7 30 7a 07 37 3f ff 4f 50 72 34 92 5f f5 a5 33 d7 36 72 81 69 df de 59 6a fe 4b 49 84 90 de cf 25 f0 7b fd 30 dc 9c d6 f1 

#  19.164    [HCI - COM4] HCI event received: SUBEVT_LE_EXT_ADV_REPORT_EVT (0x3E)
#                              Event_Code = 0x3E (62)
#                              Num_Reports = 0x1 (1)
#                              Parameter_Length = 0x35 (53)
#                              Subevent_Code = 0xD (13)
#                              report = "[{'Primary_PHY': 1, 'Secondary_PHY': 1, 'Event_Type': 10, 'Data': 'O^\x87uM\xe8\\ \xe33\\\xa8\xdbS\x11w\xbf\xdd9\x08\xa9T\xc8\xf3\xca\xe4-', 'Data_Length': 27, 'Direct_Address_Type': 0, 'Periodic_Advertising_Interval': 0, 'Tx_Power': 127, 'Address': 250873233311176L, 'Direct_Address': 0, 'RSSI': -60, 'Address_Type': 0, 'Advertising_SID': 14}]"
#  19.164    COM4 <- 04 3e 35 0d 01 0a 00 00 c8 85 f4 f9 2a e4 01 01 0e 7f c4 00 00 00 00 00 00 00 00 00 1b 4f 5e 87 75 4d e8 5c 20 e3 33 5c a8 db 53 11 77 bf dd 39 08 a9 54 c8 f3 ca e4 2d 

while(1):
  rcv_event()
  