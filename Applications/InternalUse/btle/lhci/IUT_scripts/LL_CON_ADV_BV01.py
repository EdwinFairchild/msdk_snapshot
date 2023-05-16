#! /usr/bin/env python

filename="../common.py"
exec(open(filename).read())

# Tests that an advertiser IUT receives a connection request, stops advertising after the reception and
# starts to maintain a connection in the slave role.
# The Lower Tester acts first in the initiating state, sending the connection request to the IUT, then
# starts to maintain a connection in the master role, observing the packets and timing from the IUT.

# Tester addr
# peer_addr="006634128B0500"
peer_addr="00b573d26e4df7" # Dekra tester

# Send the commands
send_command("01030C00")
send_command("01031000")
send_command("01010C089088010000800020")
send_command("010120081F00000000000000")
send_command("01091000")
send_command("01102000")
send_command("0111200700"+peer_addr)
send_command("0106200F200020000000"+peer_addr+"0703")
send_command("010920200354554900000000000000000000000000000000000000000000000000000000")
send_command("010A200101")

# Read events from the device
while(1):
    rcv_event()
