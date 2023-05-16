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
HCI_CMD_RX_TEST_LEN = "03"

HCI_CMD_TX_TEST = "3420"
HCI_CMD_TX_TEST_LEN = "04"

HCI_CMD_END_TEST = "1F20"
HCI_CMD_END_TEST_LEN = "00"

HCI_CMD_RESET = "030C"
HCI_CMD_RESET_LEN = "00"

def twos_complement(hexstr,bits):
    value = hexstr
    if value & (1 << (bits-1)):
        value -= 1 << bits
    return value

def signal_handler(signal, frame):
    print("")
    port_rx.close()
    port_tx.close()
    sys.exit(0)

def send_cmd(port, packet_type, command, length, data="00"):

    # Send the command and data
    # print("CMD > 0x"+packet_type+command+length, end="")
    port.write(bytearray.fromhex(packet_type))
    port.write(bytearray.fromhex(command))
    port.write(bytearray.fromhex(length))
    
    if(int(length)):
        # print (data, end="")
        port.write(bytearray.fromhex(data))
    # print()

    # Receive the status
    status=port.read(size=3)
    
    status_len = status[2]
    status_data = port.read(size=status_len)
    status=int(codecs.encode(status, 'hex_codec'),16)

    # print('EVT < 0x%06X'%status, end="")
    # for i in range(0,status_len):
    #     if(i>=(status_len-2) and status_len==6):
    #         print('%02X'%status_data[i+1], end="")
    #         print('%02X'%status_data[i], end="")
    #         break
    #     else:
    #         print('%02X'%status_data[i], end="")


    # print()
        
    # convert the number of packets received to an integer
    if(command == HCI_CMD_END_TEST and status_len > 5):
        status_data = int(codecs.encode(status_data, 'hex_codec'),16)

        if(status_len > 9):
            # Convert first 4 bytes to integer for RSSI
            print(str(twos_complement(int(((status_data & 0xFF000000)>>24) | ((status_data & 0xFF0000)>>8) | ((status_data & 0xFF00)<<8) | ((status_data & 0xFF)<<24)), 32)), end=", ")

            # Convert last 2 bytes to integer for # completed packets
            print(str(int((status_data & 0xFF0000000000)>>(8+32))+int((status_data & 0xFF00000000)>>24)), end="")
        else :
            # Convert last 2 bytes to integer
            print(str(int((status_data & 0xFF00)>>8)+int((status_data & 0xFF)<<8)), end="")
            

# Setup the Signal handler to catch the ctrl-C
signal.signal(signal.SIGINT, signal_handler)

if(len(sys.argv) != 4):
    print("ch_sweep.py <serial0> <serial1> chmap phy length packets")
    sys.exit(1)
else:
    COM_PORT_RX = sys.argv[1]
    COM_PORT_TX = sys.argv[2]
    chmap = sys.argv[3]
    # phy = sys.argv[4]
    # length = sys.argv[4]

print("# Using "+COM_PORT_RX+" for RX")
print("# Using "+COM_PORT_TX+" for TX")
print(f"# chmap = {chmap}")

# Print the header row
print("ch, rssi, pkts_rx")

# convert strings to ints
chmap = int(chmap,0)

# Open serial port
port_rx = serial.Serial(
    port=str(COM_PORT_RX),
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
)
port_tx = serial.Serial(
    port=str(COM_PORT_TX),
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
)

port_rx.isOpen()
port_tx.isOpen()

# Reset both devices
send_cmd(port_rx, HCI_CMD, HCI_CMD_RESET, HCI_CMD_RESET_LEN)
send_cmd(port_tx, HCI_CMD, HCI_CMD_RESET, HCI_CMD_RESET_LEN)


# Sweep the channels
while(1):
    for channel in range(40):
        if ((0x1 << channel) & (chmap)):
            print(f"{channel}", end=", ")
            
            # Start Receiver
            data = "%0.2X"%channel;
            data+= "%0.2X"%1; # for 1M PHY, TODO: sweep the PHYs
            data+= "00" # Uset standard modulation index
            send_cmd(port_rx, HCI_CMD, HCI_CMD_RX_TEST, HCI_CMD_RX_TEST_LEN, data)

            # Start Transmitter
            data = "%0.2X"%channel;
            data+= "%0.2X"%48; # default the lenbth to 37
            data+= "%0.2X"%0;  # default to PRBS9
            data+= "%0.2X"%1; # for 1M PHY, TODO: sweep the PHYs
            send_cmd(port_tx, HCI_CMD, HCI_CMD_TX_TEST, HCI_CMD_TX_TEST_LEN, data)

            # Sleep while the transmitter is transmitting, 1 second for 1000 packets with 37 byte payload
            time.sleep(2)

            # Test end for receiver
            send_cmd(port_rx, HCI_CMD, HCI_CMD_END_TEST, HCI_CMD_END_TEST_LEN)

            print()
