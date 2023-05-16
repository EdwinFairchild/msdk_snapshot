#! /usr/bin/env python

filename="../common.py"
exec(open(filename).read())

peer_addr="006634128B0500" # ME14 tester

# [HCI - COM20] Reset()
send_command("01030c00")

# Adding to white list
send_command("01112007"+peer_addr)

# [HCI - COM20] LeSetExtScanParam(Own_Address_Type=0,
# Adding scanning with white list
send_command("014120080001010110001000")

# [HCI - COM20] LeSetExtScanEnable(Enable=True,
send_command("01422006010000000000")

# Read events from the device
while(1):
    rcv_event()


# Sample output
# 0:00:01.875873| 043E1A0D010000006634128B050000000000FE00000000000000000000
# 0:00:01.876011| 043E390D010400006634128B050000000000FE0000000000000000001F00A40020B5010000F1010000F3010000F5010000F7010000F9010000000000