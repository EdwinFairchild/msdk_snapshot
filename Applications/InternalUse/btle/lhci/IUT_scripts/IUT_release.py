#! /usr/bin/env python

filename="../common.py"
exec(open(filename).read())

# Commands copied from the Dekra test logs

# Test the IUT to make sure nothing serious is broken

# Send the commands
send_command("01030c00")
send_command("01091000")
send_command("01010c089088010000800020")
send_command("01630c080000800000000000")
send_command("01011000")
send_command("01032000")
send_command("01022000")
send_command("01010c089088010000800020")
send_command("01012008ffff0f0000000000")
send_command("0127202700b573d26e4df74865652d4861772142522d3534394a53cfb402ea35ad5557569016545b4d3973")
send_command("012e20020f00")
send_command("012d200101")
send_command("01102000")

# Set default PHY
# 03 length
# 1 byte ALL_PHYS
# 1 byte TX_PHYS
# 1 byte RX_PHYS
send_command("01312003"+"00"+"03"+"03")

send_command("0111200700b573d26e4df7")
send_command("010d2019100010000002b573d26e4df702300030000000640000000000", exp_rsp = "040F0400010D20")

# rcv_event(exp_rsp = "043E1F0A0000000002B573D26E4DF75343C945746F24636CC4CB7030000000640000")
# Resolvable private address will be different every time.
rcv_event()
rcv_event(exp_rsp = "043E0414000000")
rcv_event(exp_rsp = "043E0C04000000FFF9010800000000")
rcv_event(exp_rsp = "043E0B070000FB001B00FB009042")


# Test Master Initiated PHY update procedure

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

send_command("01322007"+"0000"+"00"+"03"+"03"+"0000", exp_rsp = "040F0400013220")

read_events(seconds = 4)


# Test the Slave initiated PHY update procedure
rcv_event(exp_rsp = "043E060C0000000202")
rcv_event(exp_rsp = "043E060C0000000101")

print("Test Complete")

# Read events from the device
while(1):
    rcv_event()