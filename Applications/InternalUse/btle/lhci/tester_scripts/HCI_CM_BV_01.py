
dev_addr = "B573D26E4DF7"
peer_addr = "5634128B0500"

filename="../common.py"
exec(open(filename).read())

#   4.950    [HCI - COM3] Reset()
#   4.951    COM3 -> 01 03 0c 00 
send_command("01030c00")

#   5.793    [HCI - COM3] VsSetBdAddr(addr='\xb5s\xd2nM\xf7')
#   5.793    COM3 -> 01 f0 ff 06 b5 73 d2 6e 4d f7 
# send_command("01f0ff06b573d26e4df7")
# TODO: Set the propper address

#   7.453    COM3 <- 04 0e 04 01 01 20 00 

#   8.359    [HCI - COM3] LeAddDevResList(Peer_Identity_Address_Type=0,
#                       Peer_Identity_Address='V4\x12\x8b\x05\x00',
#                       Peer_IRK='\xcf\xb4\x02\xea5\xadUWV\x90\x16T[M9s',
#                       Local_IRK='Hee-Haw!BR-549JS')
#   8.359    COM3 -> 01 27 20 27 00 56 34 12 8b 05 00 cf b4 02 ea 35 ad 55 57 56 90 16 54 5b 4d 39 73 48 65 65 2d 48 61 77 21 42 52 2d 35 34 39 4a 53 
send_command("0127202700"+peer_addr+"cfb402ea35ad5557569016545b4d39734865652d4861772142522d3534394a53")

#   8.359    [HCI - COM3] HCI event received: OPCODE_LE_ADD_DEV_RES_LIST (0xE)
#                              Command_Opcode = 0x2027 (8231)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.359    COM3 <- 04 0e 04 01 27 20 00 

#   8.422    [HCI - COM3] LeSetAddrResEnable(Address_Resolution_Enable=True)
#   8.422    COM3 -> 01 2d 20 01 01 
send_command("012d200101")

#   8.422    [HCI - COM3] HCI event received: OPCODE_LE_SET_ADDR_RES_ENABLE (0xE)
#                              Command_Opcode = 0x202D (8237)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.422    COM3 <- 04 0e 04 01 2d 20 00 

#   8.734    [HCI - COM3] LeSetAdvParam(Advertising_Interval_Min=32,
#                     Advertising_Interval_Max=32,
#                     Advertising_Type=4,
#                     Own_Address_Type=2,
#                     Peer_Address_Type=0,
#                     Peer_Address='V4\x12\x8b\x05\x00',
#                     Advertising_Channel_Map=7,
#                     Advertising_Filter_Policy=0)
#   8.734    COM3 -> 01 06 20 0f 20 00 20 00 04 02 00 56 34 12 8b 05 00 07 00 
send_command("0106200f20002000040200"+peer_addr+"0700")

#   8.734    [HCI - COM3] HCI event received: OPCODE_LE_SET_ADV_PARAM (0xE)
#                              Command_Opcode = 0x2006 (8198)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.734    COM3 <- 04 0e 04 01 06 20 00 

#   8.797    [HCI - COM3] LeSetAdvData(Advertising_Data='')
#   8.797    COM3 -> 01 08 20 20 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
send_command("010820200000000000000000000000000000000000000000000000000000000000000000")

#   8.797    [HCI - COM3] HCI event received: OPCODE_LE_SET_ADV_DATA (0xE)
#                              Command_Opcode = 0x2008 (8200)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.797    COM3 <- 04 0e 04 01 08 20 00 

#   8.813    [HCI - COM3] LeSetAdvEnable(Advertising_Enable=True)
#   8.813    COM3 -> 01 0a 20 01 01 
send_command("010a200101")

#   8.813    [HCI - COM3] HCI event received: OPCODE_LE_SET_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x200A (8202)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   8.813    COM3 <- 04 0e 04 01 0a 20 00 

if(wait_event(event = "043E13010000000102"+peer_addr+"30000000640001", retry_count = 4) == 0):
    print("Failed to connect")
    exit(1)

print("Test passed")
exit(0)
