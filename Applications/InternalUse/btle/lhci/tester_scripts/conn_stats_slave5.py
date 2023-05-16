#! /usr/bin/env python

dev_addr = "7A34128B0500"

filename="../common.py"
exec(open(filename).read())

# IUT addr
peer_addr="008734128B0500"

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

slave_connect(peer_addr, dev_addr)

# Wait for signal from IUT that we're finished collecting data
while(1):
  evt = rcv_event()
  if("AACC" in evt):
    break

conn_stats()

while(1):
  rcv_event()
