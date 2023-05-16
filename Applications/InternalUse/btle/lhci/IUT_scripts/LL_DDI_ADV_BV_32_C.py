#! /usr/bin/env python
filename="../common.py"
exec(open(filename).read())

# Define advertising sets 01.
advsets_tests=[
	[
		{'handle': 0x01, 'params': 0x13, 'intmin': b'\xcd\x00\x00', 'intmax': b'\xcd\x00\x00', 'peer': b'\xc8\x85\xf4\xf9\x2a\xe4', 'advsid': 0x0f, 'data': b''},
                {'handle': 0x02, 'params': 0x00, 'intmin': b'\xcd\x00\x00', 'intmax': b'\xcd\x00\x00', 'peer': b'\xc8\x85\xf4\xf9\x2a\xe4', 'advsid': 0x0e, 'data': b''}],
	[
		{'handle': 0x01, 'params': 0x13, 'intmin': b'\xdc\x00\x00', 'intmax': b'\xdc\x00\x00', 'peer': b'\xc8\x85\xf4\xf9\x2a\xe4', 'advsid': 0x0f, 'data': b''},
                {'handle': 0x02, 'params': 0x00, 'intmin': b'\xdc\x00\x00', 'intmax': b'\xdc\x00\x00', 'peer': b'\xc8\x85\xf4\xf9\x2a\xe4', 'advsid': 0x0e, 'data': b'0' * 0xbf}],
#	[
#		{'handle': 0x01, 'params': 0x00, 'intmin': b'\x20\x00\x00', 'intmax': b'\x00\x40\x00', 'peer': b'\xc8\x85\xf4\xf9\x2a\xe4', 'advsid': 0x0f, 'data': b''},
#		{'handle': 0x02, 'params': 0x10, 'intmin': b'\x20\x00\x00', 'intmax': b'\x00\x40\x00', 'peer': b'\xc8\x85\xf4\xf9\x2a\xe4', 'advsid': 0x0e, 'data': b''}],
#	[
#		{'handle': 0x01, 'params': 0x13, 'intmin': b'\x20\x00\x00', 'intmax': b'\x00\x40\x00', 'peer': b'\xc8\x85\xf4\xf9\x2a\xe4', 'advsid': 0x0f, 'data': b''},
#		{'handle': 0x02, 'params': 0x00, 'intmin': b'\x20\x00\x00', 'intmax': b'\x00\x40\x00', 'peer': b'\xc8\x85\xf4\xf9\x2a\xe4', 'advsid': 0x0e, 'data': b'0' * 191}],
#	[
#		{'handle': 0x01, 'params': 0x00, 'intmin': b'\x20\x00\x00', 'intmax': b'\x00\x40\x00', 'peer': b'\xc8\x85\xf4\xf9\x2a\xe4', 'advsid': 0x0f, 'data': b'0' * 191},
#		{'handle': 0x02, 'params': 0x13, 'intmin': b'\x20\x00\x00', 'intmax': b'\x00\x40\x00', 'peer': b'\xc8\x85\xf4\xf9\x2a\xe4', 'advsid': 0x0e, 'data': b'0' * 31}],
#	[
#		{'handle': 0x01, 'params': 0x13, 'intmin': b'\x20\x00\x00', 'intmax': b'\x00\x40\x00', 'peer': b'\xc8\x85\xf4\xf9\x2a\xe4', 'advsid': 0x0f, 'data': b'0' * 31},
#		{'handle': 0x02, 'params': 0x10, 'intmin': b'\x20\x00\x00', 'intmax': b'\x00\x40\x00', 'peer': b'\xc8\x85\xf4\xf9\x2a\xe4', 'advsid': 0x0e, 'data': b'0' * 31}],
#	[
#		{'handle': 0x01, 'params': 0x15, 'intmin': b'\x20\x00\x00', 'intmax': b'\x00\x40\x00', 'peer': b'\xc8\x85\xf4\xf9\x2a\xe4', 'advsid': 0x0f, 'data': b''},
#		{'handle': 0x02, 'params': 0x04, 'intmin': b'\x20\x00\x00', 'intmax': b'\x00\x40\x00', 'peer': b'\xc8\x85\xf4\xf9\x2a\xe4', 'advsid': 0x0e, 'data': b'0' * 251}],
#	[
#		{'handle': 0x01, 'params': 0x00, 'intmin': b'\x20\x00\x00', 'intmax': b'\x00\x40\x00', 'peer': b'\xc8\x85\xf4\xf9\x2a\xe4', 'advsid': 0x0f, 'data': b'0' * 253},
#		{'handle': 0x02, 'params': 0x1D, 'intmin': b'\x20\x00\x00', 'intmax': b'\x00\x40\x00', 'peer': b'\xc8\x85\xf4\xf9\x2a\xe4', 'advsid': 0x0e, 'data': b''}],
#	[
#		{'handle': 0x01, 'params': 0x00, 'intmin': b'\x20\x00\x00', 'intmax': b'\x00\x40\x00', 'peer': b'\xc8\x85\xf4\xf9\x2a\xe4', 'advsid': 0x0f, 'data': b'0' * 474},
#		{'handle': 0x02, 'params': 0x12, 'intmin': b'\x20\x00\x00', 'intmax': b'\x00\x40\x00', 'peer': b'\xc8\x85\xf4\xf9\x2a\xe4', 'advsid': 0x0e, 'data': b''}]
	]

# [HCI] Reset()
send_command("01030c00")
#input("Press ENTER key to continue...")

# [HCI] HCI event received: OPCODE_RESET (0xE)
#                   Command_Opcode = 0xC03 (3075)
#                   Event_Code = 0xE (14)
#                   Num_HCI_Command_Packets = 0x1 (1)
#                   Parameter_Length = 0x4 (4)
#                   Status = 0x0 (0)
# COM <- 04 0e 04 01 03 0c 00 

# [HCI] VsSetBdAddr(addr='\xc8\x85\xf4\xf9*\xe4')
# COM -> 01 f0 ff 06 c8 85 f4 f9 2a e4 
send_command("01f0ff06c885f4f92ae4")
#input("Press ENTER key to continue...")

# [HCI] HCI event received: OPCODE_VS_SET_BD_ADDR (0xE)
#                   Command_Opcode = 0xFFF0 (65520)
#                   Num_HCI_Command_Packets = 0x1 (1)
#                   Status = 0x0 (0)
# COM <- 04 0E 03 01 F0 FF

# [HCI] ReadBdAddr()
# COM -> 01 09 10 00 
send_command("01091000")
#input("Press ENTER key to continue...")

# [HCI] HCI event received: OPCODE_READ_BD_ADDR (0xE)
#                   BD_ADDR = 0xE42AF9F485C8 (250873233311176)
#                   Command_Opcode = 0x1009 (4105)
#                   Event_Code = 0xE (14)
#                   Num_HCI_Command_Packets = 0x1 (1)
#                   Parameter_Length = 0xA (10)
#                   Status = 0x0 (0)
# COM <- 04 0e 0a 01 09 10 00 c8 85 f4 f9 2a e4 

#########################################
# TODO: Verify whether these are in fact necessary.
## [HCI] SetEventMask(Event_Mask=2305983746702149776L)
## COM -> 01 01 0c 08 90 88 01 00 00 80 00 20 
#send_command("01010c089088010000800020")
#
## [HCI] HCI event received: OPCODE_SET_EVENT_MASK (0xE)
##                   Command_Opcode = 0xC01 (3073)
##                   Event_Code = 0xE (14)
##                   Num_HCI_Command_Packets = 0x1 (1)
##                   Parameter_Length = 0x4 (4)
##                   Status = 0x0 (0)
## COM <- 04 0e 04 01 01 0c 00 
#
## [HCI] SetEventMaskPage2(Event_Mask_Page_2=8388608)
## COM -> 01 63 0c 08 00 00 80 00 00 00 00 00 
#send_command("01630c080000800000000000")
#
## [HCI] HCI event received: OPCODE_SET_EVENT_MASK_PAGE2 (0xE)
##                   Command_Opcode = 0xC63 (3171)
##                   Event_Code = 0xE (14)
##                   Num_HCI_Command_Packets = 0x1 (1)
##                   Parameter_Length = 0x4 (4)
##                   Status = 0x0 (0)
## COM <- 04 0e 04 01 63 0c 00 
#
## [HCI] LeSetEventMask(LE_Event_Mask=536870399)
## COM -> 01 01 20 08 ff fd ff 1f 00 00 00 00 
#send_command("01012008fffdff1f00000000")
#
## [HCI] HCI event received: OPCODE_LE_SET_EVENT_MASK (0xE)
##                   Command_Opcode = 0x2001 (8193)
##                   Event_Code = 0xE (14)
##                   Num_HCI_Command_Packets = 0x1 (1)
##                   Parameter_Length = 0x4 (4)
##                   Status = 0x0 (0)
## COM <- 04 0e 04 01 01 20 00 
#
## [HCI] VsSetEventMask(eventMask=0)
## COM -> 01 e1 ff 08 00 00 00 00 00 00 00 00 
#send_command("01e1ff080000000000000000")
#
## [HCI] HCI event received: OPCODE_VS_SET_EVENT_MASK (0xE)
##                   Command_Opcode = 0xFFE1 (65505)
##                   Num_HCI_Command_Packets = 0x1 (1)
##                   Status = 0x0 (0)
## COM <- 04 0e 04 01 e1 ff 00 
#########################################

# [HCI] LeSetDefaultPhy(ALL_PHYS=0,
#             TX_PHYS=1,
#             RX_PHYS=1)
# COM -> 01 31 20 03 00 01 01 
send_command("01312003000101")
#input("Press ENTER key to continue...")

# [HCI] HCI event received: OPCODE_LE_SET_DEF_PHY (0xE)
#                   Command_Opcode = 0x2031 (8241)
#                   Event_Code = 0xE (14)
#                   Num_HCI_Command_Packets = 0x1 (1)
#                   Parameter_Length = 0x4 (4)
#                   Status = 0x0 (0)
# COM <- 04 0e 04 01 31 20 00 

for advsets in advsets_tests:
	for advset in advsets:
		# [HCI] LeSetExtAdvParam(Advertising_Handle=1,
		#              Advertising_Event_Properties=0,
		#              Primary_Advertising_Interval_Min=32,
		#              Primary_Advertising_Interval_Max=16384,
		#              Primary_Advertising_Channel_Map=7,
		#              Own_Address_Type=0,
		#              Peer_Address_Type=0,
		#              Peer_Address='\x00\x00\x00\x00\x00\x00',
		#              Advertising_Filter_Policy=0,
		#              Advertising_Tx_Power=127,
		#              Primary_Advertising_PHY=1,
		#              Secondary_Advertising_Max_Skip=0,
		#              Secondary_Advertising_PHY=1,
		#              Advertising_SID=15,
		#              Scan_Request_Notification_Enable=False)
		# COM -> 01 36 20 19 01 02 00 20 00 00 00 40 00 07 00 00 00 00 00 00 00 00 00 7f 01 00 01 0f 00 
		send_command("01362019" +
				"%0.2X" % (advset["handle"] % 256) +
				"%0.2X" % (advset["params"] % 256) + "00" +
				advset["intmin"].hex() +
				advset["intmax"].hex() +
				"070000" +
				advset["peer"].hex() +
				"007f010001" +
                                "%0.2X" % (advset["advsid"] % 256) +
                                "00")

		#input("Press ENTER key to continue...")

		# [HCI] HCI event received: OPCODE_LE_SET_EXT_ADV_PARAM (0xE)
		#                   Command_Opcode = 0x2036 (8246)
		#                   Event_Code = 0xE (14)
		#                   Num_HCI_Command_Packets = 0x1 (1)
		#                   Parameter_Length = 0x5 (5)
		#                   Selected_Tx_Power = 0x9 (9)
		#                   Status = 0x0 (0)
		# COM <- 04 0e 05 01 36 20 00 09 

	for advset in advsets:
		# [HCI] LeSetExtAdvData(Advertising_Handle=1,
		#                  Operation=3,
		#                  Fragment_Preference=1,
		#                  Advertising_Data='')
		# COM -> 01 37 20 03 01 03 00 00
		#send_command("0137200401030100")
		advset_data_len = len(advset["data"])
		frag_size = 255 - 4
		frag_cursize = frag_size
		advset_data_frag_count = int((advset_data_len + (frag_size - 1)) / frag_size)
		if (advset_data_len <= frag_size):
			if (advset_data_len > 0):
				send_command("013720" +
						("%0.2X" % (4 + advset_data_len)) +
						("%0.2X" % advset["handle"]) +
						"0301" +
						("%0.2X" % advset_data_len) +
						advset["data"].hex())
		else:
			for frag_iter in range(advset_data_frag_count):
				frag_start = (frag_size * frag_iter)
				frag_end = (frag_size * (frag_iter + 1))
				if (frag_iter == 0):
					operation = 1
				elif (frag_iter == (advset_data_frag_count - 1)):
					operation = 2
					frag_end = len(advset["data"])
					frag_cursize = (frag_end - frag_start)
				else:
					operation = 4
				send_command("013720" +
						("%0.2X" % (4 + frag_cursize)) +
						("%0.2X" % advset["handle"]) +
						("%0.2X" % operation) +
						"01" +
						("%0.2X" % frag_cursize) +
						advset["data"][frag_start:frag_end].hex())
				#input("Press ENTER key to continue...")

		# [HCI] HCI event received: OPCODE_LE_SET_EXT_ADV_DATA (0xE)
		#                   Command_Opcode = 0x???? (????)
		#                   Event_Code = 0x? (??)
		#                   Num_HCI_Command_Packets = 0x1 (1)
		#                   Parameter_Length = 0x4 (4)
		#                   Status = 0x0 (0)
		# COM <- 04 0E 04 01 37 20 00 

	# [HCI] LeSetExtAdvEnable(Enable=True,
	#               Num_Sets=1,
	#               enable_param=[{'Duration': 0,
	#               'Advertising_Handle': 1,
	#               'Max_Extended_Advertising_Events': 0}])
	# COM -> 01 39 20 06 01 01 01 00 00 00 00 
	#for advset in advsets:
	#	send_command("013920060101" +
	#			"%0.2X" % (advset["handle"] % 256) +
	#			"000000")
	#	#input("Press ENTER key to continue...")
	payload = b'\x01' # Enable
	payload = payload + bytes([len(advsets)])
	for advset_iter, advset in enumerate(advsets):
		payload = payload + bytes([advsets[advset_iter]["handle"]]);
		payload = payload + b'\0\0\0'
	#input("Press ENTER key to continue...")
	send_command("013920" +
			("%0.2X" % len(payload)) +
			payload.hex())

	# [HCI] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
	#                   Command_Opcode = 0x2039 (8249)
	#                   Event_Code = 0xE (14)
	#                   Num_HCI_Command_Packets = 0x1 (1)
	#                   Parameter_Length = 0x4 (4)
	#                   Status = 0x0 (0)
	# COM <- 04 0e 04 01 39 20 00 

	tf = datetime.datetime.now()
	print("  ", end="")
	print(tf-ts, end="| ")
	print("Wait 10s to transmit advertisements.");
	sleep(10)

	# [HCI] LeSetExtAdvEnable(Enable=False,
	#               Num_Sets=2,
	#               enable_param=[{'Duration': 0,
	#               'Advertising_Handle': 1,
	#               'Max_Extended_Advertising_Events': 0},
	#               enable_param=[{'Duration': 0,
	#               'Advertising_Handle': 2,
	#               'Max_Extended_Advertising_Events': 0}])
	# COM -> 01 39 20 0A 00 02 01 00 00 00 00 02 00 00 00 00 
	payload = b'\x00' # Disable
	payload = payload + bytes([len(advsets)])
	for advset_iter, advset in enumerate(advsets):
		payload = payload + bytes([advsets[advset_iter]["handle"]]);
		payload = payload + b'\0\0\0'
	send_command("013920" +
			("%0.2X" % len(payload)) +
			payload.hex())
	#send_command("013920020000")
	#input("Press ENTER key to continue...")

	# [HCI] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
	#                   Command_Opcode = 0x2039 (8249)
	#                   Event_Code = 0xE (14)
	#                   Num_HCI_Command_Packets = 0x1 (1)
	#                   Parameter_Length = 0x4 (4)
	#                   Status = 0x0 (0)
	# COM <- 04 0e 04 01 39 20 00 

	# [HCI] LeClearAdvSets()
	# COM -> 01 3D 20 00
	send_command("013D2000");
	#input("Press ENTER key to continue...")

	# [HCI] HCI event received: OPCODE_LE_CLEAR_ADV_SETS (0xE)
	#                   Command_Opcode = 0x203D (8253)
	#                   Event_Code = 0xE (14)
	#                   Num_HCI_Command_Packets = 0x1 (1)
	#                   Parameter_Length = 0x4 (4)
	#                   Status = 0x0 (0)
	# COM <- 04 0e 04 01 3D 20 00 

tf = datetime.datetime.now()
while((datetime.datetime.now() - tf) > datetime.timedelta(seconds = 10)):
  rcv_event()

