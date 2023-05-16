#! /usr/bin/env python

dev_addr = "6634128B0500"

filename="../common.py"
exec(open(filename).read())

print("Tester CI basic connection test")

peer_addr = "005634128B0500"

#   5.961    [HCI - COM20] SetEventMask(Event_Mask=2305983746702149776L)
#   5.961    COM20 -> 01 01 0c 08 90 88 01 00 00 80 00 20 
send_command("01010c08FFFFFFFFFFFFFFFF")

#   6.135    [HCI - COM20] HCI event received: OPCODE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0xC01 (3073)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.135    COM20 <- 04 0e 04 01 01 0c 00 

#   6.145    [HCI - COM20] SetEventMaskPage2(Event_Mask_Page_2=8388608)
#   6.145    COM20 -> 01 63 0c 08 00 00 80 00 00 00 00 00 
send_command("01630c08FFFFFFFFFFFFFFFF")

#   6.321    [HCI - COM20] HCI event received: OPCODE_SET_EVENT_MASK_PAGE2 (0xE)
#                              Command_Opcode = 0xC63 (3171)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.321    COM20 <- 04 0e 04 01 63 0c 00 

#   6.962    [HCI - COM20] SetEventMask(Event_Mask=2305983746702149776L)
#   6.962    COM20 -> 01 01 0c 08 90 88 01 00 00 80 00 20 
send_command("01010c08FFFFFFFFFFFFFFFF")

#   7.134    [HCI - COM20] HCI event received: OPCODE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0xC01 (3073)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.134    COM20 <- 04 0e 04 01 01 0c 00 

#   7.274    [HCI - COM20] LeSetEventMask(LE_Event_Mask=526337)
#   7.274    COM20 -> 01 01 20 08 01 08 08 00 00 00 00 00 
send_command("01012008FFFFFFFFFFFFFFFF")


retry_count = 3
while(retry_count):

    # Setup Scanning
    send_command("01030C00")
    send_command("01031000")
    send_command("01010C089088010000800020")
    send_command("010120081F00000000000000")
    send_command("01091000")
    send_command("01102000")

    # Set default PHY
    send_command("01312003"+"00"+"03"+"03")

    # Setup the white list
    send_command("01112007"+peer_addr)
    send_command("010B200701A000A0000001")
    send_command("010C20020100")

    # Receive some scan reports
    i = 0
    while(i < 10):
        # Make sure we receive both the advertising report and the scan response report
        rcv_event()
        i += 1

    # Create the connection
    send_command("010C20020000")
    send_command("01022000")
    send_command("010D2019A000A00000"+peer_addr+"000A000A0000005A0007000700")

    # Wait for the connection
    if(wait_event(event = "043E130100000000005634128B05000A0000005A0007", retry_count = 6) == 0):
        if(retry_count == 0):
            print("Failed to establish connection")
            exit(1)
    else:
        break

    retry_count = retry_count-1


# Change the PHY to the 2M
send_command("01322007"+"0000"+"00"+"02"+"02"+"0000", exp_rsp = "040F0400013220")
# if(wait_event(event = "043E060C0000000202", retry_count = 5) == 0):
#     print("Failed to change to 2M PHY")
#     exit(1)

# make sure we don't disconnect
if(rcv_event() != ""):
    print("Disconnection")
    exit(1)

if(rcv_event() != ""):
    print("Disconnection")
    exit(1)

if(rcv_event() != ""):
    print("Disconnection")
    exit(1)

if(rcv_event() != ""):
    print("Disconnection")
    exit(1)

# Change back to 1M
send_command("01322007"+"0000"+"00"+"01"+"01"+"0000", exp_rsp = "040F0400013220")
# if(wait_event(event = "043E060C0000000101", retry_count = 10) == 0):
#     print("Failed to change back to 1M PHY")
#     exit(1)

# Make sure we don't get any disconnect events
if(rcv_event() != ""):
    print("Disconnection!")
    exit(1)

if(rcv_event() != ""):
    print("Disconnection")
    exit(1)

if(rcv_event() != ""):
    print("Disconnection!")
    exit(1)

if(rcv_event() != ""):
    print("Disconnection")
    exit(1)

print("Test passed!")
exit(0)
