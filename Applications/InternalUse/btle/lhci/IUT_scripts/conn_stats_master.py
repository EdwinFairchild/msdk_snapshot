#! /usr/bin/env python

dev_addr = "8734128B0500"

filename="../common.py"
exec(open(filename).read())

def connect(addr, wait_cnt = 5, retries = 5):
  #   7.603    [HCI - COM20] LeCreateConn(LE_Scan_Interval=16,
  #                     LE_Scan_Window=16,
  #                     Initiator_Filter_Policy=0,
  #                     Peer_Address_Type=0,
  #                     Peer_Address='\xb5s\xd2nM\xf7',
  #                     Own_Address_Type=0,
  #                     Conn_Interval_Min=0x6,
  #                     Conn_Interval_Max=0x6,
  #                     Conn_Latency=0,
  #                     Supervision_Timeout=1000,
  #                     Minimum_CE_Length=0,
  #                     Maximum_CE_Length=0)

  # LSB first
  scan_int="1000"
  scan_window="1000"
  int_min = "2000" # 640 ms
  int_max = "2000"
  min_ce_len = "0000" # 620 us
  max_ce_len = "0100" # 1250 us

  while(retries > 0):
    retries = retries-1
    connected = False

    # Start initiating
    send_command("010d2019"+scan_int+scan_window+"00"+addr+"00"+int_min+int_max+"0000e803"+min_ce_len+max_ce_len)

    # Wait until we get a connection to the peer
    start_time = datetime.datetime.now()
    while(1):
      # Wait for address in event to signal connection
      evt = rcv_event()
      if(addr.upper() in evt):
        connected = True
        break

      # Timeout
      delta = datetime.datetime.now() - start_time
      if(delta.seconds > wait_cnt):
        retries = 0
        break

    if(connected):
      # Wait wait_cnt seconds for failure or ACL data from peer
      start_time = datetime.datetime.now()
      delta = datetime.datetime.now() - start_time
      while(delta.seconds < wait_cnt):
        evt = rcv_event()

        # Connection failed
        if("003E" in evt):
          connected = False
          break

        # Connection Closed
        if("08" in evt):
          connected = False
          break

        # ACL data in event
        if("AABB" in evt):
          connected = True
          break

        delta = datetime.datetime.now() - start_time

      if(connected):
        # sleep(1)
        return True

  # Connected failed after retries
  # Create connection cancel
  send_command("010E2000")
  rcv_event()
  return False


peer_addr0="007534128B0500" # ME14 tester0
peer_addr1="007634128B0500" # ME14 tester
peer_addr2="007734128B0500" # ME14 tester
peer_addr3="007834128B0500" # ME14 tester
peer_addr4="007934128B0500" # ME14 tester
peer_addr5="007a34128B0500" # ME14 tester
peer_addr6="007b34128B0500" # ME14 tester
peer_addr7="007c34128B0500" # ME14 tester

# usb-FTDI_FT230X_Basic_UART_DN05Y5OA-if00-port0 0
# usb-FTDI_FT230X_Basic_UART_DN05Y5OC-if00-port0 1
# usb-FTDI_FT230X_Basic_UART_DN05Y5OJ-if00-port0 2
# usb-FTDI_FT230X_Basic_UART_DN05Y5OL-if00-port0 3, busted USB
# usb-FTDI_FT230X_Basic_UART_DN05Y5P0-if00-port0 4
# usb-FTDI_FT230X_Basic_UART_DN05Y5OD-if00-port0 5
# usb-FTDI_FT230X_Basic_UART_DN05Y5PC-if00-port0 6
# usb-FTDI_FT230X_Basic_UART_DN05Y5PD-if00-port0 7

# TODO: Properly set the connection event timing and max data len to 48 bytes

#   4.982    [HCI - COM20] Reset()
#   4.982    COM20 -> 01 03 0c 00 
send_command("01030c00")


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

#   7.462    [HCI - COM20] HCI event received: OPCODE_LE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0x2001 (8193)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.462    COM20 <- 04 0e 04 01 01 20 00 

#   7.603    [HCI - COM20] LeCreateConn(LE_Scan_Interval=16,
#                     LE_Scan_Window=16,
#                     Initiator_Filter_Policy=0,
#                     Peer_Address_Type=0,
#                     Peer_Address='\xb5s\xd2nM\xf7',
#                     Own_Address_Type=0,
#                     Conn_Interval_Min=0x6,
#                     Conn_Interval_Max=0x6,
#                     Conn_Latency=0,
#                     Supervision_Timeout=1000,
#                     Minimum_CE_Length=0,
#                     Maximum_CE_Length=0)

# Update the chmap to exclude ch 13 and 29
# 0x1FDFFFDFFF
send_command("01142005"+"FFDFFFDF1F")

if(not connect(peer_addr0)):
  print("Failed to connect to peer0")
  close()
else:
  print("Connected to peer0")
read_events(seconds = 2)

if(not connect(peer_addr1)):
  print("Failed to connect to peer1")
  close()
else:
  print("Connected to peer1")
read_events(seconds = 2)

if(not connect(peer_addr2)):
  print("Failed to connect to peer2")
  close()
else:
  print("Connected to peer2")
read_events(seconds = 2)

if(not connect(peer_addr3)):
  print("Failed to connect to peer3")
  close()
else:
  print("Connected to peer3")
read_events(seconds = 2)

if(not connect(peer_addr4)):
  print("Failed to connect to peer4")
  close()
else:
  print("Connected to peer4")
read_events(seconds = 2)

if(not connect(peer_addr5)):
  print("Failed to connect to peer5")
  close()
else:
  print("Connected to peer5")
read_events(seconds = 2)

if(not connect(peer_addr6)):
  print("Failed to connect to peer6")
  close()
else:
  print("Connected to peer6")
read_events(seconds = 2)

if(not connect(peer_addr7)):
  print("Failed to connect to peer7")
  close()
else:
  print("Connected to peer7")
read_events(seconds = 2)

# Sleep while collecting data
read_events(seconds = 300)

# # Send a notification to the peer to collect results
send_command("0200000200AACC")
rcv_event()

send_command("0201000200AACC")
rcv_event()

send_command("0202000200AACC")
rcv_event()

send_command("0203000200AACC")
rcv_event()

send_command("0204000200AACC")
rcv_event()

send_command("0205000200AACC")
rcv_event()

send_command("0206000200AACC")
rcv_event()

send_command("0207000200AACC")
rcv_event()

# Collect the channel results
chmap = conn_stats()

while(1):
  rcv_event()

# Swap the byte ordering
chmap_swapped = 0
chmap_swapped = chmap_swapped | ((chmap & 0xFF00000000) >> 32)
chmap_swapped = chmap_swapped | ((chmap & 0x00FF000000) >> 16)
chmap_swapped = chmap_swapped | ((chmap & 0x0000FF0000) >> 0)
chmap_swapped = chmap_swapped | ((chmap & 0x000000FF00) << 16)
chmap_swapped = chmap_swapped | ((chmap & 0x00000000FF) << 32)

# Update the chmap
send_command("01142005"'%010X'%chmap_swapped)

# Sleep while collecting data
read_events(seconds = 10)

# # Send a notification to the peer to collect results
# send_command("0200000200AACC")
# rcv_event()

# send_command("0204000200AACC")
# rcv_event()

# send_command("0202000200AACC")
# rcv_event()

# send_command("0203000200AACC")
# rcv_event()

# Collect the channel results
chmap = conn_stats()

read_events(seconds = 1)
close()

while(1):
  rcv_event()
