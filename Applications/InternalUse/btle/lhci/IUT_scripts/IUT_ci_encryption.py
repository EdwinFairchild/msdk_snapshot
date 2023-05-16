#! /usr/bin/env python

from fastecdsa.curve import P256
from fastecdsa.point import Point

dev_addr = "5634128B0500"

filename="../common.py"
exec(open(filename).read())

print("IUT CI encryption test")
print("")

#   SetEventMask
send_command("01010c08FFFFFFFFFFFFFFFF")

#   SetEventMaskPage2
send_command("01630c08FFFFFFFFFFFFFFFF")

#   LeSetEventMask
send_command("01012008FFFFFFFFFFFFFFFF")
print("")

# Send the Read P256 Public Key command
print("Generate and verify P256 public key")
send_command("01252000")

# Read the public key, make sure that it's a valid point
key = rcv_event()[10:]
xcord=key[:64]
ycord=key[64:]

# Reverse the byte ordering
xcord_rev=""
ycord_rev=""
for i in range(1,64,2):
    xcord_rev+=key[63-i]
    xcord_rev+=key[63-i+1]
    ycord_rev+=key[127-i]
    ycord_rev+=key[127-i+1]

print("xcord = "+xcord_rev)
print("ycord = "+ycord_rev)
print("")

# Valid
# xs = 0xde2444bebc8d36e682edd27e0f271508617519b3221a8fa0b77cab3989da97c9
# ys = 0xc093ae7ff36e5380fc01a5aad1e66659702de80f53cec576b6350b243042a256

# # Invalid
# xs = 0xde2444bebc8d36e682edd27e0f271508617519b3221a8fa0b77cab3989da97c9
# ys = 0xc093ae7ff36e5380fc01a5aad1e66659702de80f53cec576b6350b243042a255

try:
    S = Point(int(xcord_rev,16), int(ycord_rev,16), curve=P256)
except ValueError:
    print("Point not on curve!")
    exit(1)

# Send the Generate DH Key command with valid public key
print("Generate and verify DH Key pair")
send_command("012620405e2ba4ab16bad56ebdba2327cee20a3a639a14eb07c8400692785e3f1817aa42430450974e25ecf3795ace8c6b43408c1628fe87f8d05539a270bf4e68495185")
status = rcv_event()[8:10]

# Make sure the status is 0
if(status != "00"):
    print("Failure to generate DH key "+status)
    exit(1)
print("")

# Send the Generate DH Key command with invalid public key
print("Verify remote public key")
status = send_command("012620401ea1f0f01faf1d9609592284f19e4c0047b58afd8615a69f559077b22faaa1904c55f33e429dad377356703a9ab85160472d1130e28e36765f89aff915b1214b")
status = status [8:10]

# Make sure the status is 2F
if(status == "00"):
    print("Failure to recognize invalid key pair "+status)
    # exit(1)
print("")

# Sample AES encryption
print("Test AES encryption")
ct = send_command("01172020bf01fb9d4ef3bc36d874f5394138684c1302f1e0dfcebdac7968574635241302")[14:]
print("ct = "+ct)
if(ct != "66C6C2278E3B8E053E7EA326521BAD99"):
    print("Cypher text mismatch")
    exit(1)

print("Test passed!")
exit(0)
