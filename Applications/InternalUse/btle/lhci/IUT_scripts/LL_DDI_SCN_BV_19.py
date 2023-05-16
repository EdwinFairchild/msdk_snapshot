#! /usr/bin/env python

filename="../common.py"
exec(open(filename).read())

#   4.928    [HCI - COM20] Reset()
send_command("01030c00")

#   4.954    [HCI - COM20] Reset()
send_command("01030c00")

#   5.588    [HCI - COM20] ReadBdAddr()
send_command("01091000")

#   5.665    [HCI - COM20] SetEventMask(Event_Mask=2305983746702149776L)
send_command("01010c089088010000800020")

#   5.686    [HCI - COM20] SetEventMaskPage2(Event_Mask_Page_2=8388608)
send_command("01630c080000800000000000")

#   5.851    [HCI - COM20] ReadLocalVerInfo()
send_command("01011000")

#   5.997    [HCI - COM20] LeReadLocalSupFeat()
send_command("01032000")

#   6.079    [HCI - COM20] LeReadBufSize()
send_command("01022000")

#   6.104    [HCI - COM20] SetEventMask(Event_Mask=2305983746702149776L)
send_command("01010c089088010000800020")

#   6.252    [HCI - COM20] LeSetEventMask(LE_Event_Mask=69632)
send_command("010120080010010000000000")

#  71.993    [HCI - COM20] LeSetExtScanParam(Own_Address_Type=0,
send_command("014120080000010010001000")

#  72.021    [HCI - COM20] LeSetExtScanEnable(Enable=True,
send_command("01422006010000000000")

# Need to make sure we're not sending reports for directed advertising not directed at us
# 0:00:01.677392| 043E1A0D011500006634128B050001010000F80000000D000300000000

# Read events from the device
while(1):
    rcv_event()