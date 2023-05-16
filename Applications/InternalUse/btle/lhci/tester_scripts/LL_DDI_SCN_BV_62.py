filename="../common.py"
exec(open(filename).read())

testvar={}
if len(sys.argv) > 1:
	for i in range(2,len(sys.argv)):
		testvar[sys.argv[i]] = True;
else:
	testvar = {
		'1': True,
		'2': True,
		'3': True,
		'4': True,
		'5': True,
		'6': True
	}

#   5.027    [HCI - COM51] Reset()
#   5.027    COM51 -> 01 03 0c 00 
send_command("01030c00")

#   5.042    [HCI - COM51] HCI event received: OPCODE_RESET (0xE)
#                              Command_Opcode = 0xC03 (3075)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   5.042    COM51 <- 04 0e 04 01 03 0c 00 

#   5.214    [HCI - COM51] Reset()
#   5.214    COM51 -> 01 03 0c 00 
send_command("01030c00")

#   5.230    [HCI - COM51] HCI event received: OPCODE_RESET (0xE)
#                              Command_Opcode = 0xC03 (3075)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   5.230    COM51 <- 04 0e 04 01 03 0c 00 

#   5.246    [HCI - COM51] VsGetLicenseStatus()
#   5.246    COM51 -> 01 02 fc 00 
send_command("0102fc00")

#   5.261    [HCI - COM51] HCI event received: <unknown> (0xE)
#                              Command_Opcode = 0xFC02 (64514)
#                              LicenseStatus = "(2,)"
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Status = 0x0 (0)
#   5.261    COM51 <- 04 0e 08 01 02 fc 00 02 00 00 00 

#   5.324    [HCI - COM51] VsSetLicenseInfo(licenseInfo=[120,
#                         0,
#                         0,
#                         0,
#                         0,
#                         32,
#                         67,
#                         20,
#                         32,
#                         118,
#                         203,
#                         212,
#                         23,
#                         178,
#                         5,
#                         65,
#                         203,
#                         190,
#                         106,
#                         102,
#                         23,
#                         137,
#                         255,
#                         131,
#                         54,
#                         132,
#                         20,
#                         76,
#                         203,
#                         172,
#                         233,
#                         200,
#                         148,
#                         94,
#                         108,
#                         240,
#                         136,
#                         236,
#                         1,
#                         8,
#                         55,
#                         250,
#                         202,
#                         28,
#                         0,
#                         0,
#                         0,
#                         0,
#                         2,
#                         8,
#                         1,
#                         214,
#                         72,
#                         40,
#                         18,
#                         123,
#                         192,
#                         0,
#                         255,
#                         64,
#                         62,
#                         116,
#                         17,
#                         121,
#                         66,
#                         158,
#                         22,
#                         103,
#                         238,
#                         12,
#                         53,
#                         230,
#                         71,
#                         231,
#                         185,
#                         42,
#                         38,
#                         154,
#                         222,
#                         82,
#                         180,
#                         75,
#                         76,
#                         229,
#                         67,
#                         136,
#                         101,
#                         202,
#                         7,
#                         61,
#                         223,
#                         162,
#                         239,
#                         128,
#                         50,
#                         137,
#                         37,
#                         10,
#                         19,
#                         37,
#                         165,
#                         197,
#                         78,
#                         134,
#                         12,
#                         235,
#                         155,
#                         24,
#                         171,
#                         46,
#                         119,
#                         122,
#                         252,
#                         119,
#                         142,
#                         17,
#                         239,
#                         117,
#                         227,
#                         231,
#                         208,
#                         74,
#                         8,
#                         242])
#   5.324    COM51 -> 01 01 fc 7c 78 00 00 00 00 20 43 14 20 76 cb d4 17 b2 05 41 cb be 6a 66 17 89 ff 83 36 84 14 4c cb ac e9 c8 94 5e 6c f0 88 ec 01 08 37 fa ca 1c 00 00 00 00 02 08 01 d6 48 28 12 7b c0 00 ff 40 3e 74 11 79 42 9e 16 67 ee 0c 35 e6 47 e7 b9 2a 26 9a de 52 b4 4b 4c e5 43 88 65 ca 07 3d df a2 ef 80 32 89 25 0a 13 25 a5 c5 4e 86 0c eb 9b 18 ab 2e 77 7a fc 77 8e 11 ef 75 e3 e7 d0 4a 08 f2 
send_command("0101fc7c78000000002043142076cbd417b20541cbbe6a661789ff833684144ccbace9c8945e6cf088ec010837faca1c00000000020801d64828127bc000ff403e741179429e1667ee0c35e647e7b92a269ade52b44b4ce5438865ca073ddfa2ef803289250a1325a5c54e860ceb9b18ab2e777afc778e11ef75e3e7d04a08f2")

#   5.527    [HCI - COM51] HCI event received: <unknown> (0xE)
#                              Command_Opcode = 0xFC01 (64513)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Status = 0x0 (0)
#   5.527    COM51 <- 04 0e 04 01 01 fc 00 

#   5.542    [HCI - COM51] VsGetLicenseStatus()
#   5.542    COM51 -> 01 02 fc 00 
send_command("0102fc00")

#   5.558    [HCI - COM51] HCI event received: <unknown> (0xE)
#                              Command_Opcode = 0xFC02 (64514)
#                              LicenseStatus = "(0,)"
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Status = 0x0 (0)
#   5.558    COM51 <- 04 0e 08 01 02 fc 00 00 00 00 00 

#   5.605    [HCI - COM51] VsGetSignature(salt=(1,
#                       201,
#                       250,
#                       41))
#   5.605    COM51 -> 01 00 fc 04 01 c9 fa 29 
send_command("0100fc0401c9fa29")

#   5.730    [HCI - COM51] HCI event received: <unknown> (0xE)
#                              Command_Opcode = 0xFC00 (64512)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Signature = "(52, 227, 83, 243, 165, 141, 254, 79, 166, 97, 90, 239, 66, 118, 49, 30, 100, 126, 193, 217, 138, 197, 147, 215, 121, 39, 135, 57, 130, 56, 42, 158, 201, 91, 114, 69, 195, 235, 48, 85, 114, 130, 221, 230, 44, 245, 164, 45, 101, 41, 245, 20, 182, 22, 242, 161, 15, 53, 173, 222, 85, 104, 147, 114)"
#                              Status = 0x0 (0)
#   5.730    COM51 <- 04 0e 44 01 00 fc 00 34 e3 53 f3 a5 8d fe 4f a6 61 5a ef 42 76 31 1e 64 7e c1 d9 8a c5 93 d7 79 27 87 39 82 38 2a 9e c9 5b 72 45 c3 eb 30 55 72 82 dd e6 2c f5 a4 2d 65 29 f5 14 b6 16 f2 a1 0f 35 ad de 55 68 93 72 

#   5.886    [HCI - COM51] VsSetBdAddr(addr='\xc8\x85\xf4\xf9*\xe4')
#   5.886    COM51 -> 01 f0 ff 06 c8 85 f4 f9 2a e4 
send_command("01f0ff06c885f4f92ae4")

#   5.902    [HCI - COM51] HCI event received: OPCODE_VS_SET_BD_ADDR (0xE)
#                              Command_Opcode = 0xFFF0 (65520)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Status = 0x0 (0)
#   5.902    COM51 <- 04 0e 04 01 f0 ff 00 

#   6.027    [HCI - COM51] ReadBdAddr()
#   6.027    COM51 -> 01 09 10 00 
send_command("01091000")

#   6.042    [HCI - COM51] HCI event received: OPCODE_READ_BD_ADDR (0xE)
#                              BD_ADDR = 0xE42AF9F485C8 (250873233311176)
#                              Command_Opcode = 0x1009 (4105)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0xA (10)
#                              Status = 0x0 (0)
#   6.042    COM51 <- 04 0e 0a 01 09 10 00 c8 85 f4 f9 2a e4 

#   6.495    [HCI - COM51] SetEventMask(Event_Mask=2305983746702149776L)
#   6.495    COM51 -> 01 01 0c 08 90 88 01 00 00 80 00 20 
send_command("01010c089088010000800020")

#   6.511    [HCI - COM51] HCI event received: OPCODE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0xC01 (3073)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.511    COM51 <- 04 0e 04 01 01 0c 00 

#   6.558    [HCI - COM51] SetEventMaskPage2(Event_Mask_Page_2=8388608)
#   6.558    COM51 -> 01 63 0c 08 00 00 80 00 00 00 00 00 
send_command("01630c080000800000000000")

#   6.574    [HCI - COM51] HCI event received: OPCODE_SET_EVENT_MASK_PAGE2 (0xE)
#                              Command_Opcode = 0xC63 (3171)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.574    COM51 <- 04 0e 04 01 63 0c 00 

#   6.620    [HCI - COM51] LeSetEventMask(LE_Event_Mask=536870399)
#   6.620    COM51 -> 01 01 20 08 ff fd ff 1f 00 00 00 00 
send_command("01012008fffdff1f00000000")

#   6.636    [HCI - COM51] HCI event received: OPCODE_LE_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0x2001 (8193)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   6.636    COM51 <- 04 0e 04 01 01 20 00 

#   6.698    [HCI - COM51] VsSetEventMask(eventMask=0)
#   6.698    COM51 -> 01 e1 ff 08 00 00 00 00 00 00 00 00 
send_command("01e1ff080000000000000000")

#   6.714    [HCI - COM51] HCI event received: OPCODE_VS_SET_EVENT_MASK (0xE)
#                              Command_Opcode = 0xFFE1 (65505)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Status = 0x0 (0)
#   6.714    COM51 <- 04 0e 04 01 e1 ff 00 

#   6.870    [HCI - COM51] ReadLocalVerInfo()
#   6.870    COM51 -> 01 01 10 00 
send_command("01011000")

#   6.886    [HCI - COM51] HCI event received: OPCODE_READ_LOCAL_VER_INFO (0xE)
#                              Command_Opcode = 0x1001 (4097)
#                              Event_Code = 0xE (14)
#                              HCI_Revision = 0x0 (0)
#                              HCI_Version = 0xB (11)
#                              LMP_Subversion = 0x0 (0)
#                              LMP_Version = 0xB (11)
#                              Manufacturer_Name = 0x59A (1434)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0xC (12)
#                              Status = 0x0 (0)
#   6.886    COM51 <- 04 0e 0c 01 01 10 00 0b 00 00 0b 9a 05 00 00 

#   6.902    [HCI - COM51] VsGetFwVersion()
#   6.902    COM51 -> 01 04 fc 00 
send_command("0104fc00")

#   6.917    [HCI - COM51] HCI event received: <unknown> (0xE)
#                              Command_Opcode = 0xFC04 (64516)
#                              FwVersion = "(89,)"
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Status = 0x0 (0)
#   6.917    COM51 <- 04 0e 08 01 04 fc 00 59 00 00 00 

#   7.042    [HCI - COM51] LeReadLocalSupFeat()
#   7.042    COM51 -> 01 03 20 00 
send_command("01032000")

#   7.058    [HCI - COM51] HCI event received: OPCODE_LE_READ_LOCAL_SUP_FEAT (0xE)
#                              Command_Opcode = 0x2003 (8195)
#                              Event_Code = 0xE (14)
#                              LE_Features = 0x3FFF7FFF (1073709055)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0xC (12)
#                              Status = 0x0 (0)
#   7.058    COM51 <- 04 0e 0c 01 03 20 00 ff 7f ff 3f 00 00 00 00 

#   7.401    [HCI - COM51] LeSetDefaultPhy(ALL_PHYS=0,
#                        TX_PHYS=1,
#                        RX_PHYS=1)
#   7.401    COM51 -> 01 31 20 03 00 01 01 
send_command("01312003000101")

#   7.417    [HCI - COM51] HCI event received: OPCODE_LE_SET_DEF_PHY (0xE)
#                              Command_Opcode = 0x2031 (8241)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#   7.417    COM51 <- 04 0e 04 01 31 20 00 

#   7.682    [HCI - COM51] LeReadNumSupAdvSets()
#   7.682    COM51 -> 01 3b 20 00 
send_command("013b2000")

if ('1' in testvar and testvar.get('1')):
	#  16.759    [HCI - COM51] LeSetExtAdvParam(Advertising_Handle=1,
	#                         Advertising_Event_Properties=2,
	#                         Primary_Advertising_Interval_Min=128,
	#                         Primary_Advertising_Interval_Max=128,
	#                         Primary_Advertising_Channel_Map=7,
	#                         Own_Address_Type=0,
	#                         Peer_Address_Type=0,
	#                         Peer_Address='V4\x12_\x00\x00',
	#                         Advertising_Filter_Policy=0,
	#                         Advertising_Tx_Power=127,
	#                         Primary_Advertising_PHY=1,
	#                         Secondary_Advertising_Max_Skip=0,
	#                         Secondary_Advertising_PHY=1,
	#                         Advertising_SID=15,
	#                         Scan_Request_Notification_Enable=1)
	#  16.759    COM51 -> 01 36 20 19 01 02 00 80 00 00 80 00 00 07 00 00 56 34 12 5f 00 00 00 7f 01 00 01 0f 01 
	send_command("013620190102008000008000000700005634125f0000007f0100010f01")

#  16.774    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_PARAM (0xE)
#                              Command_Opcode = 0x2036 (8246)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x5 (5)
#                              Selected_Tx_Power = 0x9 (9)
#                              Status = 0x0 (0)
#  16.774    COM51 <- 04 0e 05 01 36 20 00 09 

if ('1' in testvar and testvar.get('1')):
	#  16.821    [HCI - COM51] LeSetExtScanRespData(Advertising_Handle=1,
	#                             Operation=1,
	#                             Fragment_Preference=1,
	#                             Scan_Response_Data='\xc1\x13\xc8P\x1102mH\x9a\xd2\x8c\x82\xfd\x92\x0b\x0b|\xdb>l\x0f\xf3u\x8cg4\xd6\x9f\x96 \xe4\xfe\x17\x0e\xce\xb5\x1e\xe6K\x1da\xca\xe4\x12}\xc3\xa2\xae\xf0\xb0i\xeb\xc6/\xb1Go\xf60\xe5\x05\x95\xbb\xf3\xcc\x08\x05\x0cP\xe1B\xf0)l\xc3(\xccZ\xeaLa\xcc\xdb\xe7`\xadh!\xbb\xa1\xc0\xdfy\xb7\xa9U\xb4I\xb8e\xf9\xb8\x10\x1f\'\x18\xdbM\x96\xaf\xeb:\xed"\x9f\x11\x12\xec\x95\x90\x1ea"\xb8\xe4=\x0f\xe5\x8d\x9c\x1b\x1bG\xfc\xf9\xbd\xf3 \xe7\xc4\xde\xc25\xda\x87.\x97\x1c\x89w\x0c\x8e\x88\xe9`\xe0$:\xca\x8e\x1b\x17!\x0e\xf6\xb5\xb6nf\x95&\x1d/Z.\xb2g;\xfe\xff\x88\x87\xe5\xf4b\xeb\xf0zh\xcd\x06\xa4\xa4\xe9_Yx\x13Lf8\x94Nj\xc6\xbeIG\xc8\xe1\xb2\xb0@\xd0J|09Wp/\xf2\xd0\xc2\x14\xe3U}. \\\xacR\xf1s\x84\x87\xc4S\x85\xb5\xb6\xa7\xdbTy\x99\x87\xd8\xb4')
	#  16.821    COM51 -> 01 38 20 ff 01 01 01 fb c1 13 c8 50 11 30 32 6d 48 9a d2 8c 82 fd 92 0b 0b 7c db 3e 6c 0f f3 75 8c 67 34 d6 9f 96 20 e4 fe 17 0e ce b5 1e e6 4b 1d 61 ca e4 12 7d c3 a2 ae f0 b0 69 eb c6 2f b1 47 6f f6 30 e5 05 95 bb f3 cc 08 05 0c 50 e1 42 f0 29 6c c3 28 cc 5a ea 4c 61 cc db e7 60 ad 68 21 bb a1 c0 df 79 b7 a9 55 b4 49 b8 65 f9 b8 10 1f 27 18 db 4d 96 af eb 3a ed 22 9f 11 12 ec 95 90 1e 61 22 b8 e4 3d 0f e5 8d 9c 1b 1b 47 fc f9 bd f3 20 e7 c4 de c2 35 da 87 2e 97 1c 89 77 0c 8e 88 e9 60 e0 24 3a ca 8e 1b 17 21 0e f6 b5 b6 6e 66 95 26 1d 2f 5a 2e b2 67 3b fe ff 88 87 e5 f4 62 eb f0 7a 68 cd 06 a4 a4 e9 5f 59 78 13 4c 66 38 94 4e 6a c6 be 49 47 c8 e1 b2 b0 40 d0 4a 7c 30 39 57 70 2f f2 d0 c2 14 e3 55 7d 2e 20 5c ac 52 f1 73 84 87 c4 53 85 b5 b6 a7 db 54 79 99 87 d8 b4 
	send_command("013820ff010101fbc113c8501130326d489ad28c82fd920b0b7cdb3e6c0ff3758c6734d69f9620e4fe170eceb51ee64b1d61cae4127dc3a2aef0b069ebc62fb1476ff630e50595bbf3cc08050c50e142f0296cc328cc5aea4c61ccdbe760ad6821bba1c0df79b7a955b449b865f9b8101f2718db4d96afeb3aed229f1112ec95901e6122b8e43d0fe58d9c1b1b47fcf9bdf320e7c4dec235da872e971c89770c8e88e960e0243aca8e1b17210ef6b5b66e6695261d2f5a2eb2673bfeff8887e5f462ebf07a68cd06a4a4e95f5978134c6638944e6ac6be4947c8e1b2b040d04a7c303957702ff2d0c214e3557d2e205cac52f1738487c45385b5b6a7db54799987d8b4")

#  17.024    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_SCAN_RESP_DATA (0xE)
#                              Command_Opcode = 0x2038 (8248)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  17.024    COM51 <- 04 0e 04 01 38 20 00 

if ('1' in testvar and testvar.get('1')):
	#  17.071    [HCI - COM51] LeSetExtScanRespData(Advertising_Handle=1,
	#                             Operation=2,
	#                             Fragment_Preference=1,
	#                             Scan_Response_Data='\x94\xfc\xc9r\xc4')
	#  17.071    COM51 -> 01 38 20 09 01 02 01 05 94 fc c9 72 c4 
	send_command("013820090102010594fcc972c4")

#  17.087    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_SCAN_RESP_DATA (0xE)
#                              Command_Opcode = 0x2038 (8248)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  17.087    COM51 <- 04 0e 04 01 38 20 00 

if ('1' in testvar and testvar.get('1')):
	#  17.134    [HCI - COM51] LeSetExtAdvEnable(Enable=True,
	#                          Advertising_Handle=1,
	#                          Duration=0,
	#                          Max_Extended_Advertising_Events=0,
	#                          enable_param=[{'Duration': 0,
	#                          'Advertising_Handle': 1,
	#                          'Max_Extended_Advertising_Events': 0}])
	#  17.134    COM51 -> 01 39 20 06 01 01 01 00 00 00 
	send_command("01392006010101000000")

#  17.149    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  17.149    COM51 <- 04 0e 04 01 39 20 00 

#  17.149    [HCI - COM51] HCI event received: SUBEVT_LE_SCAN_REQ_RCVD_EVT (0x3E)
#                              Advertising_Handle = 0x1 (1)
#                              Event_Code = 0x3E (62)
#                              Parameter_Length = 0x9 (9)
#                              Scanner_Address = 0x5F123456 (1595028566)
#                              Scanner_Address_Type = 0x0 (0)
#                              Subevent_Code = 0x13 (19)
#  17.149    COM51 <- 04 3e 09 13 01 00 56 34 12 5f 00 00 

#  17.196    [HCI - COM51] VsSetOpFlags(flags=2048,
#                     enable=False)
#  17.196    COM51 -> 01 f3 ff 05 00 08 00 00 00 
send_command("01f3ff050008000000")

#  17.212    [HCI - COM51] HCI event received: OPCODE_VS_SET_OP_FLAGS (0xE)
#                              Command_Opcode = 0xFFF3 (65523)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Status = 0x0 (0)
#  17.212    COM51 <- 04 0e 04 01 f3 ff 00 

if ('2' in testvar and testvar.get('2')):
	#  17.259    [HCI - COM51] LeSetExtAdvParam(Advertising_Handle=2,
	#                         Advertising_Event_Properties=2,
	#                         Primary_Advertising_Interval_Min=128,
	#                         Primary_Advertising_Interval_Max=128,
	#                         Primary_Advertising_Channel_Map=7,
	#                         Own_Address_Type=0,
	#                         Peer_Address_Type=0,
	#                         Peer_Address='V4\x12_\x00\x00',
	#                         Advertising_Filter_Policy=0,
	#                         Advertising_Tx_Power=127,
	#                         Primary_Advertising_PHY=1,
	#                         Secondary_Advertising_Max_Skip=0,
	#                         Secondary_Advertising_PHY=1,
	#                         Advertising_SID=14,
	#                         Scan_Request_Notification_Enable=1)
	#  17.259    COM51 -> 01 36 20 19 02 02 00 80 00 00 80 00 00 07 00 00 56 34 12 5f 00 00 00 7f 01 00 01 0e 01 
	send_command("013620190202008000008000000700005634125f0000007f0100010e01")

#  17.274    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_PARAM (0xE)
#                              Command_Opcode = 0x2036 (8246)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x5 (5)
#                              Selected_Tx_Power = 0x9 (9)
#                              Status = 0x0 (0)
#  17.274    COM51 <- 04 0e 05 01 36 20 00 09 

if ('2' in testvar and testvar.get('2')):
	#  17.321    [HCI - COM51] LeSetExtScanRespData(Advertising_Handle=2,
	#                             Operation=1,
	#                             Fragment_Preference=1,
	#                             Scan_Response_Data='L7D6g<K\xe7\xe4_\r\x9d\xc0\x9c\x8d\x7f\xda\x80Q\x90\xf4G\xef\xe8\xf9U\xd9\x97isFt\xc5\xd8\x02\x96\xa3\xf8\tdn0*\xdel\x16\xe8\xe1\xc5\xf1\xf6\xb5$\xa8\xfd\xbd\x82,v/\xa5\x94\xabJ\x98\xffLw\x99\xe9\xae\xc7\x16\x02\xd8B\xba\x07\xb3d&Z"fB\xfb\x88T\x94\xfaG\xf5\xc1\xa74b{\xcd\xd4\xff<\xe5\xb9\xf2\xf7\x07\xfa?\x03\xfe\xc8\xfa@\x8d\xb5\xf5[r\xe5\x7f\xe9Q\xb9\xdf\xbf\x8a\x02?07\xfd\xd7_\xe2\x84\x15\x1e;\xb5\x18\x9e\x85\xd5\xf5us3G\xcd9S\x883\xfb!N\xc9\x13Y\xef\x8f"U?s&X\xce\xbdn\x07\x1d1\x1eh\xb3\xf3Q\xd0I\xf1\x15\xeb\xfdn\x83w\xde\xf70z\x077?\xffOPr4\x92_\xf5\xa53\xd76r\x81i\xdf\xdeYj\xfeKI\x84\x90\xde\xcf%\xf0{\xfd0\xdc\x9c\xd6\xf1O^\x87uM\xe8\\ \xe33\\\xa8\xdbS\x11w\xbf\xdd9\x08\xa9T')
	#  17.321    COM51 -> 01 38 20 ff 02 01 01 fb 4c 37 44 36 67 3c 4b e7 e4 5f 0d 9d c0 9c 8d 7f da 80 51 90 f4 47 ef e8 f9 55 d9 97 69 73 46 74 c5 d8 02 96 a3 f8 09 64 6e 30 2a de 6c 16 e8 e1 c5 f1 f6 b5 24 a8 fd bd 82 2c 76 2f a5 94 ab 4a 98 ff 4c 77 99 e9 ae c7 16 02 d8 42 ba 07 b3 64 26 5a 22 66 42 fb 88 54 94 fa 47 f5 c1 a7 34 62 7b cd d4 ff 3c e5 b9 f2 f7 07 fa 3f 03 fe c8 fa 40 8d b5 f5 5b 72 e5 7f e9 51 b9 df bf 8a 02 3f 30 37 fd d7 5f e2 84 15 1e 3b b5 18 9e 85 d5 f5 75 73 33 47 cd 39 53 88 33 fb 21 4e c9 13 59 ef 8f 22 55 3f 73 26 58 ce bd 6e 07 1d 31 1e 68 b3 f3 51 d0 49 f1 15 eb fd 6e 83 77 de f7 30 7a 07 37 3f ff 4f 50 72 34 92 5f f5 a5 33 d7 36 72 81 69 df de 59 6a fe 4b 49 84 90 de cf 25 f0 7b fd 30 dc 9c d6 f1 4f 5e 87 75 4d e8 5c 20 e3 33 5c a8 db 53 11 77 bf dd 39 08 a9 54 
	send_command("013820ff020101fb4c374436673c4be7e45f0d9dc09c8d7fda805190f447efe8f955d99769734674c5d80296a3f809646e302ade6c16e8e1c5f1f6b524a8fdbd822c762fa594ab4a98ff4c7799e9aec71602d842ba07b364265a226642fb885494fa47f5c1a734627bcdd4ff3ce5b9f2f707fa3f03fec8fa408db5f55b72e57fe951b9dfbf8a023f3037fdd75fe284151e3bb5189e85d5f575733347cd39538833fb214ec91359ef8f22553f732658cebd6e071d311e68b3f351d049f115ebfd6e8377def7307a07373fff4f507234925ff5a533d736728169dfde596afe4b498490decf25f07bfd30dc9cd6f14f5e87754de85c20e3335ca8db531177bfdd3908a954")

#  17.399    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_SCAN_RESP_DATA (0xE)
#                              Command_Opcode = 0x2038 (8248)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  17.399    COM51 <- 04 0e 04 01 38 20 00 

if ('2' in testvar and testvar.get('2')):
	#  17.430    [HCI - COM51] LeSetExtScanRespData(Advertising_Handle=2,
	#                             Operation=2,
	#                             Fragment_Preference=1,
	#                             Scan_Response_Data='\xc8\xf3\xca\xe4-')
	#  17.430    COM51 -> 01 38 20 09 02 02 01 05 c8 f3 ca e4 2d 
	send_command("0138200902020105c8f3cae42d")

#  17.446    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_SCAN_RESP_DATA (0xE)
#                              Command_Opcode = 0x2038 (8248)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  17.446    COM51 <- 04 0e 04 01 38 20 00 

if ('2' in testvar and testvar.get('2')):
	#  17.493    [HCI - COM51] LeSetExtAdvEnable(Enable=True,
	#                          Advertising_Handle=2,
	#                          Duration=0,
	#                          Max_Extended_Advertising_Events=0,
	#                          enable_param=[{'Duration': 0,
	#                          'Advertising_Handle': 2,
	#                          'Max_Extended_Advertising_Events': 0}])
	#  17.493    COM51 -> 01 39 20 06 01 01 02 00 00 00 
	send_command("01392006010102000000")

#  17.509    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  17.509    COM51 <- 04 0e 04 01 39 20 00 

#  17.556    [HCI - COM51] VsSetOpFlags(flags=2048,
#                     enable=False)
#  17.556    COM51 -> 01 f3 ff 05 00 08 00 00 00 
send_command("01f3ff050008000000")

#  17.571    [HCI - COM51] HCI event received: OPCODE_VS_SET_OP_FLAGS (0xE)
#                              Command_Opcode = 0xFFF3 (65523)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Status = 0x0 (0)
#  17.571    COM51 <- 04 0e 04 01 f3 ff 00 

#  17.602    [HCI - COM51] HCI event received: SUBEVT_LE_SCAN_REQ_RCVD_EVT (0x3E)
#                              Advertising_Handle = 0x2 (2)
#                              Event_Code = 0x3E (62)
#                              Parameter_Length = 0x9 (9)
#                              Scanner_Address = 0x5F123456 (1595028566)
#                              Scanner_Address_Type = 0x0 (0)
#                              Subevent_Code = 0x13 (19)
#  17.602    COM51 <- 04 3e 09 13 02 00 56 34 12 5f 00 00 

if ('3' in testvar and testvar.get('3')):
	#  17.618    [HCI - COM51] LeSetExtAdvParam(Advertising_Handle=3,
	#                         Advertising_Event_Properties=2,
	#                         Primary_Advertising_Interval_Min=128,
	#                         Primary_Advertising_Interval_Max=128,
	#                         Primary_Advertising_Channel_Map=7,
	#                         Own_Address_Type=0,
	#                         Peer_Address_Type=0,
	#                         Peer_Address='V4\x12_\x00\x00',
	#                         Advertising_Filter_Policy=0,
	#                         Advertising_Tx_Power=127,
	#                         Primary_Advertising_PHY=3,
	#                         Secondary_Advertising_Max_Skip=0,
	#                         Secondary_Advertising_PHY=3,
	#                         Advertising_SID=13,
	#                         Scan_Request_Notification_Enable=1)
	#  17.618    COM51 -> 01 36 20 19 03 02 00 80 00 00 80 00 00 07 00 00 56 34 12 5f 00 00 00 7f 03 00 03 0d 01 
	send_command("013620190302008000008000000700005634125f0000007f0300030d01")

#  17.634    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_PARAM (0xE)
#                              Command_Opcode = 0x2036 (8246)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x5 (5)
#                              Selected_Tx_Power = 0x9 (9)
#                              Status = 0x0 (0)
#  17.634    COM51 <- 04 0e 05 01 36 20 00 09 

if ('3' in testvar and testvar.get('3')):
	#  17.680    [HCI - COM51] LeSetExtScanRespData(Advertising_Handle=3,
	#                             Operation=1,
	#                             Fragment_Preference=1,
	#                             Scan_Response_Data='M\x8f\xd5\x81\x0e\xc8\xaa|\xac\\\xb3\xed\x0b\xedEb\x87\xe3\xf7E\x0bp\x8e\xa9\x08\xa2jL!,s\xae\xad_8\xa18G\x11E\x88\xe4M\xddM \x0c\xd5\xe56\xdcP\xbf\xa9e%\x14\xa9\x9d\xc1\x1d\x84F#\x82d \xafz\xbd\x1e\xa9\x11\xfa\xe1\xad\x05\x1a\xa1]j\xc7\xbd\xd1A\xee\x16\xaa\x8e\x8f\x1c\x06\r"+\x90\x08\xffJ\xf8\xceD\xc9\xcc\x1c\x91G\xb0L(m\xcd\x80\xec\xcfA6T\'\x17\xdbW\xc0\x87\x02qE\x06\xb5v\xef=\xe2p\xff5=\t\x1a\x01\xfe\xa9a\xa5\n\xd5\xf7\xa9`M\xcfRj\x96\x16\x079b!\x845[\xed\xf4.\x9dD*\x9e\x9ajo\x8d@\x0f\t\x0eZ\x82\xcc\xfc\xae\xe8\xe2\x88\\\xe0f\x0b\xea4FJ\x02\x8a\x82\xf4N)\xed>%0\x10!\xea\x84\xe6(X\xca\x1b\xba\xe2f;\xdeAGC\xe6\xf4\xc1\xaa\xd3\x1e\xae\xae^\xe99\xe5A9\xec\x80\xce\xf3\x8c\x11\xa5\x1f\xe0\xde<\xd1\x17\xe9U\x0b!')
	#  17.680    COM51 -> 01 38 20 ff 03 01 01 fb 4d 8f d5 81 0e c8 aa 7c ac 5c b3 ed 0b ed 45 62 87 e3 f7 45 0b 70 8e a9 08 a2 6a 4c 21 2c 73 ae ad 5f 38 a1 38 47 11 45 88 e4 4d dd 4d 20 0c d5 e5 36 dc 50 bf a9 65 25 14 a9 9d c1 1d 84 46 23 82 64 20 af 7a bd 1e a9 11 fa e1 ad 05 1a a1 5d 6a c7 bd d1 41 ee 16 aa 8e 8f 1c 06 0d 22 2b 90 08 ff 4a f8 ce 44 c9 cc 1c 91 47 b0 4c 28 6d cd 80 ec cf 41 36 54 27 17 db 57 c0 87 02 71 45 06 b5 76 ef 3d e2 70 ff 35 3d 09 1a 01 fe a9 61 a5 0a d5 f7 a9 60 4d cf 52 6a 96 16 07 39 62 21 84 35 5b ed f4 2e 9d 44 2a 9e 9a 6a 6f 8d 40 0f 09 0e 5a 82 cc fc ae e8 e2 88 5c e0 66 0b ea 34 46 4a 02 8a 82 f4 4e 29 ed 3e 25 30 10 21 ea 84 e6 28 58 ca 1b ba e2 66 3b de 41 47 43 e6 f4 c1 aa d3 1e ae ae 5e e9 39 e5 41 39 ec 80 ce f3 8c 11 a5 1f e0 de 3c d1 17 e9 55 0b 21 
	send_command("013820ff030101fb4d8fd5810ec8aa7cac5cb3ed0bed456287e3f7450b708ea908a26a4c212c73aead5f38a13847114588e44ddd4d200cd5e536dc50bfa9652514a99dc11d844623826420af7abd1ea911fae1ad051aa15d6ac7bdd141ee16aa8e8f1c060d222b9008ff4af8ce44c9cc1c9147b04c286dcd80eccf4136542717db57c08702714506b576ef3de270ff353d091a01fea961a50ad5f7a9604dcf526a96160739622184355bedf42e9d442a9e9a6a6f8d400f090e5a82ccfcaee8e2885ce0660bea34464a028a82f44e29ed3e25301021ea84e62858ca1bbae2663bde414743e6f4c1aad31eaeae5ee939e54139ec80cef38c11a51fe0de3cd117e9550b21")

#  17.759    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_SCAN_RESP_DATA (0xE)
#                              Command_Opcode = 0x2038 (8248)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  17.759    COM51 <- 04 0e 04 01 38 20 00 

if ('3' in testvar and testvar.get('3')):
	#  17.774    [HCI - COM51] LeSetExtScanRespData(Advertising_Handle=3,
	#                             Operation=2,
	#                             Fragment_Preference=1,
	#                             Scan_Response_Data='E\xd8\xb49\x10')
	#  17.774    COM51 -> 01 38 20 09 03 02 01 05 45 d8 b4 39 10 
	send_command("013820090302010545d8b43910")

#  17.790    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_SCAN_RESP_DATA (0xE)
#                              Command_Opcode = 0x2038 (8248)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  17.790    COM51 <- 04 0e 04 01 38 20 00 

if ('3' in testvar and testvar.get('3')):
	#  17.837    [HCI - COM51] LeSetExtAdvEnable(Enable=True,
	#                          Advertising_Handle=3,
	#                          Duration=0,
	#                          Max_Extended_Advertising_Events=0,
	#                          enable_param=[{'Duration': 0,
	#                          'Advertising_Handle': 3,
	#                          'Max_Extended_Advertising_Events': 0}])
	#  17.837    COM51 -> 01 39 20 06 01 01 03 00 00 00 
	send_command("01392006010103000000")

#  17.852    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  17.852    COM51 <- 04 0e 04 01 39 20 00 

#  17.899    [HCI - COM51] VsSetOpFlags(flags=2048,
#                     enable=False)
#  17.899    COM51 -> 01 f3 ff 05 00 08 00 00 00 
send_command("01f3ff050008000000")

#  17.915    [HCI - COM51] HCI event received: OPCODE_VS_SET_OP_FLAGS (0xE)
#                              Command_Opcode = 0xFFF3 (65523)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Status = 0x0 (0)
#  17.915    COM51 <- 04 0e 04 01 f3 ff 00 

#  17.930    [HCI - COM51] HCI event received: SUBEVT_LE_SCAN_REQ_RCVD_EVT (0x3E)
#                              Advertising_Handle = 0x2 (2)
#                              Event_Code = 0x3E (62)
#                              Parameter_Length = 0x9 (9)
#                              Scanner_Address = 0x5F123456 (1595028566)
#                              Scanner_Address_Type = 0x0 (0)
#                              Subevent_Code = 0x13 (19)
#  17.930    COM51 <- 04 3e 09 13 02 00 56 34 12 5f 00 00 

if ('4' in testvar and testvar.get('4')):
	#  17.977    [HCI - COM51] LeSetExtAdvParam(Advertising_Handle=4,
	#                         Advertising_Event_Properties=2,
	#                         Primary_Advertising_Interval_Min=128,
	#                         Primary_Advertising_Interval_Max=128,
	#                         Primary_Advertising_Channel_Map=7,
	#                         Own_Address_Type=0,
	#                         Peer_Address_Type=0,
	#                         Peer_Address='V4\x12_\x00\x00',
	#                         Advertising_Filter_Policy=0,
	#                         Advertising_Tx_Power=127,
	#                         Primary_Advertising_PHY=3,
	#                         Secondary_Advertising_Max_Skip=0,
	#                         Secondary_Advertising_PHY=3,
	#                         Advertising_SID=12,
	#                         Scan_Request_Notification_Enable=1)
	#  17.977    COM51 -> 01 36 20 19 04 02 00 80 00 00 80 00 00 07 00 00 56 34 12 5f 00 00 00 7f 03 00 03 0c 01 
	send_command("013620190402008000008000000700005634125f0000007f0300030c01")

#  17.993    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_PARAM (0xE)
#                              Command_Opcode = 0x2036 (8246)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x5 (5)
#                              Selected_Tx_Power = 0x9 (9)
#                              Status = 0x0 (0)
#  17.993    COM51 <- 04 0e 05 01 36 20 00 09 

if ('4' in testvar and testvar.get('4')):
	#  18.040    [HCI - COM51] LeSetExtScanRespData(Advertising_Handle=4,
	#                             Operation=1,
	#                             Fragment_Preference=1,
	#                             Scan_Response_Data='\xabR\x9da\r\xc5\xe0\xe1N\x8d\xc3\x93\xba\xdey*\xa2V\x17\xd4S\x9e\x91\xd7\xbbTV\xfd}w\xc3\x8f\x85v\xac\xb7ciLX.87\x90`]\xa4n\xffu66\xe6\xa0\xf6\x7f\xf9\x03\x90\x94\x02\x9b%p\xcc\x05W|\x13\x9a\x95\x07\x17\x8cVu\x06\xd8J\x89\x80\x13\xbd\xf3g\x9e\xfcZ[\'\xa6\xdc\xde\xfb\xd4\x18Xo\xe9 -\xdd\x1a.4\x8eS>\x82\xf1\xdd\xae(\xe4\xdd$\xd8\t\xd3\xbdaT\xe2\xd3D\xe2/\xef9\x96P\xbd\xecZsq\xf1z1S\x8dA\x86\xa6kV\xef\xa8\x11\xd1\xe44\xd8\xdb\x92\x1c\x87\xde?\xd3TK\xa6O\xac\xd0\xaa\x1aG\x95\xf7c<\x8d>I\xbbDX\xe2\xc8\xe8\x05\x8b\xdd\xbd\xff)\x81\x9c`\xbff0\x97\x88\x80\xb5\x90\x12/@\xf7F)\xcf\x81P\nb\xe5\x952\xab\xb36\rnl\'\n,\x03\xd6\xab{H|Q\xf0\xa8\x06\x11c\x1a\x1f\xd2-mM\xb7\tD\xd1"\x95\x11b\x1e\xc5\xf2')
	#  18.040    COM51 -> 01 38 20 ff 04 01 01 fb ab 52 9d 61 0d c5 e0 e1 4e 8d c3 93 ba de 79 2a a2 56 17 d4 53 9e 91 d7 bb 54 56 fd 7d 77 c3 8f 85 76 ac b7 63 69 4c 58 2e 38 37 90 60 5d a4 6e ff 75 36 36 e6 a0 f6 7f f9 03 90 94 02 9b 25 70 cc 05 57 7c 13 9a 95 07 17 8c 56 75 06 d8 4a 89 80 13 bd f3 67 9e fc 5a 5b 27 a6 dc de fb d4 18 58 6f e9 20 2d dd 1a 2e 34 8e 53 3e 82 f1 dd ae 28 e4 dd 24 d8 09 d3 bd 61 54 e2 d3 44 e2 2f ef 39 96 50 bd ec 5a 73 71 f1 7a 31 53 8d 41 86 a6 6b 56 ef a8 11 d1 e4 34 d8 db 92 1c 87 de 3f d3 54 4b a6 4f ac d0 aa 1a 47 95 f7 63 3c 8d 3e 49 bb 44 58 e2 c8 e8 05 8b dd bd ff 29 81 9c 60 bf 66 30 97 88 80 b5 90 12 2f 40 f7 46 29 cf 81 50 0a 62 e5 95 32 ab b3 36 0d 6e 6c 27 0a 2c 03 d6 ab 7b 48 7c 51 f0 a8 06 11 63 1a 1f d2 2d 6d 4d b7 09 44 d1 22 95 11 62 1e c5 f2 
	send_command("013820ff040101fbab529d610dc5e0e14e8dc393bade792aa25617d4539e91d7bb5456fd7d77c38f8576acb763694c582e383790605da46eff753636e6a0f67ff9039094029b2570cc05577c139a9507178c567506d84a898013bdf3679efc5a5b27a6dcdefbd418586fe9202ddd1a2e348e533e82f1ddae28e4dd24d809d3bd6154e2d344e22fef399650bdec5a7371f17a31538d4186a66b56efa811d1e434d8db921c87de3fd3544ba64facd0aa1a4795f7633c8d3e49bb4458e2c8e8058bddbdff29819c60bf6630978880b590122f40f74629cf81500a62e59532abb3360d6e6c270a2c03d6ab7b487c51f0a80611631a1fd22d6d4db70944d1229511621ec5f2")

#  18.180    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_SCAN_RESP_DATA (0xE)
#                              Command_Opcode = 0x2038 (8248)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  18.180    COM51 <- 04 0e 04 01 38 20 00 

if ('4' in testvar and testvar.get('4')):
	#  18.196    [HCI - COM51] LeSetExtScanRespData(Advertising_Handle=4,
	#                             Operation=2,
	#                             Fragment_Preference=1,
	#                             Scan_Response_Data='\xadd\xe50\x1d')
	#  18.196    COM51 -> 01 38 20 09 04 02 01 05 ad 64 e5 30 1d 
	send_command("0138200904020105ad64e5301d")

#  18.212    [HCI - COM51] HCI event received: SUBEVT_LE_SCAN_REQ_RCVD_EVT (0x3E)
#                              Advertising_Handle = 0x2 (2)
#                              Event_Code = 0x3E (62)
#                              Parameter_Length = 0x9 (9)
#                              Scanner_Address = 0x5F123456 (1595028566)
#                              Scanner_Address_Type = 0x0 (0)
#                              Subevent_Code = 0x13 (19)
#  18.212    COM51 <- 04 3e 09 13 02 00 56 34 12 5f 00 00 

#  18.212    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_SCAN_RESP_DATA (0xE)
#                              Command_Opcode = 0x2038 (8248)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  18.212    COM51 <- 04 0e 04 01 38 20 00 

if ('4' in testvar and testvar.get('4')):
	#  18.258    [HCI - COM51] LeSetExtAdvEnable(Enable=True,
	#                          Advertising_Handle=4,
	#                          Duration=0,
	#                          Max_Extended_Advertising_Events=0,
	#                          enable_param=[{'Duration': 0,
	#                          'Advertising_Handle': 4,
	#                          'Max_Extended_Advertising_Events': 0}])
	#  18.258    COM51 -> 01 39 20 06 01 01 04 00 00 00 
	send_command("01392006010104000000")

#  18.274    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  18.274    COM51 <- 04 0e 04 01 39 20 00 

#  18.290    [HCI - COM51] VsSetOpFlags(flags=2048,
#                     enable=False)
#  18.290    COM51 -> 01 f3 ff 05 00 08 00 00 00 
send_command("01f3ff050008000000")

#  18.305    [HCI - COM51] HCI event received: OPCODE_VS_SET_OP_FLAGS (0xE)
#                              Command_Opcode = 0xFFF3 (65523)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Status = 0x0 (0)
#  18.305    COM51 <- 04 0e 04 01 f3 ff 00 

if ('5' in testvar and testvar.get('5')):
	#  18.321    [HCI - COM51] LeSetExtAdvParam(Advertising_Handle=5,
	#                         Advertising_Event_Properties=2,
	#                         Primary_Advertising_Interval_Min=128,
	#                         Primary_Advertising_Interval_Max=128,
	#                         Primary_Advertising_Channel_Map=7,
	#                         Own_Address_Type=0,
	#                         Peer_Address_Type=0,
	#                         Peer_Address='V4\x12_\x00\x00',
	#                         Advertising_Filter_Policy=0,
	#                         Advertising_Tx_Power=127,
	#                         Primary_Advertising_PHY=1,
	#                         Secondary_Advertising_Max_Skip=0,
	#                         Secondary_Advertising_PHY=2,
	#                         Advertising_SID=11,
	#                         Scan_Request_Notification_Enable=1)
	#  18.321    COM51 -> 01 36 20 19 05 02 00 80 00 00 80 00 00 07 00 00 56 34 12 5f 00 00 00 7f 01 00 02 0b 01 
	send_command("013620190502008000008000000700005634125f0000007f0100020b01")

#  18.337    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_PARAM (0xE)
#                              Command_Opcode = 0x2036 (8246)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x5 (5)
#                              Selected_Tx_Power = 0x9 (9)
#                              Status = 0x0 (0)
#  18.337    COM51 <- 04 0e 05 01 36 20 00 09 

if ('5' in testvar and testvar.get('5')):
	#  18.399    [HCI - COM51] LeSetExtScanRespData(Advertising_Handle=5,
	#                             Operation=1,
	#                             Fragment_Preference=1,
	#                             Scan_Response_Data="Y\xd6\x91\x16\xee\x83\xbes\xaf\xae$\x91\xc8\x9c\xe8\xf5\x19-\x8d\xc18\xa2\xcf\x839p\xebQ\xc9\xc7\xfa\xf9\x8as@\x1e\xef\x8f\x8c\xb0\xfed5!\xe32\xc4\x92\xfeJ\xb7<cZ.\xe8?\x1f\xad\xc7p\xd9\x86\x0f-C\xf0\x83\x18\xa9\xefr0\x11'*{\xf5\x18\xd9d<\xd2\xcf\xeb\x15\x8f\xda}0\x7f\x9b\xd7\x02\x90\xaa5\xdd\xe0\xe2\x1e\x16\xc0\xe9/[\x9ag\xfb\x1d\xd3\x01b~q\x02V\xc8\xa7\xfe\x1d\xc4Z\x0f}\xc77V\xb6\x12\x99\x86\x1f\xdbe\x89\xd8Ia?\x1f\x08\x01\xe9]i\xf8A\x1b\xf8\x15\xf9\xd2\x96\x92\x87x\xb4AL \xd6\xf9\xe8\xb77P`[\xd206\x80\xe9\x91\xda5\xcc\x0f\xbb\xfa\x87\xce!\x07\xb6P\xc4\x17s\xec^T7@\xfa\xb0P\xae\xff!x$\x04\xce\x9a\x13/j\xe5\x85\xd1\xb4\xdde#gH+\xd2\x8c\x19\xa5\xe4R\x01O(@x\xc4\xdbq;*\x8f\xe2/\xebB\xd7\x9c\xf5$8fw\xdf\xa5Q+")
	#  18.399    COM51 -> 01 38 20 ff 05 01 01 fb 59 d6 91 16 ee 83 be 73 af ae 24 91 c8 9c e8 f5 19 2d 8d c1 38 a2 cf 83 39 70 eb 51 c9 c7 fa f9 8a 73 40 1e ef 8f 8c b0 fe 64 35 21 e3 32 c4 92 fe 4a b7 3c 63 5a 2e e8 3f 1f ad c7 70 d9 86 0f 2d 43 f0 83 18 a9 ef 72 30 11 27 2a 7b f5 18 d9 64 3c d2 cf eb 15 8f da 7d 30 7f 9b d7 02 90 aa 35 dd e0 e2 1e 16 c0 e9 2f 5b 9a 67 fb 1d d3 01 62 7e 71 02 56 c8 a7 fe 1d c4 5a 0f 7d c7 37 56 b6 12 99 86 1f db 65 89 d8 49 61 3f 1f 08 01 e9 5d 69 f8 41 1b f8 15 f9 d2 96 92 87 78 b4 41 4c 20 d6 f9 e8 b7 37 50 60 5b d2 30 36 80 e9 91 da 35 cc 0f bb fa 87 ce 21 07 b6 50 c4 17 73 ec 5e 54 37 40 fa b0 50 ae ff 21 78 24 04 ce 9a 13 2f 6a e5 85 d1 b4 dd 65 23 67 48 2b d2 8c 19 a5 e4 52 01 4f 28 40 78 c4 db 71 3b 2a 8f e2 2f eb 42 d7 9c f5 24 38 66 77 df a5 51 2b 
	send_command("013820ff050101fb59d69116ee83be73afae2491c89ce8f5192d8dc138a2cf833970eb51c9c7faf98a73401eef8f8cb0fe643521e332c492fe4ab73c635a2ee83f1fadc770d9860f2d43f08318a9ef723011272a7bf518d9643cd2cfeb158fda7d307f9bd70290aa35dde0e21e16c0e92f5b9a67fb1dd301627e710256c8a7fe1dc45a0f7dc73756b61299861fdb6589d849613f1f0801e95d69f8411bf815f9d296928778b4414c20d6f9e8b73750605bd2303680e991da35cc0fbbfa87ce2107b650c41773ec5e543740fab050aeff21782404ce9a132f6ae585d1b4dd652367482bd28c19a5e452014f284078c4db713b2a8fe22feb42d79cf524386677dfa5512b")

#  18.446    [HCI - COM51] HCI event received: SUBEVT_LE_SCAN_REQ_RCVD_EVT (0x3E)
#                              Advertising_Handle = 0x4 (4)
#                              Event_Code = 0x3E (62)
#                              Parameter_Length = 0x9 (9)
#                              Scanner_Address = 0x5F123456 (1595028566)
#                              Scanner_Address_Type = 0x0 (0)
#                              Subevent_Code = 0x13 (19)
#  18.446    COM51 <- 04 3e 09 13 04 00 56 34 12 5f 00 00 

#  18.602    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_SCAN_RESP_DATA (0xE)
#                              Command_Opcode = 0x2038 (8248)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  18.602    COM51 <- 04 0e 04 01 38 20 00 

if ('5' in testvar and testvar.get('5')):
	#  18.649    [HCI - COM51] LeSetExtScanRespData(Advertising_Handle=5,
	#                             Operation=2,
	#                             Fragment_Preference=1,
	#                             Scan_Response_Data='\x0f\x19h$\x8c')
	#  18.649    COM51 -> 01 38 20 09 05 02 01 05 0f 19 68 24 8c 
	send_command("01382009050201050f1968248c")

#  18.665    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_SCAN_RESP_DATA (0xE)
#                              Command_Opcode = 0x2038 (8248)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  18.665    COM51 <- 04 0e 04 01 38 20 00 

if ('5' in testvar and testvar.get('5')):
	#  18.711    [HCI - COM51] LeSetExtAdvEnable(Enable=True,
	#                          Advertising_Handle=5,
	#                          Duration=0,
	#                          Max_Extended_Advertising_Events=0,
	#                          enable_param=[{'Duration': 0,
	#                          'Advertising_Handle': 5,
	#                          'Max_Extended_Advertising_Events': 0}])
	#  18.711    COM51 -> 01 39 20 06 01 01 05 00 00 00 
	send_command("01392006010105000000")

#  18.727    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  18.727    COM51 <- 04 0e 04 01 39 20 00 

#  18.743    [HCI - COM51] HCI event received: SUBEVT_LE_SCAN_REQ_RCVD_EVT (0x3E)
#                              Advertising_Handle = 0x1 (1)
#                              Event_Code = 0x3E (62)
#                              Parameter_Length = 0x9 (9)
#                              Scanner_Address = 0x5F123456 (1595028566)
#                              Scanner_Address_Type = 0x0 (0)
#                              Subevent_Code = 0x13 (19)
#  18.743    COM51 <- 04 3e 09 13 01 00 56 34 12 5f 00 00 

#  18.774    [HCI - COM51] VsSetOpFlags(flags=2048,
#                     enable=False)
#  18.774    COM51 -> 01 f3 ff 05 00 08 00 00 00 
send_command("01f3ff050008000000")

#  18.790    [HCI - COM51] HCI event received: OPCODE_VS_SET_OP_FLAGS (0xE)
#                              Command_Opcode = 0xFFF3 (65523)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Status = 0x0 (0)
#  18.790    COM51 <- 04 0e 04 01 f3 ff 00 

if ('6' in testvar and testvar.get('6')):
	#  18.836    [HCI - COM51] LeSetExtAdvParam(Advertising_Handle=6,
	#                         Advertising_Event_Properties=2,
	#                         Primary_Advertising_Interval_Min=128,
	#                         Primary_Advertising_Interval_Max=128,
	#                         Primary_Advertising_Channel_Map=7,
	#                         Own_Address_Type=0,
	#                         Peer_Address_Type=0,
	#                         Peer_Address='V4\x12_\x00\x00',
	#                         Advertising_Filter_Policy=0,
	#                         Advertising_Tx_Power=127,
	#                         Primary_Advertising_PHY=1,
	#                         Secondary_Advertising_Max_Skip=0,
	#                         Secondary_Advertising_PHY=2,
	#                         Advertising_SID=10,
	#                         Scan_Request_Notification_Enable=1)
	#  18.836    COM51 -> 01 36 20 19 06 02 00 80 00 00 80 00 00 07 00 00 56 34 12 5f 00 00 00 7f 01 00 02 0a 01 
	send_command("013620190602008000008000000700005634125f0000007f0100020a01")

#  18.852    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_PARAM (0xE)
#                              Command_Opcode = 0x2036 (8246)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x5 (5)
#                              Selected_Tx_Power = 0x9 (9)
#                              Status = 0x0 (0)
#  18.852    COM51 <- 04 0e 05 01 36 20 00 09 

if ('6' in testvar and testvar.get('6')):
	#  18.899    [HCI - COM51] LeSetExtScanRespData(Advertising_Handle=6,
	#                             Operation=1,
	#                             Fragment_Preference=1,
	#                             Scan_Response_Data="x\x08r\xcd\x1c\x04]\x8e\x94Q\xb2.\xdd\xdc\xcd\x8cN\xdb\x93\xba\x8a\xe1\x16\xf0\xf0\xaag\xa2\xf1\xb0\x14\x18\x1dIk\xfc\xc8\x17yB\t\x80\xf6u\x95\xe08+;\xc6\xcd\x1eX\x99\xbb\x9b\xe9\x03\x16\xcap\xbb1r\xda>Y\x18\xbcHr\x15\xed\xd1\x85\xb2r\xabx\x06\xfec\x9d\xb5\xb2;:Ro\xe25Y}\x90\xee\xbeu\xcd\\\xbd\x06\xec\xfa\xe8\x19\xb6\xc2\xa4\x85\x0b\x16]\xe8\xe4\x03\x8cF\x17\xf4~\\\xe3\xe5\x93`\xb9\xc8#\xc2L\x0bUw{eG\xd1;\x93\x1c=\x07\xbf\xa6\xc3\xa0\xdej\xdc\x04\xf1\xd9H&\xd8i\xfd?\xf2\x16\x94\x86\x8a\xda\xf7OM\xd7\x90\xbd\x164\x17\x98U\xf3R\xb4\xc5G\x10\xa3\xf3\x88\x0e\xd7\xc7\x9a^\xf3\xb3;\xcd\t0\x84\xe1\xb7\x8e\x87\xd7\x7f\x92\xee\xdc\rH:\x01l\xea\xb0'\xaf\x03\xfb\xc9\xff\x8a\xd0t9:k\xd1\x95\xa8l\x86m\xd7N\x8c\x8c\x87X5\xc7\x80D\xd8\x1f\xb0\xd0\xbcGR\x81\xb4\xf32")
	#  18.899    COM51 -> 01 38 20 ff 06 01 01 fb 78 08 72 cd 1c 04 5d 8e 94 51 b2 2e dd dc cd 8c 4e db 93 ba 8a e1 16 f0 f0 aa 67 a2 f1 b0 14 18 1d 49 6b fc c8 17 79 42 09 80 f6 75 95 e0 38 2b 3b c6 cd 1e 58 99 bb 9b e9 03 16 ca 70 bb 31 72 da 3e 59 18 bc 48 72 15 ed d1 85 b2 72 ab 78 06 fe 63 9d b5 b2 3b 3a 52 6f e2 35 59 7d 90 ee be 75 cd 5c bd 06 ec fa e8 19 b6 c2 a4 85 0b 16 5d e8 e4 03 8c 46 17 f4 7e 5c e3 e5 93 60 b9 c8 23 c2 4c 0b 55 77 7b 65 47 d1 3b 93 1c 3d 07 bf a6 c3 a0 de 6a dc 04 f1 d9 48 26 d8 69 fd 3f f2 16 94 86 8a da f7 4f 4d d7 90 bd 16 34 17 98 55 f3 52 b4 c5 47 10 a3 f3 88 0e d7 c7 9a 5e f3 b3 3b cd 09 30 84 e1 b7 8e 87 d7 7f 92 ee dc 0d 48 3a 01 6c ea b0 27 af 03 fb c9 ff 8a d0 74 39 3a 6b d1 95 a8 6c 86 6d d7 4e 8c 8c 87 58 35 c7 80 44 d8 1f b0 d0 bc 47 52 81 b4 f3 32 
	send_command("013820ff060101fb780872cd1c045d8e9451b22edddccd8c4edb93ba8ae116f0f0aa67a2f1b014181d496bfcc81779420980f67595e0382b3bc6cd1e5899bb9be90316ca70bb3172da3e5918bc487215edd185b272ab7806fe639db5b23b3a526fe235597d90eebe75cd5cbd06ecfae819b6c2a4850b165de8e4038c4617f47e5ce3e59360b9c823c24c0b55777b6547d13b931c3d07bfa6c3a0de6adc04f1d94826d869fd3ff21694868adaf74f4dd790bd1634179855f352b4c54710a3f3880ed7c79a5ef3b33bcd093084e1b78e87d77f92eedc0d483a016ceab027af03fbc9ff8ad074393a6bd195a86c866dd74e8c8c875835c78044d81fb0d0bc475281b4f332")

#  19.039    [HCI - COM51] HCI event received: SUBEVT_LE_SCAN_REQ_RCVD_EVT (0x3E)
#                              Advertising_Handle = 0x1 (1)
#                              Event_Code = 0x3E (62)
#                              Parameter_Length = 0x9 (9)
#                              Scanner_Address = 0x4CE32843DAB0 (84538516822704)
#                              Scanner_Address_Type = 0x1 (1)
#                              Subevent_Code = 0x13 (19)
#  19.039    COM51 <- 04 3e 09 13 01 01 b0 da 43 28 e3 4c 

#  19.055    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_SCAN_RESP_DATA (0xE)
#                              Command_Opcode = 0x2038 (8248)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  19.055    COM51 <- 04 0e 04 01 38 20 00 

#  19.055    [HCI - COM51] HCI event received: SUBEVT_LE_SCAN_REQ_RCVD_EVT (0x3E)
#                              Advertising_Handle = 0x5 (5)
#                              Event_Code = 0x3E (62)
#                              Parameter_Length = 0x9 (9)
#                              Scanner_Address = 0x5F123456 (1595028566)
#                              Scanner_Address_Type = 0x0 (0)
#                              Subevent_Code = 0x13 (19)
#  19.055    COM51 <- 04 3e 09 13 05 00 56 34 12 5f 00 00 

if ('6' in testvar and testvar.get('6')):
	#  19.071    [HCI - COM51] LeSetExtScanRespData(Advertising_Handle=6,
	#                             Operation=2,
	#                             Fragment_Preference=1,
	#                             Scan_Response_Data='\xd9L\xe6\x13x')
	#  19.071    COM51 -> 01 38 20 09 06 02 01 05 d9 4c e6 13 78 
	send_command("0138200906020105d94ce61378")

	#  19.086    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_SCAN_RESP_DATA (0xE)
	#                              Command_Opcode = 0x2038 (8248)
	#                              Event_Code = 0xE (14)
	#                              Num_HCI_Command_Packets = 0x1 (1)
	#                              Parameter_Length = 0x4 (4)
	#                              Status = 0x0 (0)
	#  19.086    COM51 <- 04 0e 04 01 38 20 00 
	read_events(seconds=5)

if ('6' in testvar and testvar.get('6')):
	#  19.133    [HCI - COM51] LeSetExtAdvEnable(Enable=True,
	#                          Advertising_Handle=6,
	#                          Duration=0,
	#                          Max_Extended_Advertising_Events=0,
	#                          enable_param=[{'Duration': 0,
	#                          'Advertising_Handle': 6,
	#                          'Max_Extended_Advertising_Events': 0}])
	#  19.133    COM51 -> 01 39 20 06 01 01 06 00 00 00 
	send_command("01392006010106000000")

#  19.149    [HCI - COM51] HCI event received: SUBEVT_LE_SCAN_REQ_RCVD_EVT (0x3E)
#                              Advertising_Handle = 0x2 (2)
#                              Event_Code = 0x3E (62)
#                              Parameter_Length = 0x9 (9)
#                              Scanner_Address = 0x5F123456 (1595028566)
#                              Scanner_Address_Type = 0x0 (0)
#                              Subevent_Code = 0x13 (19)
#  19.149    COM51 <- 04 3e 09 13 02 00 56 34 12 5f 00 00 

#  19.149    [HCI - COM51] HCI event received: OPCODE_LE_SET_EXT_ADV_ENABLE (0xE)
#                              Command_Opcode = 0x2039 (8249)
#                              Event_Code = 0xE (14)
#                              Num_HCI_Command_Packets = 0x1 (1)
#                              Parameter_Length = 0x4 (4)
#                              Status = 0x0 (0)
#  19.149    COM51 <- 04 0e 04 01 39 20 00 

while(1):
  rcv_event()
  
