#! /usr/bin/env python
import sys

# Read the input arguments
if len(sys.argv) != 6:
	print "Usage log_merge.py hci_raw.log hci.log <com_target0> <com_target1> <output>"
	sys.exit(0)

hci_raw = sys.argv[1]
hci = sys.argv[2]
com_target0 = sys.argv[3]
com_target1 = sys.argv[4]
output = sys.argv[5]

# Open the files and read the lines
hci_raw_lines=[]
with open(hci_raw) as fp:
    for line in fp:
        if(com_target0 in line):
            hci_raw_lines.append(line)
        if(com_target1 in line):
            hci_raw_lines.append(line)

hci_lines=[]
com_match=0
temp_line=""
with open(hci) as fp:
    for line in fp:
        # New command or event after different COM
        if((com_target0 in line) and (com_match == 0)):
            temp_line=line
            com_match=1
        # New command or event after save COM
        elif((com_target0 in line) and (com_match == 1)):
            hci_lines.append(temp_line)
            temp_line=line
        # New command or event after different COM
        elif((com_target1 in line) and (com_match == 0)):
            temp_line=line
            com_match=1
        # New command or event after save COM
        elif((com_target1 in line) and (com_match == 1)):
            hci_lines.append(temp_line)
            temp_line=line
        # End of current COM
        elif(("COM" in line) and (com_match == 1)):
            hci_lines.append(temp_line)
            com_match=0
        # Continuation of current COM
        elif(com_match == 1):
            temp_line+="#"
            temp_line+=line

# Merge them together
output_text = ""
output_file = open(output, 'w')
cnt=0
for line in hci_lines:
    output_text+="# "+line
    output_text+="# "+hci_raw_lines[cnt]

    # Add the send_command if it's not an event
    if("->" in hci_raw_lines[cnt]):
        output_text+="send_command(\""
        output_text+=hci_raw_lines[cnt].split("->")[1].replace(" ", "").replace("\r\n", "")

        if(com_target0 in hci_raw_lines[cnt]):
            output_text+="\", port = port0)\r\n"
        else:
            output_text+="\", port = port1)\r\n"

    cnt = cnt+1
    output_text+="\r\n"

# Write the output
output_file.write(output_text)
output_file.close()
