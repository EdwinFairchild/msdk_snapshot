For the CMW tester to work...
1) LHCI_UART has been set to 0 for the CMW Tester to work on UART0
2) board.h in the ...Libraries/Boards/BCB/IncludeBCB folder has the CONSOLE_UART set to 1 to allow the tester to work 
3) "test_regs.h" has been added to the ...Libraries/Boards/BCB/Include folder.