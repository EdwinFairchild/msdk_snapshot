#! /usr/bin/env python

from __future__ import print_function
import serial, array, codecs
import sys
import signal
import time

# HCI definitions
HCI_CMD = "01"
HCI_ACL = "02"
HCI_EVT = "04"

HCI_CMD_RESET = "030C"
HCI_CMD_RESET_LEN = "00"

HCI_CMD_READ_PHY = "3020"
HCI_CMD_READ_PHY_LEN = "02"

HCI_CMD_SET_DEFAULT_PHY = "3120"
HCI_CMD_SET_DEFAULT_PHY_LEN = "03"

HCI_CMD_SET_PHY = "3220"
HCI_CMD_SET_PHY_LEN = "07"

HCI_CMD_LOCAL_VER_INFO = "0110"
HCI_CMD_LOCAL_VER_INFO_LEN = "00"

HCI_CMD_LOCAL_SUP_CMD = "0210"
HCI_CMD_LOCAL_SUP_CMD_LEN = "00"

HCI_CMD_LE_LOCAL_SUP_FEAT = "0320"
HCI_CMD_LE_LOCAL_SUP_FEAT_LEN = "00"

HCI_CMD_LE_ADD_DEV_RES_LIST = "2720"
HCI_CMD_LE_ADD_DEV_RES_LIST_LEN = "27"

HCI_CMD_LE_READ_LOCAL_P256 = "2520"
HCI_CMD_LE_READ_LOCAL_P256_LEN = "00"

HCI_CMD_LE_GEN_DHKEY = "2620"
HCI_CMD_LE_GEN_DHKEY_LEN = "40"

HCI_CMD_LE_READ_TX_POWER = "4B20"
HCI_CMD_LE_READ_TX_POWER_LEN = "00"

HCI_CMD_LE_READ_RF_PATH_COMP = "4C20"
HCI_CMD_LE_READ_RF_PATH_COMP_LEN = "00"

HCI_CMD_LE_WRITE_RF_PATH_COMP = "4D20"
HCI_CMD_LE_WRITE_RF_PATH_COMP_LEN = "04"

HCI_CMD_LE_READ_DEF_DATA_LEN = "2320"
HCI_CMD_LE_READ_DEF_DATA_LEN_LEN = "00"

HCI_CMD_LE_ENCRYPT = "1720"
HCI_CMD_LE_ENCRYPT_LEN = "20"

HCI_CMD_SET_TX_POWER = "D4FF"
HCI_CMD_SET_TX_POWER_LEN = "01"

def signal_handler(signal, frame):
    print("")
    port.close()
    sys.exit(0)

def send_cmd(packet_type="", command="", length=0, data="", print_status=True, event=False):

    if(event == False):
        # Send the command and data
        print("CMD > 0x"+packet_type+command+length, end="")
        port.write(bytearray.fromhex(packet_type))
        port.write(bytearray.fromhex(command))
        port.write(bytearray.fromhex(length))
        
        if(int(length)):
            print (data, end="")
            port.write(bytearray.fromhex(data))
        print()

    # Receive the status
    status=port.read(size=3)
    
    status_len = status[2]
    status_data = port.read(size=status_len)
    status=int(codecs.encode(status, 'hex_codec'),16)

    if(print_status):
        print('EVT < ')
        print('      Status: 0x%06X'%status)
        print('      Data  : 0x', end="")
        for i in range(0,status_len):
                print('%02X'%status_data[i], end="")


        print()
            

# Setup the Signal handler to catch the ctrl-C
signal.signal(signal.SIGINT, signal_handler)

if(len(sys.argv) == 1):
    print("Enter serial port: \"COM20\" or \"/dev/ttyUSB0\"")
    COM_PORT = input()
else:
    COM_PORT = sys.argv[1]

print("Using "+COM_PORT+" for HCI serial")

# Open serial port
port = serial.Serial(
    port=str(COM_PORT),
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
)

port.isOpen()

while(1):
    print("\nEnter the command:")
    print("1  HCI Reset")
    print("2  LE Read PHY")
    print("3  LE Set Default PHY")
    print("4  LE Set PHY")
    print("5  Test LE Set PHY")
    print("6  Read Local Version Info")
    print("7  Read Local Supported Commands")
    print("8  Read Local Supported Features")
    print("9  Add Device to Resolving List")
    print("10  Read Local P256 public key")
    print("11  Generate DH key")
    print("12  Read TX Power")
    print("13  Read RF Path Compensation")
    print("14  Write RF Path Compensation")
    print("15  Read Default Data Length")
    print("16  LE Encrypt")
    print("17  Set TX Power")
    print("99 Read event")
    print("0. Quit")
    user_input = int(input())

    if(user_input == 1):
        print("Executing HCI Reset")
        packet_type = HCI_CMD
        command = HCI_CMD_RESET
        length = HCI_CMD_RESET_LEN
        data = "00"

    elif(user_input == 2):
        print("LE Read PHY")
        packet_type = HCI_CMD
        command = HCI_CMD_READ_PHY
        length = HCI_CMD_READ_PHY_LEN
        print("Enter handle, 2 bytes hex")
        data = input()

    elif(user_input == 3):
        print("LE Set Default PHY")
        packet_type = HCI_CMD
        command = HCI_CMD_SET_DEFAULT_PHY
        length = HCI_CMD_SET_DEFAULT_PHY_LEN
        print("Enter ALL_PHYS, 1 byte hex")
        data = input()
        print("Enter TX_PHYS, 1 byte hex")
        data = data+input()
        print("Enter RX_PHYS, 1 byte hex")
        data = data+input()

    elif(user_input == 4):
        print("LE Set PHY")
        packet_type = HCI_CMD
        command = HCI_CMD_SET_PHY
        length = HCI_CMD_SET_PHY_LEN
        print("Enter handle, 2 bytes hex")
        data = input()
        print("Enter ALL_PHYS, 1 byte hex")
        data = data+input()
        print("Enter TX_PHYS, 1 byte hex")
        data = data+input()
        print("Enter RX_PHYS, 1 byte hex")
        data = data+input()
        print("Enter PHY_options, 2 byte hex")
        data = data+input()

    elif(user_input == 5):
        print("Testing default LE Set PHY")
        packet_type = HCI_CMD
        command = HCI_CMD_SET_PHY
        length = HCI_CMD_SET_PHY_LEN
        data = "0000"
        data = data+"00"
        data = data+"01"
        data = data+"01"
        data = data+"0000"

    elif(user_input == 6):
        print("Executing Read Local Version Info")
        packet_type = HCI_CMD
        command = HCI_CMD_LOCAL_VER_INFO
        length = HCI_CMD_LOCAL_VER_INFO_LEN
        data = "00"

    elif(user_input == 7):
        print("Executing Read Local Supported Commands")
        packet_type = HCI_CMD
        command = HCI_CMD_LOCAL_SUP_CMD
        length = HCI_CMD_LOCAL_SUP_CMD_LEN
        data = "00"

    elif(user_input == 8):
        print("Executing LE Read Local Supported Features")
        packet_type = HCI_CMD
        command = HCI_CMD_LE_LOCAL_SUP_FEAT
        length = HCI_CMD_LE_LOCAL_SUP_FEAT_LEN
        data = "00"

    elif(user_input == 9):
        print("Executing Add device to resoliving list")
        packet_type = HCI_CMD
        command = HCI_CMD_LE_ADD_DEV_RES_LIST
        length = HCI_CMD_LE_ADD_DEV_RES_LIST_LEN
        data = "00" # Public identity Address
        data+= "001122334455" # Peer Identity Address
        data+= "00112233445566778899AABBCCDDEEFF" # Peer IRK
        data+= "00112233445566778899AABBCCDDEEFF" # Local IRK

    elif(user_input == 10):
        print("Read Local P256 public key")
        packet_type = HCI_CMD
        command = HCI_CMD_LE_READ_LOCAL_P256
        length = HCI_CMD_LE_READ_LOCAL_P256_LEN
        data="00"

    elif(user_input == 11):
        print("Generate DH key")
        packet_type = HCI_CMD
        command = HCI_CMD_LE_GEN_DHKEY
        length = HCI_CMD_LE_GEN_DHKEY_LEN
        data = "1ea1f0f01faf1d9609592284f19e4c0047b58afd8615a69f559077b22faaa190"
        data+= "4c55f33e429dad377356703a9ab85160472d1130e28e36765f89aff915b1214a"

    elif(user_input == 12):
        print("Generate DH key")
        packet_type = HCI_CMD
        command = HCI_CMD_LE_READ_TX_POWER
        length = HCI_CMD_LE_READ_TX_POWER_LEN
        data = "00"

    elif(user_input == 13):
        print("Generate DH key")
        packet_type = HCI_CMD
        command = HCI_CMD_LE_READ_RF_PATH_COMP
        length = HCI_CMD_LE_READ_RF_PATH_COMP_LEN
        data = "00"

    elif(user_input == 14):
        print("Generate DH key")
        packet_type = HCI_CMD
        command = HCI_CMD_LE_WRITE_RF_PATH_COMP
        length = HCI_CMD_LE_WRITE_RF_PATH_COMP_LEN
        data = "00010001"

    elif(user_input == 15):
        print("Read Default Data Length")
        packet_type = HCI_CMD
        command = HCI_CMD_LE_READ_DEF_DATA_LEN
        length = HCI_CMD_LE_READ_DEF_DATA_LEN_LEN
        data = "00"

    elif(user_input == 16):
        print("LE Encrypt")
        packet_type = HCI_CMD
        command = HCI_CMD_LE_ENCRYPT
        length = HCI_CMD_LE_ENCRYPT_LEN
        data = "bf01fb9d4ef3bc36d874f5394138684c1302f1e0dfcebdac7968574635241302"

    elif(int(user_input) == 17):
        print("Enter level, int 0-15")
        string_input = input()
        data = "%0.2X"%(0xFF & int(string_input))
        packet_type = HCI_CMD
        command = HCI_CMD_SET_TX_POWER
        command = HCI_CMD_SET_TX_POWER
        length = HCI_CMD_SET_TX_POWER_LEN
        length = HCI_CMD_SET_TX_POWER_LEN

    elif(user_input == 99):
        print("Reading event")
        send_cmd(event = True)
        continue

    elif(user_input == 0):
        port.close()
        exit(1)

    else:
        print("Unrecognized option")
        continue

    send_cmd(packet_type, command, length, data, True)
