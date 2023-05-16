
dev_addr = "5634128B0500"
peer_addr = "B573D26E4DF7"

filename="../common.py"
exec(open(filename).read())

#   5.020    [HCI - COM20] Reset()
#   5.020    COM20 -> 01 03 0c 00 
send_command("01030c00")

#   7.516    [HCI - COM20] LeAddDevResList(Peer_Identity_Address_Type=0,
#                        Peer_Identity_Address='\xb5s\xd2nM\xf7',
#                        Peer_IRK='Hee-Haw!BR-549JS',
#                        Local_IRK='\xcf\xb4\x02\xea5\xadUWV\x90\x16T[M9s')
#   7.516    COM20 -> 01 27 20 27 00 b5 73 d2 6e 4d f7 48 65 65 2d 48 61 77 21 42 52 2d 35 34 39 4a 53 cf b4 02 ea 35 ad 55 57 56 90 16 54 5b 4d 39 73 
send_command("0127202700"+peer_addr+"4865652d4861772142522d3534394a53cfb402ea35ad5557569016545b4d3973")

#   8.172    [HCI - COM20] HCI event received: OPCODE_LE_ADD_DEV_RES_LIST (0xE)
#                              Command_Opcode = 0x2027 (8231)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.172    COM20 <- 04 0e 04 01 27 20 00 

#   8.188    [HCI - COM20] LeSetResPrivAddrTimeout(RPA_Timeout=15)
#   8.188    COM20 -> 01 2e 20 02 0f 00 
send_command("012e20020f00")

#   8.266    [HCI - COM20] HCI event received: OPCODE_LE_SET_RES_PRIV_ADDR_TO (0xE)
#                              Command_Opcode = 0x202E (8238)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.266    COM20 <- 04 0e 04 01 2e 20 00 

#   8.281    [HCI - COM20] LeSetAddrResEnable(Address_Resolution_Enable=True)
#   8.281    COM20 -> 01 2d 20 01 01 
send_command("012d200101")

#   8.344    [HCI - COM20] HCI event received: OPCODE_LE_SET_ADDR_RES_ENABLE (0xE)
#                              Command_Opcode = 0x202D (8237)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.344    COM20 <- 04 0e 04 01 2d 20 00 

#   8.484    [HCI - COM20] LeClearWhiteList()
#   8.484    COM20 -> 01 10 20 00 
send_command("01102000")

#   8.547    [HCI - COM20] HCI event received: OPCODE_LE_CLEAR_WHITE_LIST (0xE)
#                              Command_Opcode = 0x2010 (8208)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.547    COM20 <- 04 0e 04 01 10 20 00 

#   8.563    [HCI - COM20] LeAddDevWhiteList(Address_Type=0,
#                          Address='\xb5s\xd2nM\xf7')
#   8.563    COM20 -> 01 11 20 07 00 b5 73 d2 6e 4d f7 
send_command("0111200700"+peer_addr)

#   8.719    [HCI - COM20] HCI event received: OPCODE_LE_ADD_DEV_WHITE_LIST (0xE)
#                              Command_Opcode = 0x2011 (8209)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.719    COM20 <- 04 0e 04 01 11 20 00 

#   8.875    [HCI - COM20] LeCreateConn(LE_Scan_Interval=16,
#                     LE_Scan_Window=16,
#                     Initiator_Filter_Policy=0,
#                     Peer_Address_Type=2,
#                     Peer_Address='\xb5s\xd2nM\xf7',
#                     Own_Address_Type=2,
#                     Conn_Interval_Min=48,
#                     Conn_Interval_Max=48,
#                     Conn_Latency=0,
#                     Supervision_Timeout=100,
#                     Minimum_CE_Length=0,
#                     Maximum_CE_Length=0)
#   8.875    COM20 -> 01 0d 20 19 10 00 10 00 00 02 b5 73 d2 6e 4d f7 02 30 00 30 00 00 00 64 00 00 00 00 00 
send_command("010d2019100010000002"+peer_addr+"02300030000000640000000000")

if(wait_event(event = "043E13010000000002"+peer_addr+"30000000640000", retry_count = 4) == 0):
    print("Failed to connect")
    exit(1)

print("Test passed")
exit(0)