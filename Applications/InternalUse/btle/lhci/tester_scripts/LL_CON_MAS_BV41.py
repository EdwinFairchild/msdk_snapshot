#! /usr/bin/env python

from __future__ import print_function
import serial, array, codecs
import sys
from time import sleep
import signal
import datetime

filename="../common.py"
exec(open(filename).read())
# Send the commands

# IUT addr
peer_addr="005634128B0500"

# Send the commands
send_command("01030C00")
send_command("01031000")
send_command("01010C089088010000800020")
send_command("010120081F00000000000000")
send_command("01091000")
send_command("01102000")

# Set default PHY
# 03 length
# 1 byte ALL_PHYS
# 1 byte TX_PHYS
# 1 byte RX_PHYS
send_command("01312003"+"00"+"07"+"07")

send_command("01112007"+peer_addr)
send_command("0106200F200020000000"+peer_addr+"0103")
send_command("010920200354554900000000000000000000000000000000000000000000000000000000")
send_command("010A200101")

# Read events from the tester
while(1):
    rcv_event()
