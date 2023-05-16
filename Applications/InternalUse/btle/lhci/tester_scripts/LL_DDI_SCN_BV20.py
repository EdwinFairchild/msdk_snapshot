#! /usr/bin/env python

filename="../common.py"
exec(open(filename).read())

# Reset()
send_command("01030c00")

# LeWriteDefaultDataLength(TxOctets=251,
send_command("01242004fb009042")

# LeSetDefaultPhy(ALL_PHYS=0,
send_command("01312003000101")

# LeSetExtAdvParam(Advertising_Handle=1,
send_command("01362019011300500000500000070000000000000000007f0100010f00")

# LeSetExtScanRespData(Advertising_Handle=1,
send_command("013820230103011f1ac75d01c433499c9149ebeea1671aeff284607efd5e8fac1186d97d3a12c0")

# LeSetExtAdvEnable(Enable=True,
send_command("01392006010101000000")

while(1):
    read_events()