;Definitions for ___putchar UART
#if(CONSOLE_UART==1)
#define UART_BASE      40043000h ; UART 1
#elif(CONSOLE_UART==2)
#define UART_BASE      40044000h ; UART2
#else
#define UART_BASE      40042000h ; UART0 by default
#endif

#define UART_ST        (UART_BASE + 8h)
#define UART_SBUF      (UART_BASE + 1Ch)
#define BTLE_IRQ_STATUS      (40051084h)

; MAXQ HW locations
#define TBR     M0[0]
#define TBC     M0[1]
#define TBCN    M0[2]
#define TBV     M0[3]
#define INTOUT  M0[6]
#define WDCN    M0[7]
#define CKCN    M0[13]
#define IV      M1[24]
#define IPR0    M1[25]
#define IPR1    M1[26]
#define SWINT   M1[29]

; Set if the TMRB initialization may be placed in the SHCODE section
//#define TMRB_USE_SHCODE 1

; TMRB prescale setting
#define TMRB_PRESCALE 4

; SysTick period in ms.
#define WSF_MS_PER_TICK 1

; For testing: Set a fixed value for TMRB_FIXED_BDIV   
//#define TMRB_FIXED_SYS_CORE_CLOCK 96000000

; TBCN Flag that needs to be cleared 
; For testing: Set a fixed value for TMRB_FIXED_BDIV   
//#define TMRB_FIXED_BDIV (((TMRB_FIXED_SYS_CORE_CLOCK + 1) / (1 << (TMRB_PRESCALE << 1))) / ((1000 + (WSF_MS_PER_TICK >> 1)) / WSF_MS_PER_TICK))

; TBCN Flag that needs to be cleared 
#define TMRB_INT_FLAG_BITS 0x80 ; Clear overflow flag
//#define TMRB_INT_FLAG_BITS (0x80 + 0x40) ; Clear overflow and external capture flag

; Set to rely on autoreload functionality 
//#define USE_TMRB_AUTORELOAD 1

; Set to clear TBCN bits using atomic bit clearing instructions
; Requires AUTORELOAD to be enabled to funcion. 
//#define TMRB_INT_FLAG_ATOMIC_CLEAR 1

; Specify flags to clear when disabling/reenabling timer.
//#define TMRB_REINIT_FLAGS (0x04 + 0x02) ; ETB + TRB
#define TMRB_REINIT_FLAGS (0x02) ; ETB ; disable interrupts only
//#define TMRB_REINIT_FLAGS (0x04) ; TRB ; disable timer run control only

; Specify how many times to clear bits in TBCN.
; Suggested as a possible workaround for incorrect TMRB behavior.
#define TMRB_INT_FLAG_BIT_WRITE_COUNT 1

; default interrupt vector
PLACE_IV      macro x
    reti
    DS.B    (14)
    endm

    ;external functions
    IMPORT  _INT_IN_IRQHandler
    IMPORT  _TMRB_IRQHandler
    IMPORT  _BTLE_TX_DONE_IRQHandler
    IMPORT  _BTLE_RX_RCVD_IRQHandler
    IMPORT  _BTLE_RX_ENG_DET_IRQHandler
    IMPORT  _BTLE_SFD_DET_IRQHandler
    IMPORT  _BTLE_SFD_TO_IRQHandler
    IMPORT  _BTLE_RFFE_SPIM_IRQHandler
    IMPORT  _BTLE_GP_EVENT_IRQHandler
    IMPORT  _TMR0_IRQHandler
    IMPORT  _main
    IMPORT  _Catch_IRQHandler
    IMPORT  _get_ARM_flag
    IMPORT  _SystemCoreClockUpdate
    IMPORT  _SystemCoreClock
    IMPORT  _TmrBDiv

    IMPORT  UDATA0
    USECT   "UDATA0"

    CODE
    EVEN
    PSECT   "STARTUP"

    public entry
entry:
    jump startup_code

    public vector_base
vector_base:
    PLACE_IV 0; Interrupt 0 int_sdma_berr - int_in_ctrl[1]
    PLACE_IV 1; Interrupt 1 sdma_wdog
    ; PLACE_IV 2; Interrupt 2 sdma_wut, int_in_ctrl[0]
    jump _INT_IN_IRQHandler_wrapper
    DS.B    (8)

    ; PLACE_IV 3; Interrupt 3 sdma_timerb
    jump _TMRB_IRQHandler_wrapper
    DS.B    (8)

    ;PLACE_IV 4; Interrupt 4 RTC
    jump _INT_Catch_wrapper
    DS.B    (8)

    ;PLACE_IV 5; Interrupt 5 I2C0
    jump _INT_Catch_wrapper
    DS.B    (8)

    ;PLACE_IV 6; Interrupt 6 I2C1
    jump _INT_Catch_wrapper
    DS.B    (8)

    ;PLACE_IV 7; Interrupt 7 UART0
    jump _INT_Catch_wrapper
    DS.B    (8)

    ; PLACE_IV 8; Interrupt 8 UART1
    ; Used for LHCI interface
    jump _INT_Catch_wrapper
    DS.B    (8)

    ; PLACE_IV 9; Interrupt 9 SPI0
    jump _INT_Catch_wrapper
    DS.B    (8)

    ; PLACE_IV 10; Interrupt 10 SPI1
    jump _INT_Catch_wrapper
    DS.B    (8)

    ; PLACE_IV 11; Interrupt 11 GPIO0
    jump _INT_Catch_wrapper
    DS.B    (8)

    ; PLACE_IV 12; Interrupt 12 GPIO1
    jump _INT_Catch_wrapper
    DS.B    (8)

    ; PLACE_IV 13; Interrupt 13 GPIO2
    jump _INT_Catch_wrapper
    DS.B    (8)

    ; PLACE_IV 14; Interrupt 14 ADC
    jump _INT_Catch_wrapper
    DS.B    (8)

    ; PLACE_IV 15; Interrupt 15 Pulse Train
    jump _INT_Catch_wrapper
    DS.B    (8)

    ; Interrupts 16-31 controlled by MXC_SDMA->MUX_CTRL register
    ; Wrapper handlers below need to clear corresponding bit in SWINT

    ; PLACE_IV 16; Interrupt 16 MUX_CTRL0[7:0]
    jump _BTLE_IRQ_Handler
    DS.B    (8)

    ;PLACE_IV 17; Interrupt 17 MUX_CTRL0[15:8]
    jump _BTLE_IRQ_Handler
    DS.B    (8)

    ;PLACE_IV 18; Interrupt 18 MUX_CTRL0[23:16]
    jump _BTLE_IRQ_Handler
    DS.B    (8)

    ;PLACE_IV 19; Interrupt 19 MUX_CTRL0[31:24]
    jump _BTLE_IRQ_Handler
    DS.B    (8)

    ;PLACE_IV 20; Interrupt 20 MUX_CTRL1[7:0]
    jump _BTLE_IRQ_Handler
    DS.B    (8)

    PLACE_IV 21; Interrupt 21 MUX_CTRL1[15:8]
    PLACE_IV 22; Interrupt 22 MUX_CTRL1[23:16]
    PLACE_IV 23; Interrupt 23 MUX_CTRL1[31:24]
    PLACE_IV 24; Interrupt 24 MUX_CTRL2[7:0]
    PLACE_IV 25; Interrupt 25 MUX_CTRL2[15:8]
    PLACE_IV 26; Interrupt 26 MUX_CTRL2[23:16]
    PLACE_IV 27; Interrupt 27 MUX_CTRL2[31:24]
    PLACE_IV 28; Interrupt 28 MUX_CTRL3[7:0]
    PLACE_IV 29; Interrupt 29 MUX_CTRL3[15:8]
    PLACE_IV 30; Interrupt 30 MUX_CTRL3[23:16]
    PLACE_IV 31; Interrupt 31 MUX_CTRL3[31:24]

#if (TMRB_USE_SHCODE==1)
    PSECT   "SHCODE"
#else ; (TMRB_USE_SHCODE!=1)
    PSECT   "CODE"
#endif ; TMRB_USE_SHCODE
    public _TMRB_WSF_Init
_TMRB_WSF_Init:
    ; Setup TimerB to provide 1kHz ticks

    ; First, refresh source clock frequency (System Core Clock)
    call _SystemCoreClockUpdate

    ; Reset TimerB value.
    move  TBV, #0

#if (TMRB_FIXED_BDIV!=0)
    ; Calculate TMRB reset value at compile time.
    move A[7], #(TMRB_FIXED_BDIV)
#else ; (TMRB_FIXED_BDIV==0)

    ; Determine the TMRB source clock.
#if (TMRB_FIXED_SYS_CORE_CLOCK!=0)
    ; Calculate TMRB source clock frequency at compile time.
    move  A[7], #((TMRB_FIXED_SYS_CORE_CLOCK + 1) / (1 << (TMRB_PRESCALE << 1)))

#else ; (TMRB_FIXED_SYS_CORE_CLOCK==0)
    ; Calculate TimerB reload/capture value.
    ; Note: This value is placed into A[7] by call to SystemCoreClockUpdate
    ;move  DP[0], #_SystemCoreClock
    ;move  A[7], @DP[0].L

    ; Divide system core clock down to get TMRB source clock frequency.
#if (TMRB_PRESCALE==1)
    move  AP, #7
    add #2 ; Round to nearest integer.
    sr
    sr
#elif (TMRB_PRESCALE==2)
    move  AP, #7
    sr
    sr
    add #2 ; Round to nearest integer.
    sra2
#elif (TMRB_PRESCALE==3)
    move  AP, #7
    sr
    sr
    add #8 ; Round to nearest integer.
    sra4
#elif (TMRB_PRESCALE==4)
    move  AP, #7
    sr
    sr
    sra2
    add #8 ; Round to nearest integer.
    sra4
#elif (TMRB_PRESCALE==5)
    move  AP, #7
    sr
    sr
    sra4
    add #8 ; Round to nearest integer.
    sra4
#endif ; TMRB_PRESCALE
#endif ; TMRB_FIXED_SYS_CORE_CLOCK

    ; Load Systick Frequency
    ; 1000 (ms/sec) / (ms per SysTick) rounded to nearest integer
    move A[6], #((1000 + (WSF_MS_PER_TICK >> 1)) / WSF_MS_PER_TICK)

    ;; Divide TMRB clock frequency (clock/s) by SysTick frequency (tick/s).
    ; TMRB - A[7]
    ; SysTick freq - A[6]
    call ___uint32_div
#endif ; TMRB_FIXED_BDIV
    
    ; Load reset value of timer.
    move  TBR, A[7]

    ; Save reset value of timer for future reference/debugging
    move  DP[0], #_TmrBdiv
    move  @DP[0].L, A[7]

    ;; Load TMRB control register. Bit meanings are described below.
    ; bit[1]=ETB Enable Timer B Interrupt
    ; bit[2]=TRB Timer B Run Control
    ; bit[10:8]= clk/div 256=(2^(2*TMRB_PRESCALE)) 
    move  TBCN, #((TMRB_PRESCALE << 8) + 4 + 2)

    ret

#if (TMRB_USE_SHCODE==1)
    PSECT   "CODE"
#endif ; TMRB_USE_SHCODE

    public startup_code
startup_code:
    move    SP, #STACK_START
    move    IC.0, #0b           ; Clear IGE
    move    WDCN, #0

    ; Dont clear RAM if restarting
    call  _get_ARM_flag
    move  AP, #7
    jump  NZ, L$2

    ;clear BSS memory
    LINKIF  SIZEOF(UDATA0)
    move    DP[0], #SFB(UDATA0)
    move    LC[0], #(SIZEOF(UDATA0)) & 0xFFFF
    move    LC[1], #(SIZEOF(UDATA0)>>16) + 1
L$1:
    move    @DP[0].B++, #0
    djnz    LC[0], L$1
    djnz    LC[1], L$1
    ENDLINKIF

L$2:

    ; VVVVVVVVVV Expected Setup for Rowley Crossworks C Compiler VVVVVVVVVV

    ; Set AACCE
    move    AP, #7
    move    A[7], SC
    or      #0200h
    move    SC, A[7]

    ; Disable modulo updates
    move    APC, #0x00

    ; ^^^^^^^^^^ Expected Setup for Rowley Crossworks C Compiler ^^^^^^^^^^

    move  TBCN, #0h
    call _SystemCoreClockUpdate
    
    ; enable interrupts
    move    IV, #(vector_base/2)
    move    IPR0, #0x00000004   ; 0x55555555
    move    IPR1, #0x0          ; 0x15555555
    move    IC.0, #1b           ; Set IGE

    ;jump to C-main
    jump  _main

    public _TMRB_delayUs
_TMRB_delayUs:
    ret
    move  AP, #7
    ret   Z
    ; Setup the timer
    move  TBCN, #0
    move  TBV,  #0
    move  A[6], A[7]
    move  DP[0], #_SystemCoreClockM
    move  A[7], @DP[0].L
    SRA2                ; /4
    CALL  ___int32_mul
    move  TBR, A[7]
    move  TBCN, #104h ; bit[1]=IE bit[2] = run bit bit[8] = /4

    ; Wait for the expiration
_TMRB_delay$2:
    move  C, TBCN.7
    jump  NC, _TMRB_delay$2
    move  TBCN.7, #0
    ret

    public _TMRB_sleepUs
_TMRB_sleepUs:
    ret
    ; If global interrupts are disabled, we will never wake up.
    ; Delay instead.
    move C, IC.0
    jc _TMRB_delayUs

    ; Sleep US is in A[7]
    move  AP, #7
    ret   Z
    ; Setup the timer
    move  TBCN, #0
    move  TBV,  #0
    move  A[6], A[7]
    move  DP[0], #_SystemCoreClockM
    move  A[7], @DP[0].L
    SRA2                ; /4
    CALL  ___int32_mul
    move  TBR, A[7]
    move  TBCN, #106h ; bit[1]=IE bit[2] = run bit bit[8] = /4

    ; Enter Idle mode
    move    CKCN, #(1<<8)
    nop

    ret

    ; character is in a[7]
    public ___putchar
___putchar:
    push   A[7]

    ; wait for TXFIFO empty and TX not busy
___putchar$1:
    move  dp[0], #UART_ST
    move  A[7], @dp[0].L
    and   #040h
    jump  Z, ___putchar$1

    ;Put data into fifo
    move  dp[0], #UART_SBUF
    pop   @dp[0].b
    ret

    public ___uint8_load_c
___uint8_load_c:
    move    Acc, @DP[0].B
    ret

    public ___uint16_load_c
___uint16_load_c:
    move    Acc, @DP[0].W
    ret

    public ___uint32_load_c
___uint32_load_c:
    move    Acc, @DP[0].L
    ret

    public ___uint64_load_c
___uint64_load_c:
    push    APC
    move    APC, #7
    move    Acc, @DP[0].L++
    move    APC, #0
    move    Acc, @DP[0].L++
    pop     APC
    ret


    ; Send interrupt to ARM core
    public _interrupt_ARM
_interrupt_ARM:
    move INTOUT, #1
    ret

    public  _SDMA_EnterIdleMode
_SDMA_EnterIdleMode:
    move    CKCN, #(1<<8)
    ret

    public  _SDMA_EnterStopMode
_SDMA_EnterStopMode:
    move    CKCN.0, #1
    ret

_INT_Catch_wrapper:
    jump    _Catch_IRQHandler

_INT_IN_IRQHandler_wrapper:
    jump    _INT_IN_IRQHandler

_TMRB_IRQHandler_wrapper:   
    jump _TMRB_IRQHandler

    public _TMRB_IRQHandler_clear
_TMRB_IRQHandler_clear:
    ; bit[0]=CP/!RLB Capture/Reload Select
    ; bit[1]=ETB Enable Timer B Interrupt
    ; bit[2]=TRB Timer B Run Control
    ; bit[6]=EXFB External Timer B Trigger Flag
    ; bit[7]=TRB Timer B Overflow Flag
    ; bit[10:8]= clk/div 256=(2^(2*TMRB_PRESCALE)) 
#if (TMRB_INT_FLAG_ATOMIC_CLEAR==1)
#if (TMRB_INT_FLAG_BIT_WRITE_COUNT>1)
    move    LC[0], #(TMRB_INT_FLAG_BIT_WRITE_COUNT)
TMRB_IRQHandler_clear_tmrb_int_flag_bit_write_loop:
#endif ; (TMRB_INT_FLAG_BIT_WRITE_COUNT>1)
#if ((TMRB_INT_FLAG_BITS & 0x80) == 0x80)
    move    TBCN.7, #0
#endif ; (TMRB_INT_FLAG_BITS & 0x40)
#if ((TMRB_INT_FLAG_BITS & 0x40) == 0x40)
    move    TBCN.6, #0
#endif ; (TMRB_INT_FLAG_BITS & 0x40)
#if (TMRB_INT_FLAG_BIT_WRITE_COUNT>1)
    djnz    LC[0], TMRB_IRQHandler_clear_tmrb_int_flag_bit_write_loop
#endif ; (TMRB_INT_FLAG_BIT_WRITE_COUNT>1)
#if (USE_TMRB_AUTORELOAD==1)
#else ; (USE_TMRB_AUTORELOAD!=1)
#error ATOMIC_CLEAR depends on TMRB AUTORELOAD
#endif ; USE_TMRB_AUTORELOAD
#else ; (TMRB_INT_FLAG_ATOMIC_CLEAR!=1)
    move    AP, #7
    move    Acc, TBCN
#if (USE_TMRB_AUTORELOAD==1)
    and     #(0xffff - (TMRB_INT_FLAG_BITS))
#else ; (USE_TMRB_AUTORELOAD!=1)
    and     #(0xffff - (TMRB_INT_FLAG_BITS + TMRB_REINIT_FLAGS))
#endif ; USE_TMRB_AUTORELOAD
#if (TMRB_INT_FLAG_BIT_WRITE_COUNT>1)
    move    LC[0], #(TMRB_INT_FLAG_BIT_WRITE_COUNT)
TMRB_IRQHandler_clear_tmrb_int_flag_bit_write_loop:
#endif ; (TMRB_INT_FLAG_BIT_WRITE_COUNT>1)
    move    TBCN, Acc
#if (TMRB_INT_FLAG_BIT_WRITE_COUNT>1)
    djnz    LC[0], TMRB_IRQHandler_clear_tmrb_int_flag_bit_write_loop
#endif ; (TMRB_INT_FLAG_BIT_WRITE_COUNT>1)
#endif ; TMRB_INT_FLAG_ATOMIC_CLEAR

    ;; Reinitialize timer if selected.
#if (USE_TMRB_AUTORELOAD==0)
#if (TMRB_FIXED_BDIV!=0)
    move    A[7], #(TMRB_FIXED_BDIV)
#else ; (TMRB_FIXED_BDIV!=0)
    move    DP[0], #_TmrBdiv
    move    A[7], @DP[0].L
#endif ; TMRB_FIXED_BDIV
    move    TBR, A[7]        
    move    TBV, #0
#if (TMRB_INT_FLAG_ATOMIC_CLEAR==1)
    move    AP, #7
#endif ; TMRB_INT_FLAG_ATOMIC_CLEAR
    move    Acc, TBCN
    or      #(TMRB_REINIT_FLAGS)
    move    TBCN, Acc           ; Reenable Timer B.
#endif ; (USE_TMRB_AUTORELOAD==0)
    ret

    public _TMRB_IRQPoll_haveOverflow
_TMRB_IRQPoll_haveOverflow:
    move    C, TBCN.7
    jump    C, _TMRB_IRQPoll_haveOverflow_flag
    move    AP, #6
    move    Acc, TBV
    sub     TBR
    move    AP, #7
    jump    c, _TMRB_IRQPoll_haveOverflow_manual
_TMRB_IRQPoll_haveOverflow_false:
    move    A[7], #0
    jump    _TMRB_IRQPoll_haveOverflow_exit
_TMRB_IRQPoll_haveOverflow_flag:
    move    A[7], #1
    jump    _TMRB_IRQPoll_haveOverflow_exit
_TMRB_IRQPoll_haveOverflow_manual:
    move    A[7], #2
_TMRB_IRQPoll_haveOverflow_exit:
    ret

    ; Must clear corresponding bits in SWINT with ME14 A2
    ; This will handle all BTLE interrupts
_BTLE_IRQ_Handler:
    PUSH	PSF
    PUSH	AP
    PUSH	APC

    ; skip if no SWINT[20:16] bits set
    move  APC, #0
    push  A[7]
    move  A[7], SWINT
    mw2
    and   #0x1f
    jump  Z, _BTLE_IRQ_Handler$1
   
    PUSH	BP
    PUSH	OFFS
    PUSH	LC[0]
    PUSH	LC[1]
    PUSH	DP[0]
    PUSH	DP[1]
    PUSH	A[6]
    PUSH	A[5]
    PUSH	A[4]
    PUSH    A[3]
 
    ; Preserve read value of SWINT.
    move    A[3], A[7]

    ; clear flags in SWINT[20:16]
    move    A[7], SWINT
    bic     #(0x1f<<16)
    move    SWINT, A[7]

    ; clear BTLE flags
    move    DP[0], #BTLE_IRQ_STATUS
    move    NUL, @DP[0].W

    ; TX_DONE ?
    move  C, A[3].0
    jump  NC, _BTLE_IRQ_Handler_tx
    call    _BTLE_TX_DONE_IRQHandler
_BTLE_IRQ_Handler_tx:

    ; RX_RCVD ?
    move  C, A[3].1
    jump  NC, _BTLE_IRQ_Handler_rx
    call    _BTLE_RX_RCVD_IRQHandler
_BTLE_IRQ_Handler_rx:

    ; RX_SFD_DET ?
    move  C, A[3].2
    jump  NC, _BTLE_IRQ_Handler_rx_sfd_det
    call    _BTLE_SFD_DET_IRQHandler
_BTLE_IRQ_Handler_rx_sfd_det:

    ; RX_SFD_TO ?
    move  C, A[3].3
    jump  NC, _BTLE_IRQ_Handler_rx_sfd_to
    call    _BTLE_SFD_TO_IRQHandler
_BTLE_IRQ_Handler_rx_sfd_to:

    ; GP_EVENT ?
    move  C, A[3].4
    jump  NC, _BTLE_IRQ_Handler_gp_event
    call    _BTLE_GP_EVENT_IRQHandler
_BTLE_IRQ_Handler_gp_event:

    ; Disable IRQ
    move    IC.0, #0

_BTLE_IRQ_Handler_cleanup:
    call    _TMRB_IRQPoll

    ; Enable IRQ
    move    IC.0, #1

    POP A[3]
  	POP	A[4]
  	POP	A[5]
   	POP	A[6]
  	POP	DP[1]
  	POP	DP[0]
   	POP	LC[1]
   	POP	LC[0]
  	POP	OFFS
  	POP	BP

_BTLE_IRQ_Handler$1:

    POP A[7]
  	POP	APC
  	POP	AP
  	POP	PSF
  	RETI
end
