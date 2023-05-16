#! /usr/bin/env python

filename="../common.py"
exec(open(filename).read())

#   4.915    [HCI - COM3] Reset()
#   4.915    COM3 -> 01 03 0c 00 
send_command("01030c00")

#   4.966    [HCI - COM3] Reset()
#   4.966    COM3 -> 01 03 0c 00 
send_command("01030c00")

#   5.608    [HCI - COM3] ReadBdAddr()
#   5.608    COM3 -> 01 09 10 00 
send_command("01091000")

#   5.720    [HCI - COM3] SetEventMask(Event_Mask=2305983746702149776L)
#   5.720    COM3 -> 01 01 0c 08 90 88 01 00 00 80 00 20 
send_command("01010c089088010000800020")

#   5.727    [HCI - COM3] SetEventMaskPage2(Event_Mask_Page_2=8388608)
#   5.727    COM3 -> 01 63 0c 08 00 00 80 00 00 00 00 00 
send_command("01630c080000800000000000")

#   5.784    [HCI - COM3] LeSetEventMask(LE_Event_Mask=1048063)
#   5.784    COM3 -> 01 01 20 08 ff fd 0f 00 00 00 00 00 
send_command("01012008fffd0f0000000000")

#   5.886    [HCI - COM3] ReadLocalVerInfo()
#   5.886    COM3 -> 01 01 10 00 
send_command("01011000")

#   6.023    [HCI - COM3] LeReadLocalSupFeat()
#   6.023    COM3 -> 01 03 20 00 
send_command("01032000")

#   6.140    [HCI - COM3] LeWriteDefaultDataLength(TxOctets=251,
#   6.140    COM3 -> 01 24 20 04 fb 00 90 42 
send_command("01242004fb009042")

#   6.197    [HCI - COM3] LeSetDefaultPhy(ALL_PHYS=0,
#   6.197    COM3 -> 01 31 20 03 00 01 01 
send_command("01312003000101")

#   6.279    [HCI - COM3] LeReadMaxAdvDataLen()
#   6.280    COM3 -> 01 3a 20 00 
send_command("013a2000")

#  72.056    [HCI - COM3] LeSetExtAdvParam(Advertising_Handle=1,
#  72.056    COM3 -> 01 36 20 19 01 04 00 70 00 00 70 00 00 07 00 00 66 55 44 33 22 11 00 7f 01 00 01 0f 00 
send_command("01362019010400700000700000070000665544332211007f0100010f00")

#  72.115    [HCI - COM3] LeSetExtAdvData(Advertising_Handle=1,
#  72.115    COM3 -> 01 37 20 04 01 03 01 00 
send_command("0137200401030100")

#  72.124    [HCI - COM3] LeSetExtAdvEnable(Enable=True,
#  72.124    COM3 -> 01 39 20 06 01 01 01 00 00 00 
send_command("01392006010101000000")



while(1):
    read_events()