#! /usr/bin/env python

filename="../common.py"
exec(open(filename).read())

#   4.969    [HCI - COM19] Reset()
#   4.969    COM19 -> 01 03 0c 00 
send_command("01030c00")

#   5.032    [HCI - COM19] HCI event received: OPCODE_RESET (0xE)
#                              Command_Opcode = 0xC03 (3075)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   5.032    COM19 <- 04 0e 04 01 03 0c 00 

#   5.047    [HCI - COM19] Reset()
#   5.047    COM19 -> 01 03 0c 00 
send_command("01030c00")

#   5.110    [HCI - COM19] HCI event received: OPCODE_RESET (0xE)
#                              Command_Opcode = 0xC03 (3075)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   5.110    COM19 <- 04 0e 04 01 03 0c 00 

#   5.922    [HCI - COM19] ReadBdAddr()
#   5.922    COM19 -> 01 09 10 00 
send_command("01091000")

#   5.985    [HCI - COM19] HCI event received: OPCODE_READ_BD_ADDR (0xE)
#                              BD_ADDR = 0x5F123456 (1595028566)
#                              Command_Opcode = 0x1009 (4105)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0xA (10)
#                              Status = 0x0 (0)
#   5.985    COM19 <- 04 0e 0a 01 09 10 00 56 34 12 5f 00 00 

#   6.078    [HCI - COM19] SetEventMask(Event_Mask=2305983746702149776L)
#   6.078    COM19 -> 01 01 0c 08 90 88 01 00 00 80 00 20 
send_command("01010c089088010000800020")

#   6.266    [HCI - COM19] HCI event received: OPCODE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0xC01 (3073)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.266    COM19 <- 04 0e 04 01 01 0c 00 

#   6.282    [HCI - COM19] SetEventMaskPage2(Event_Mask_Page_2=8388608)
#   6.282    COM19 -> 01 63 0c 08 00 00 80 00 00 00 00 00 
send_command("01630c080000800000000000")

#   6.469    [HCI - COM19] HCI event received: OPCODE_SET_EVENT_MASK_PAGE2 (0xE)
#                              Command_Opcode = 0xC63 (3171)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.469    COM19 <- 04 0e 04 01 63 0c 00 

#   6.751    [HCI - COM19] ReadLocalVerInfo()
#   6.751    COM19 -> 01 01 10 00 
send_command("01011000")

#   6.813    [HCI - COM19] HCI event received: OPCODE_READ_LOCAL_VER_INFO (0xE)
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
#   6.813    COM19 <- 04 0e 0c 01 01 10 00 09 03 23 09 5f 00 03 23 

#   6.953    [HCI - COM19] LeReadLocalSupFeat()
#   6.953    COM19 -> 01 03 20 00 
send_command("01032000")

#   7.016    [HCI - COM19] HCI event received: OPCODE_LE_READ_LOCAL_SUP_FEAT (0xE)
#                              Command_Opcode = 0x2003 (8195)
#                              Event_Code = 0xE (14)
#                              LE_Features = 0x80179FF (134314495)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0xC (12)
#                              Status = 0x0 (0)
#   7.016    COM19 <- 04 0e 0c 01 03 20 00 ff 79 01 08 00 00 00 00 

#   7.094    [HCI - COM19] LeReadBufSize()
#   7.094    COM19 -> 01 02 20 00 
send_command("01022000")

#   7.157    [HCI - COM19] HCI event received: OPCODE_LE_READ_BUF_SIZE (0xE)
#                              Command_Opcode = 0x2002 (8194)
#                              Event_Code = 0xE (14)
#                              HC_LE_ACL_Data_Packet_Length = 0x200 (512)
#                              HC_Total_Num_LE_ACL_Data_Packets = 0x10 (16)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x7 (7)
#                              Status = 0x0 (0)
#   7.157    COM19 <- 04 0e 07 01 02 20 00 00 02 10 

#   7.172    [HCI - COM19] SetEventMask(Event_Mask=2305983746702149776L)
#   7.172    COM19 -> 01 01 0c 08 90 88 01 00 00 80 00 20 
send_command("01010c089088010000800020")

#   7.360    [HCI - COM19] HCI event received: OPCODE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0xC01 (3073)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.360    COM19 <- 04 0e 04 01 01 0c 00 

#   7.469    [HCI - COM19] LeAddDevResList(Peer_Identity_Address_Type=0,
#                        Peer_Identity_Address='\xc8\x85\xf4\xf9*\xe4',
#                        Peer_IRK='\x9b}9\n\xa6\x10\x104\x05\xad\xc8W\xa34\x02\xec',
#                        Local_IRK='[M9sV\x90\x16T5\xadUW\xcf\xb4\x02\xea')
#   7.469    COM19 -> 01 27 20 27 00 c8 85 f4 f9 2a e4 9b 7d 39 0a a6 10 10 34 05 ad c8 57 a3 34 02 ec 5b 4d 39 73 56 90 16 54 35 ad 55 57 cf b4 02 ea 
send_command("0127202700c885f4f92ae49b7d390aa610103405adc857a33402ec5b4d39735690165435ad5557cfb402ea")

#   8.141    [HCI - COM19] HCI event received: OPCODE_LE_ADD_DEV_RES_LIST (0xE)
#                              Command_Opcode = 0x2027 (8231)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.141    COM19 <- 04 0e 04 01 27 20 00 

#   8.157    [HCI - COM19] LeSetResPrivAddrTimeout(RPA_Timeout=15)
#   8.157    COM19 -> 01 2e 20 02 0f 00 
send_command("012e20020f00")

#   8.282    [HCI - COM19] HCI event received: OPCODE_LE_SET_RES_PRIV_ADDR_TO (0xE)
#                              Command_Opcode = 0x202E (8238)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.282    COM19 <- 04 0e 04 01 2e 20 00 

#   8.313    [HCI - COM19] LeSetAddrResEnable(Address_Resolution_Enable=True)
#   8.313    COM19 -> 01 2d 20 01 01 
send_command("012d200101")

#   8.391    [HCI - COM19] HCI event received: OPCODE_LE_SET_ADDR_RES_ENABLE (0xE)
#                              Command_Opcode = 0x202D (8237)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.391    COM19 <- 04 0e 04 01 2d 20 00 

#   8.422    [HCI - COM19] LePeriodicAdvCreateSync(Options=0,
#                                Advertising_SID=14,
#                                Advertising_Address_Type=0,
#                                Advertiser_Address='\xc8\x85\xf4\xf9*\xe4',
#                                Skip=0,
#                                Sync_Timeout=8192,
#                                CTE_Type=0)
#   8.422    COM19 -> 01 44 20 0e 00 0e 00 c8 85 f4 f9 2a e4 00 00 00 20 00 
send_command("0144200e000e00c885f4f92ae40000002000")

#   8.703    [HCI - COM19] HCI event received: EVT_CMD_STATUS (0xF)
#                              Command_Opcode = 0x2044 (8260)
#                              Event_Code = 0xF (15)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.703    COM19 <- 04 0f 04 00 01 44 20 

#   8.719    [HCI - COM19] LeAddDevResList(Peer_Identity_Address_Type=0,
#                        Peer_Identity_Address='\xc8\x85\xf4\xf9*\xe4',
#                        Peer_IRK='\x9b}9\n\xa6\x10\x104\x05\xad\xc8W\xa34\x02\xec',
#                        Local_IRK='[M9sV\x90\x16T5\xadUW\xcf\xb4\x02\xea')
#   8.719    COM19 -> 01 27 20 27 00 c8 85 f4 f9 2a e4 9b 7d 39 0a a6 10 10 34 05 ad c8 57 a3 34 02 ec 5b 4d 39 73 56 90 16 54 35 ad 55 57 cf b4 02 ea 
send_command("0127202700c885f4f92ae49b7d390aa610103405adc857a33402ec5b4d39735690165435ad5557cfb402ea")

#   9.407    [HCI - COM19] HCI event received: OPCODE_LE_ADD_DEV_RES_LIST (0xE)
#                              Command_Opcode = 0x2027 (8231)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   9.407    COM19 <- 04 0e 04 01 27 20 00 

#   9.422    [HCI - COM19] Reset()
#   9.422    COM19 -> 01 03 0c 00 
send_command("01030c00")

#   9.485    [HCI - COM19] HCI event received: OPCODE_RESET (0xE)
#                              Command_Opcode = 0xC03 (3075)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   9.485    COM19 <- 04 0e 04 01 03 0c 00 

