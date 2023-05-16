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

HCI_CMD_RX_TEST = "3320"
HCI_CMD_RX_LEN = "03"

HCI_CMD_TX_TEST = "3420"
HCI_CMD_TX_LEN = "04"

HCI_CMD_RXV3_TEST = "4F20"
HCI_CMD_RXV3_LEN = "07"

HCI_CMD_END_TEST = "1F20"
HCI_CMD_END_LEN = "00"

HCI_CMD_RESET = "030C"
HCI_CMD_RESET_LEN = "00"

HCI_CMD_VEND_TEST = "01FC"
HCI_CMD_VEND_TEST_LEN = "02"

HCI_CMD_CFO_ENABLE = "D0FF"
HCI_CMD_CFO_ENABLE_LEN = "01"

HCI_CMD_GET_RSSI = "D1FF"
HCI_CMD_GET_RSSI_LEN = "00"

HCI_CMD_SET_AGC = "D2FF"
HCI_CMD_SET_AGC_LEN = "07"

HCI_CMD_RESTORE_AGC = "D3FF"
HCI_CMD_RESTORE_AGC_LEN = "00"

HCI_CMD_SET_TX_POWER_ENABLE = "D4FF"
HCI_CMD_SET_TX_POWER_ENABLE_LEN = "01"

HCI_CMD_TX_ENABLE = "D5FF"
HCI_CMD_TX_ENABLE_LEN = "00"

HCI_CMD_TX_DISABLE = "D6FF"
HCI_CMD_TX_DISABLE_LEN = "00"

HCI_CMD_SET_RFFE_REG9 = "D7FF"
HCI_CMD_SET_RFFE_REG9_LEN = "01"

HCI_CMD_PRBS9 = "DAFF"
HCI_CMD_PRBS9_LEN = "01"

def signal_handler(signal, frame):
    print("")
    port.close()
    sys.exit(0)

def send_cmd(packet_type, command, length, data, print_status=True):

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
        print('EVT < 0x%06X'%status, end="")
        for i in range(0,status_len):
            if(i>=(status_len-2) and status_len==6):
                print('%02X'%status_data[i+1], end="")
                print('%02X'%status_data[i], end="")
                break
            else:
                print('%02X'%status_data[i], end="")


        print()
        
        # convert the number of packets received to an integer
        if(command == HCI_CMD_END_TEST and status_len > 5):
            status_data = int(codecs.encode(status_data, 'hex_codec'),16)

            # Convert last four bytes to integer
            print("Received PKTS: "+str(int((status_data & 0xFF00)>>8)+int((status_data & 0xFF)<<8)))
            

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
    print("\nEnter the test you would like to run:")
    print("1. LE Enhanced Receiver Test")
    print("2. LE Enhanced Transmitter Test")
    print("3. LE End Test")
    print("4. HCI Reset")
    print("5. LE Receiver Test V3")
    # print("5. Vendor Test")
    # print("6. CFO Enable")
    # print("7. Get RSSI")
    # print("8. Set AGC")
    # print("9. Restore AGC")
    # print("10. Set TX Power")
    # print("11. Enable TX")
    # print("12. Disable TX")
    # print("13. Set RFFE REG9")
    # print("14. PRBS9")
    # print("0. Quit")
    user_input = input()

    if(int(user_input) == 1):
        print("Enter Channel number, int")
        data = "%0.2X"%int(input());
        print ("Enter PHY, int")
        print("    1 1M")
        print("    2 2M")
        print("    3 Coded S8")
        print("    4 Coded S2")
        data = data+"%0.2X"%int(input());
        # Use standard modulation index
        data = data+"00"

        # LE Receiver Test
        packet_type = HCI_CMD
        command = HCI_CMD_RX_TEST
        length = HCI_CMD_RX_LEN

    elif(int(user_input) == 2):
        print("Enter Channel number, int")
        data = ""
        data = "%0.2X"%int(input());
        print("Enter packet length, int")
        data = data+"%0.2X"%int(input());
        print("Enter bit pattern, int")
        print("    0 PRBS9")
        print("    1 11110000")
        print("    2 10101010")
        print("    3 PRBS15")
        print("    4 11111111")
        print("    5 00000000")
        print("    6 00001111")
        print("    7 01010101")
        data = data+"%0.2X"%int(input());
        print ("Enter PHY, int")
        print("    1 1M")
        print("    2 2M")
        print("    3 Coded S8")
        print("    4 Coded S2")
        data = data+"%0.2X"%int(input());

        # data = "172C01"
        # data = "002504"
        
        # LE Transmitter Test
        packet_type = HCI_CMD
        command = HCI_CMD_TX_TEST
        length = HCI_CMD_TX_LEN
        # TX_Channel | Length of Test Data | Packet Payload 
        # Receive status

    elif(int(user_input) == 3):
        print("Executing LE End Test")
        # LE Test End
        packet_type = HCI_CMD
        command = HCI_CMD_END_TEST
        length = HCI_CMD_END_LEN
        data = "00"
        # Receive Status and number of packets

    elif(int(user_input) == 4):
        print("Executing HCI Reset")
        # LE Test End
        packet_type = HCI_CMD
        command = HCI_CMD_RESET
        length = HCI_CMD_RESET_LEN
        data = "00"
        # Receive Status and number of packets

    elif(int(user_input) == 5 ):
        print("Executing RX Test V3")
        print("Enter Channel number, int")
        data = "%0.2X"%int(input());

        print ("Enter PHY, int")
        print("    1 1M")
        print("    2 2M")
        print("    3 Coded S8")
        print("    4 Coded S2")
        data = data+"%0.2X"%int(input());

        # Use standard modulation index
        data = data+"00"

        print ("Enter CTE Length, int")
        data = data+"%0.2X"%int(input());

        print ("Enter CTE Type, int")
        print("    0 AoA CTE")
        print("    1 AoD CTE w/ 1us slots")
        print("    2 AoD CTE w/ 2us slots")
        data = data+"%0.2X"%int(input());

        print ("Enter Slot Durations, int")
        print("    1 1us slots")
        print("    2 2us slots")
        data = data+"%0.2X"%int(input());

        print ("Enter Switching Pattern Len, int")
        switchLen = int(input())
        data = data+"%0.2X"%switchLen;

        print("Enter Antenna IDs, hex")
        data = data+input()

        packet_type = HCI_CMD
        command = HCI_CMD_RXV3_TEST
        length = "%0.2X"%(int(HCI_CMD_RXV3_LEN) + switchLen);

    # elif(int(user_input) == 5):
    #     print("Enter state, int")
    #     string_input = input()
    #     data = "%0.2X"%(0xFF & int(string_input)) + "%0.2X"%((0xFF00 & int(string_input)) >> 8)
    #     packet_type = HCI_CMD
    #     command = HCI_CMD_VEND_TEST
    #     length = HCI_CMD_VEND_TEST_LEN

    # elif(int(user_input) == 6):
    #     print("Enter state, int")
    #     print("    0 Disable")
    #     print("    1 Enable")
    #     string_input = input()
    #     data = "%0.2X"%(0xFF & int(string_input))
    #     packet_type = HCI_CMD
    #     command = HCI_CMD_CFO_ENABLE
    #     length = HCI_CMD_CFO_ENABLE_LEN

    # elif(int(user_input) == 7):
    #     print("Executing Get RSSI")
    #     packet_type = HCI_CMD
    #     command = HCI_CMD_GET_RSSI
    #     length = HCI_CMD_GET_RSSI_LEN

    # elif(int(user_input) == 8):
    #     print("Enter AGC Values, 7 bytes hex, LSB first")
    #     string_input = input()
    #     data = string_input
    #     packet_type = HCI_CMD
    #     command = HCI_CMD_SET_AGC
    #     length = HCI_CMD_SET_AGC_LEN

    # elif(int(user_input) == 9):
    #     print("Executing Restore AGC")
    #     packet_type = HCI_CMD
    #     command = HCI_CMD_RESTORE_AGC
    #     length = HCI_CMD_RESTORE_AGC_LEN

    # elif(int(user_input) == 10):
    #     print("Enter level, int 0-15")
    #     string_input = input()
    #     data = "%0.2X"%(0xFF & int(string_input))
    #     packet_type = HCI_CMD
    #     command = HCI_CMD_SET_TX_POWER_ENABLE
    #     length = HCI_CMD_SET_TX_POWER_ENABLE_LEN

    # elif(int(user_input) == 11):
    #     print("Executing TX Enable")
    #     packet_type = HCI_CMD
    #     command = HCI_CMD_TX_ENABLE
    #     length = HCI_CMD_TX_ENABLE_LEN

    # elif(int(user_input) == 12):
    #     print("Executing TX Disable")
    #     packet_type = HCI_CMD
    #     command = HCI_CMD_TX_DISABLE
    #     length = HCI_CMD_TX_DISABLE_LEN

    # elif(int(user_input) == 13):
    #     print("Enter value, 1 byte hex")
    #     string_input = input()
    #     data = string_input
    #     packet_type = HCI_CMD
    #     command = HCI_CMD_SET_RFFE_REG9
    #     length = HCI_CMD_SET_RFFE_REG9_LEN

    # elif(int(user_input) == 14):
    #     print("PRBS9, Enter channel value, 1 byte hex")
    #     string_input = input()
    #     data = string_input
    #     packet_type = HCI_CMD
    #     command = HCI_CMD_PRBS9
    #     length = HCI_CMD_PRBS9_LEN

    elif(int(user_input) == 0):
        port.close()
        exit(1)

    else:
        print("Unrecognized option")
        continue

    send_cmd(packet_type, command, length, data, True)
    # if(int(user_input) == 2):
    #     time.sleep(0.001)
    #     print("Executing LE End Test")
    #     # LE Test End
    #     packet_type = HCI_CMD
    #     command = HCI_CMD_END_TEST
    #     length = HCI_CMD_END_LEN
    #     data = "00"
    #     send_cmd(packet_type, command, length, data, True)
