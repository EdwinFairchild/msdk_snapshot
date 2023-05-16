#! /usr/bin/env python

filename="../common.py"
exec(open(filename).read())

# [HCI] Reset()
send_command("01030c00")

# [HCI] HCI event received: OPCODE_RESET (0xE)
#                   Command_Opcode = 0xC03 (3075)
#                   Event_Code = 0xE (14)
#                   Num_HCI_Command_Packets = 0x1 (1)
#                   Parameter_Length = 0x4 (4)
#                   Status = 0x0 (0)
# COM <- 04 0e 04 01 03 0c 00 

while(1):
  rcv_event()

