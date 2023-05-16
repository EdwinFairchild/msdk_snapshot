# UART used for HCI where used.
# 2 by default.
# WLP Variation board is UART0, MAP_A
# EvKit_V1 board is UART1, MAP_A
# BGA BCB board is UART0, MAP_B, pin1 is RX, pin2 is TX
HCI_UART?=2

# HCI_UART_MAP used by default.
# Unset by default.
HCI_UART_MAP?=MAP_A

# Run link layer using SDMA.
ENABLE_SDMA?=0
