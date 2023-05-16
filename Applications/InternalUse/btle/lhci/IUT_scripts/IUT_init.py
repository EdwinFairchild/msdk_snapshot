#! /usr/bin/env python

filename="../common.py"
exec(open(filename).read())

# Act as initiator and create a connection with the peer
peer_addr="006634128B0500"
peer_addr="00b573d26e4df7" # Dekra tester
# peer_addr="00F250A00E6C54" # TI Board
# peer_addr="01B410A0EA2FF6" # Nordic

# Setup Scanning
send_command("01030C00")
send_command("01031000")
send_command("01010C089088010000800020")
send_command("010120081F00000000000000")
send_command("01091000")
send_command("01102000")
send_command("01112007"+peer_addr)
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
send_command("010D2019A000A00000"+peer_addr+"000A000A0000005A0007000700") #IUT

# Read some events for 3 seconds
read_events(seconds = 3)

# Send the Read P256 Public Key command, takes about 120 ms to complete
send_command("01252000")

# Read some events for 3 seconds
read_events(seconds = 3)

# Send the Generate DH Key command
send_command("012620401ea1f0f01faf1d9609592284f19e4c0047b58afd8615a69f559077b22faaa1904c55f33e429dad377356703a9ab85160472d1130e28e36765f89aff915b1214a")

while(1):
    rcv_event()
