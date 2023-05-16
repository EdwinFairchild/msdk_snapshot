
# SCP session build
 this tool creates scp sessions packets
 for max325xx, maxq1852 and max32653

# 1100:
	modification on hello reply and challenge including protection profile
	modification for display bin (wb instead of w)
# 1110
	modification on adding response length in generic response
	modification on chlg combination with pp (on rn0 and not rn15)
	this version contains:
		-support for IC400D
		-RCS commands management
# 1120
	.packet for binaries frames extension
# 1130
	tr_id incremented after each response
# 1140
	crypto_aes_cmac embedded to avoid dependance to libicrypto.a
# 1150
	CHUNK_SIZE increased to 3KB
# 1160 
	 decimal field in file name extended to 7 digits
     session size in write_mem_payload and verify_data_payload on 2 bytes..
# 1170 
	 verify_file added in script
# 1180 
	 mem_mapping_payload was not called by mem_mapping, but read_conf...
     CHUNK_SIZE=15KB
     2bugs in write_file: the chunk_len was incremented too early (before using chunk_len=0), so
     the 1st byte was always zero
     other bug: the internal loop was performed without checking i vs data_len, which leads to an error on end of file
# 1191 
	 bug 1601 & 1597: hello reply corrected
     disconnection_request & reply used with incorrect seq number (not incremented from previous command)
# 1201 
	 management of scripts
# 1211 
	 only one process_script function; fixed-random-number mode supported
# 1221 
	 bug #1623 (bad transaction-id) corrected
     bug #1622 incorrect blpk (and fak!) values
# 1231 
	 bug # ?? : transaction id (tr_id) was never incremented (and not checked by the bootloader)
# 1241 
	 file size normalized to multiple of 16
# 1251 
	 MAX_SCP_COMMAND increased (buffer overflow)
# 1261 
	 MAX_SCP COMMAND increased (buffer overflow: bug 1749); compilation warnings removed
# 1271 
	 issue on parameters process (config file loading fgets_correction not used every time)
# 1301 
	 bug #1757 correction; support s20 format
# 1302 
	 correction s20 nbbytes, seq is modulo 16
# 1312 
	 verbosity reduced; #1748 correction
# 1321 
	 bug #1767 correction: supported flash is now only limited to 1023MB (because of 32-bit int)
# 1331 
	 implements feature #1560: DEL-MEM and WRITE-MEM have same level of admin, so codes have been changed
# 2001 
	 implements SCP for flora (specific commands; compliant with SPEC22T02 rev 0.2.0); two modes: SCP__FLORA_RSA and SCP__FLORA_AES
# 2101 
	 implements modified write-timeout, following SPEC22T02 rev 0.2.1 modification
# 2111 
	 EXECUTE CODE command added for FLORA profile
# 2121 
	 chunk_size=4KB
# 2131 
	 pp=RSA forced when SCP__FLORA_RSA
# 2141 
	 write-crk len corrected (2 extrabytes)
# 2151 
	 errors returns codes from commands processing exploited
# 2201 
	 support of write-file-only and erase-file
# 2211 
	 support of flora 13-byte USN
# 2221 
	 control of RSA lengths to avoid bug #2031
# 2231 
	 hardcoded value of hello-reply configuration was wrong (0x01); it has been changed into jtag+rwk enabled: 0xC0
# 2241 
	 write-otp command format help was wrong; offset set as 1st parameter
# 2301 
	 #bug 2061: SCP bootloader for PCI-Linux shall use SHA256 and not SHA1
# 2311 
	 #bug 2064: timeout uart target issue
# 2321 
	 warning about secure keys handling
# 2331 
	 #bug2128
# 2341 
	 #bugs 2130, #2203, #feature 2212; timeout packet length+1 (write-timeout-payload) (and added ptr_address_offset for an optional use of 		address_offset; but not finished);
	CAUTION: write-mem-config is not implemented
# 2351 
	 SCP_FLORA_RSA full support
# 2361 
	 bug 2252 (incorrect file size for verify-file); also corrected on write-file and write-only
# 2371 
	 the chunk_size for write-data corresponds to the whole packet length and not the data payload only
# 3001 
	 implements secure protocol for MAXQ1852 (ECDSA256)
# 3101 
	 added transaction id field for 1852 (msp specs rev 0.93)
# 3201 
	 added verify-crk, activate-crk commands, generate-application-startup-certificate, verify-application-startup-certificate, renamed load crk, change response ASP into AGP (msp specs rev 1.0)
	renamed SCP_MAXQ1852 into MSP_MAXQ1852
# 3301 
	 MSP_MAXQ1852: SCOFFSET removed from  host packets (incorrect); size recomputed accurately (incl. ecdsa signature size) and handled for the signature; ipayload reduced by 3 as not including the header
# 3401 
	 SCOFFSET removed from target packets (misunderstanding of the spec); signature is include lsB first in payload for MSP
# 3501 
	 xq,yq is lsB in load-crk and in verify-crk (it is preferred to reverse it within the application and to keep it msB in the script file)
# 3511 
	 correction of bug #2982; ecdsafile and rsa parameters renamed in ecdsa_file and rsa_file; rsamod, public_exponent and private_exponent retrieved from the possible parameters list (as the rsa key is loaded now via the rsa_file parameter)
# 3521 
	 support of load-file hex for maxq1852
# 3701 
	 support for hsm
# 3711 
	 support for (lighthouse) angela SCP for test keys.
# 3721 
	 rsa file format frozen to modulus|privexp|pubexp (consistent with crk_sign 1.2.1 and ca_sign 1.2.9); ecdsa signature storage in payload corrected
# 3731 
	 read crk file was not supporting ecdsa format (angela ecdsa)
# 3741 
	 ANGELA ECDSA PROFILE inserted in the HELLO-REQ
# 3751 
	 data len in write-crk ecdsa angela incorrect
# 3761 
	 generic response for angela ecdsa includes an incorrect aes cmac
# 3771 
	 MAXQ1852 load-file improved
# 3781 
	 #3747: rewrite-crk added for angela and lhassa
# 3791 
	 #3780: vbus detect support
# 3.7.10.1 
	printf(hello) removed in ecdsa_sign
# 3.7.11 
	#3946: rewrite-crk including previous crk information					 
# 3.7.12 
	DR 31032015 1704 UCL DYNAMIC LOAD CONFLICT IN CASE OF MAX32590

# 3.7.13 
	#4041 max packet size shall be default value, 
    #4023 missing file shall stop execution
    #4287 correction made in 2.3.7.1 for write-file applied to verify-file
# 3.7.14 
	#4186 blank lines support in .ini
    #4288 extended-address management was wrong for maxq1852
    #4306 parameter name string in parameter value
# 3.7.15 
	support of tiroc protocol
# 3.7.16 
	TIROC - synchro pattern fixed to 2 Bytes
    TIROC - digital signature added after payload
# 3.7.17 
	#4393: incorrect extended address management on 1852 hex file
# 3.7.18 
	echo & dump commands added (BV request)
# 3.7.19 
	#4427 BV "==session_mode" missing in test for SCP_LITE_ECDSA
# 3.7.20 
	#4428 BV, Implement Dump function in erase-data and verify-file
# 3.8.0: 
	support of MAX32653 (ROM code named PAOLA): SCP_PAOLA and SCP_PP_PAOLA: protocol equivalent to SCP_ANGELA_ECDSA except the RSA instead of the ECDSA; support only RSA 4096
# 3.8.1: 
	 support PAOLA_4096 and PAOLA_2048
# 3.8.2: 
	 hello request byte changed from 0x03 to 0x02 (because RSA)
# 3.8.3: 
	 rewrite-crk paola mode corrected for crk1 and crk2 not having the same length
# 3.8.4: 
	 kill-chip2 is previous kill-chip with USN as argument. It's different command than kill-chip.
# 3.9.0: 
	 Reorganize Code and update HSM functions
# 3.10.0
	Fix issue length S19 and del_mem in scp
	Add OS differentiation in lib handling

# 3.10.1
	Fix hsm slot number parameter parsing in config file
	Improve HSM login sequence
	Improve build parameters

#3.10.2
	Fix parsing of config file last line 
	
	
	
	

