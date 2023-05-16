#! /usr/bin/env python

from __future__ import print_function
import serial, array, codecs
import sys
from time import sleep
import signal
import datetime

filename="../common.py"
exec(open(filename).read())

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
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    rtscts=False
)
port.isOpen()

# Setup the Signal handler to catch the ctrl-C
signal.signal(signal.SIGINT, signal_handler)

# Send the commands

# Attempts to initiate a connection with the selected peer

peer_addr = "5634128B0500"

# Setup Scanning
send_command("01030C00")
send_command("01031000")
send_command("01010C089088010000800020")
send_command("010120081F00000000000000")
send_command("01091000")
send_command("01102000")
send_command("0111200700"+peer_addr)
send_command("010B200701A000A0000001")
send_command("010C20020100")

# Receive some scan reports
i = 0
while(i < 10):
    rcv_event()
    i += 1

# Create the connection
send_command("010C20020000")
send_command("01022000")
send_command("010D2019A000A0000000"+peer_addr+"000A000A0000005A0007000700") #IUT

# Read events from the device
while(1):
    rcv_event()
