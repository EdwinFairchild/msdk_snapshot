#! /usr/bin/env python

from __future__ import print_function
import serial, array, codecs
import sys
from time import sleep
import signal
import datetime

# Save the current time for timestamps
ts = datetime.datetime.now()

def wait_event(event, retry_count = 5):
    event_string = ""
    while(retry_count):
        event_string = rcv_event()
        if(event_string == event):
            break;
        retry_count = retry_count-1

    if(retry_count == 0):
        return 0

    return 1

def read_events(seconds = 2, print_evt = True):
    # Read events from the device for a few seconds
    start_time = datetime.datetime.now()
    delta = datetime.datetime.now() - start_time
    while(delta.seconds < seconds):
        rcv_event(print_evt = print_evt)
        delta = datetime.datetime.now() - start_time
        if((delta.seconds > 30) and ((delta.seconds % 30) == 0)) :
            print(str(datetime.datetime.now()-ts) + "| ")


def close():
     # Send a reset 
    send_command("01030c00")
    port.flush()
    port.close()
    sys.exit(0)

def signal_handler(signal, frame):
    close()

def rcv_event(exp_rsp = "", print_evt=True):

    # Receive the event
    evt=port.read(size=1)
    if(len(evt) == 0):
        port.flush()
        return ""

    evt=int(codecs.encode(evt, 'hex_codec'),16)
    status_string = '%02X'%evt

    # ACL data
    if(evt == 2):
        # Receive the rest of the header
        hdr = port.read(size=4)
        packet_len = hdr[2] + (hdr[3] << 8)
        hdr=int(codecs.encode(hdr, 'hex_codec'),16)
        status_string+= '%08X'%hdr

    # HCI Event
    elif(evt == 4):
        # Receive the rest of the header
        hdr = port.read(size=2)
        packet_len = hdr[1]
        hdr=int(codecs.encode(hdr, 'hex_codec'),16)
        status_string+= '%04X'%hdr
    else:
        print("unknown evt = "+str(evt))
        return

    payload = port.read(size=packet_len)

    if(print_evt):
      # Print the timestamp
      tf = datetime.datetime.now()
      print(tf-ts, end="| ")

    # Print the remainder of the packet
    for i in range(0,packet_len):
        status_string += '%02X'%payload[i]

    if(print_evt):
      print(status_string)

    if(exp_rsp != ""):
        if(exp_rsp != status_string):
            print("Error: exp_rsp "+exp_rsp+" != "+status_string)
            exit(1)

    return status_string

def send_command(packet, resp=True, delay = 0.01, exp_rsp = "", print_cmd = True):

    # Send the command and data
    tf = datetime.datetime.now()
    if(print_cmd):
      print(tf-ts, end="| ")
      print(packet)

    port.write(bytearray.fromhex(packet))
    sleep(delay)

    if(resp):
        return rcv_event(exp_rsp)


def slave_connect(addr, dev_addr = "0000"):

  # Use different advertising channels based on the device address
  if(dev_addr != "0000"):
    adv_ch = int(dev_addr[1], 16) % 3
    adv_ch = str(0x1 << adv_ch)
  else:
    adv_ch = "7"

  adv_ch = "7"

  send_command("01112007"+addr)
  send_command("0106200F200020000000"+addr+"0"+adv_ch+"03")
  send_command("010920200354554900000000000000000000000000000000000000000000000000000000")

  # Start advertising
  send_command("010A200101")

  # Wait until we get a connection to the peer
  while(1):
    # Wait for address in event to signal connection
    evt = rcv_event()
    if(peer_addr.upper() in evt):
      connected = True
      break

  # Send ACL data to signal that we're connected
  send_command("0200000200AABB")
  # rcv_event()

def conn_stats():
  # Get the connection status
  evt = send_command("01FDFF00")

  # Parse the status

  # Strip the first 7 bytes with the opcode, status, length...
  # 040EF901FDFF00...
  evt = evt[14:]

  # print the header row
  print("ch, success, crc, timeout, per")

  # Constants for indexing into the results
  success_offset = 0
  crc_offset = 4
  timeout_offset = 8
  param_len = 4

  # Extract the stats for each ch
  chmap = 0x1FFFFFFFFF
  for ch in range(37):

    # 6 bytes of data per channel
    ch_offset = ch*12

    # Extract the parameters
    success = evt[ch_offset+success_offset:ch_offset+success_offset+param_len]
    crc = evt[ch_offset+crc_offset:ch_offset+crc_offset+param_len]
    timeout = evt[ch_offset+timeout_offset:ch_offset+timeout_offset+param_len]

    # Convert to int
    success = int(success,16)
    crc = int(crc,16)
    timeout = int(timeout,16)

    # Swap the byte ordering
    success = int((success & 0xFF00)>>8)+int((success & 0xFF)<<8)
    crc = int((crc & 0xFF00)>>8)+int((crc & 0xFF)<<8)
    timeout = int((timeout & 0xFF00)>>8)+int((timeout & 0xFF)<<8)

    # Print the results
    print(str(ch)+", ", end="")
    print(str(success)+", ", end="")
    print(str(crc)+", ", end="")
    print(str(timeout)+", ", end="")

    # Calculate the PER
    if((success + crc + timeout) == 0):
      per = 100.0
    else:
      per = (1 - (success / (success + crc + timeout))) * 100
    print(str(per))

    # If PER is above 1, remove from the chmap
    if(per > 1):
      chmap = chmap & ~(0x1 << ch)

  print("chmap = 0x"+'%010X'%chmap)
  return chmap



##### main #####

# Parse the arguments
if(len(sys.argv) == 1):
    print("Enter serial port: \"COM20\" or \"/dev/ttyUSB0\"")
    COM_PORT = input()
else:
    COM_PORT = sys.argv[1]

print("Using "+COM_PORT+" for HCI serial")

# Open serial port
port = serial.Serial(
    port=str(COM_PORT),
    baudrate=2000000,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    rtscts=False,
    dsrdtr=False,
    timeout=1.0
)
port.isOpen()

# Setup the Signal handler to catch the ctrl-C
signal.signal(signal.SIGINT, signal_handler)

# Send vendor specific command to set address if dev_addr is defined
try: dev_addr
except NameError:
    print("Using default device address")
else:
    print("Setting address "+dev_addr)
    send_command("01F0FF06"+dev_addr)
