#! /usr/bin/env python

filename="../common.py"
exec(open(filename).read())

# Test that a master IUT is able to perform the PHY update procedure. Test that the IUT can use all
# supported PHYs, including asymmetric settings. Test that the IUT successfully operates using the
# selected PHY(s).
# The Lower Tester acts in the slave role maintaining a connection, the Upper Tester issues the HCI
# command to start the PHY update procedure as the Host of the IUT, and the Lower Tester then
# observes the PHY update procedure carried out by the IUT and accepts the IUT’s request.

# peer_addr="006634128B0500" # ME14 tester
# peer_addr="005634128B0500" # ME14 tester
peer_addr="00b573d26e4df7" # Dekra tester
# peer_addr="01B410A0EA2FF6" # Nordic

# Setup Scanning
send_command("01030C00")
send_command("01031000")
send_command("01010C089088010000800020")
send_command("010120081F00000000000000")
send_command("01091000")
send_command("01102000")
send_command("01112007"+peer_addr) # Tester
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
send_command("010D2019A000A00000"+peer_addr+"000A000A0000005A000F100F10") #IUT

# Read some events for 2 seconds
read_events()

# 07 length
# 2 bytes handle
# 1 byte ALL_PHYS
# 1 byte TX_PHYS
# 1 byte RX_PHYS
# 2 bytes PHY_OPTIONS
send_command("01322007"+"0000"+"00"+"01"+"01"+"0000")

# Read some events for 3 seconds
read_events(seconds = 3)

# # 07 length
# # 2 bytes handle
# # 1 byte ALL_PHYS
# # 1 byte TX_PHYS
# # 1 byte RX_PHYS
# # 2 bytes PHY_OPTIONS
send_command("01322007"+"0000"+"00"+"02"+"02"+"0000")

# # Read some events for 3 seconds
read_events(seconds = 3)

# # 07 length
# # 2 bytes handle
# # 1 byte ALL_PHYS
# # 1 byte TX_PHYS
# # 1 byte RX_PHYS
# # 2 bytes PHY_OPTIONS
# send_command("01322007"+"0000"+"00"+"01"+"01"+"0000")

# # Read some events for 3 seconds
# read_events(seconds = 3)

# 07 length
# 2 bytes handle
# 1 byte ALL_PHYS
# 1 byte TX_PHYS
# 1 byte RX_PHYS
# 2 bytes PHY_OPTIONS
send_command("01322007"+"0000"+"00"+"04"+"04"+"0000")

# Read events from the IUT
while(1):
    read_events()