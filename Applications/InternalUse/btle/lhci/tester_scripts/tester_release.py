#! /usr/bin/env python

filename="../common.py"
exec(open(filename).read())

# Commands copied from the Dekra test logs
# Make sure to adjust the tester BD ADDR arccordingly

# Send the commands
send_command("01030c00")
send_command("01030c00")
send_command("01091000")
send_command("01010c089088010000800020")
send_command("01630c080000800000000000")
send_command("01012008fffd0f0000000000")
send_command("01011000")
send_command("01032000")
send_command("01242004fb009042")
send_command("01312003000101")
send_command("01012008ffff0f0000000000")
send_command("01272027005634128b0500cfb402ea35ad5557569016545b4d39734865652d4861772142522d3534394a53")
send_command("012d200101")
send_command("0106200f200020000402005634128b05000700")
send_command("010820200000000000000000000000000000000000000000000000000000000000000000")

# Set default PHY
# 03 length
# 1 byte ALL_PHYS
# 1 byte TX_PHYS
# 1 byte RX_PHYS
send_command("01312003"+"00"+"03"+"03")

send_command("010a200101", exp_rsp = "040E04010A2000")

# rcv_event(exp_rsp = "043E1F0A00000001025634128B050042426334325BCE62F055D67E30000000640001")
# Resolvable private address will be different every time.
rcv_event()
rcv_event(exp_rsp = "043E0414000000")
rcv_event(exp_rsp = "043E0C04000000FFF9010800000000")
rcv_event(exp_rsp = "043E0B070000FB009042FB001B00")

# Test the Master initiated PHY update procedure
rcv_event(exp_rsp = "043E060C0000000202")
rcv_event(exp_rsp = "043E060C0000000101")

read_events(seconds = 5)

# Change the PHY to the 2M
send_command("01322007"+"0000"+"00"+"02"+"02"+"0000", exp_rsp = "040F0400013220")
rcv_event(exp_rsp = "043E060C0000000202")

# Change back to 1M
send_command("01322007"+"0000"+"00"+"01"+"01"+"0000", exp_rsp = "040F0400013220")
rcv_event(exp_rsp = "043E060C0000000101")

# Test No change
send_command("01322007"+"0000"+"00"+"01"+"01"+"0000", exp_rsp = "040F0400013220")
rcv_event(exp_rsp = "043E060C0000000101")

# Test asymmetric
send_command("01322007"+"0000"+"00"+"02"+"01"+"0000", exp_rsp = "040F0411013220")

print("Test Complete")

# Read events from the device
while(1):
    rcv_event()
