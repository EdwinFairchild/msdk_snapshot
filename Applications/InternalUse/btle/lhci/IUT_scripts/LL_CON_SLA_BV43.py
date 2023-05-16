#! /usr/bin/env python

filename="../common.py"
exec(open(filename).read())

# Tester addr
peer_addr="006634128B0500"
# peer_addr="005634128B0500"
 #peer_addr="01B410A0EA2FF6" # Nordic


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
send_command("01312003"+"00"+"03"+"03")

send_command("01112007"+peer_addr)
send_command("0106200F200020000000"+peer_addr+"0703")
send_command("010920200354554900000000000000000000000000000000000000000000000000000000")
send_command("010A200101")

read_events(seconds = 5)

send_command("011620020000")

# 07 length
# 2 bytes handle
# 1 byte ALL_PHYS
# 1 byte TX_PHYS
# 1 byte RX_PHYS
# 2 bytes PHY_OPTIONS
# send_command("01322007"+"0000"+"00"+"02"+"02"+"0000")

# Read events from the device
while(1):
    rcv_event()