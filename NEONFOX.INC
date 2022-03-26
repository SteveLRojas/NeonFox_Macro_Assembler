;                     /\         /\__
;                   // \       (  0 )_____/\            __
;                  // \ \     (vv          o|          /^v\
;                //    \ \   (vvvv  ___-----^        /^^/\vv\
;              //  /     \ \ |vvvvv/               /^^/    \v\
;             //  /       (\\/vvvv/              /^^/       \v\
;            //  /  /  \ (  /vvvv/              /^^/---(     \v\
;           //  /  /    \( /vvvv/----(O        /^^/           \v\
;          //  /  /  \  (/vvvv/               /^^/             \v|
;        //  /  /    \( vvvv/                /^^/               ||
;       //  /  /    (  vvvv/                 |^^|              //
;      //  / /    (  |vvvv|                  /^^/            //
;     //  / /   (    \vvvvv\          )-----/^^/           //
;    // / / (          \vvvvv\            /^^^/          //
;   /// /(               \vvvvv\        /^^^^/          //
;  ///(              )-----\vvvvv\    /^^^^/-----(      \\
; //(                        \vvvvv\/^^^^/               \\
;/(                            \vvvv^^^/                 //
;                                \vv^/         /        //
;                                             /<______//
;                                            <<<------/
;                                             \<
;                                              \
;**************************************************
;* NEONFOX.INC        HEADER FILE                 *
;* Copyright (C) 2021 Esteban Looser-Rojas.       *
;* Contains register defines for the NeonFox CPU  *
;* and address defines for platform peripherals.  *
;* Addresses are for the NeonFox test platform.   *
;**************************************************

; Address map for IO space:
; 0x0000 to 0x0FFF	unused
; 0x1000 to 0xFFDF	unused
; 0xFFE0 to 0xFFE7	XGRI (read write)
; 0xFFE8 to 0xFFED	unused
; 0xFFEE to 0xFFEF	interrupt controller (read write)
; 0xFFF0 to 0xFFF3	timer module
; 0xFFF4 to 0xFFF5 keyboard module (read write)
; 0xFFF6 to 0xFFF7 video mode register (write only)
; 0xFFF8 to 0xFFFB memory subsystem control registers	(write only)
; 0xFFFC to 0xFFFD	HEX display registers	(write only)
; 0xFFFE to 0xFFFF	RS-232 module	(read write)

; MSC Address map
; 0x0 program space control register
; 	bit0	reset bit (LSB)
; 	bit1	reserved
; 	bit2	prefetch bit
; 	bit3	control enable bit
; 0x1 program space page register
; 0x2 data space control register
; 	bit0	reset bit (LSB)
; 	bit1	flush bit
; 	bit2	prefetch bit
; 	bit3	control enable bit
; 0x3 data space page register

; RS-232 module address map
; 0 data register
; 1 status register
;		bit 0: TX overwrite (LSB)
;		bit 1: RX overwrite
;		bit 2: TX ready
;		bit 3: RX ready
;		bit 4: TX queue empty
;		bit 5: RX queue full

; keyboard module address map
; 0 data register
; 1 status register
;		bit 0: TX overwrite (LSB)
;		bit 1: RX overwrite
;		bit 2: TX ready
;		bit 3: RX ready
;		bit 4: TX queue empty
;		bit 5: RX queue full

; Timer module address map
; 0 counter bits 7:0
; 1 counter bits 15:8
; 2 counter bits 23:16
; 3 status
;		bit 0: counter 7:0 not zero
;		bit 1: counter 15:8 not zero
;		bit 2: counter 23:16 not zero
;		bit 3: counter 23:0 not zero
;		bit 4: VSYNC
;		bit 5: HSYNC

; Interrupt controller address map
; 0 status register
; 1 control register

; XenonGecko register interface (XGRI) address map
; 0 STATUS 
;		bit 0: a_empty (attribute buffer is completely empty)
;		bit 1: a_full (attribute buffer is completely full) This triggers hardware to empty the buffer and clear the flag automatically.
;		bit 2: p_empty (pattern buffer is completely empty)
;		bit 3: p_full (pattern buffer is completely full) This triggers hardware to empty the buffer and clear the flag automatically.
; 1 PAR (pattern address register) This register is automatically incremented as pattern entries are written to main memory.
; 2 AAR (attribute address register) This register is automatically incremented (by 4) as pattern entries are written to main memory.
; 3 PDR (pattern data register) This register is used to write pattern entries bypassing the data cache.
; 4 ADR (attribute data register) This register is used to write attribute entries bypassing the data cache.

;Things worthy of note:
; Writes to pattern data through XGRI are buffered by a 16 word FIFO.
; Writes to attribute data through XGRI are buffered by a 4 word FIFO.
; PAR and AAR should not be written to unless the corresponding FIFO is completely empty.
; The values in AAR and PAR are not physical addresses, they are indices for the attribute and pattern tables.
; Attribute entries should be updated in groups of 4, and the value written to AAR should be a multiple of 4.
; Writing other values to AAR will expose the interleaved access pattern of the main memory.
; The physical address range of the pattern table is 0x020000 to 0x02FFFF
; The physical address range of the attribute table is 0x030000 to 0x0312BF

; Address map for data space:
; 0x00000000 to 0xFFFFFFFF	active data memory page (cached)

; Address map for program space:
; 0x00000000 to 0xFFFFFFFF	active program memory page (cached)

; REMEMBER: PROGRAM AND DATA MEMORIES ARE BOTH WORD ADDRESSABLE!
; THE TWO MEMORY SPACES ARE SHARED THROUGH THE CACHE CONTROLLERS, BOTH CACHES MUST BE FLUSHED FOR CHANGES TO THE DATA SPACE TO SHOW UP IN THE PROGRAM SPACE.

AUX0	EQU R16
AUX1 	EQU R17
AUX2 	EQU R18
AUX3	EQU R19
DAL		EQU R20
DAH		EQU R21
IAL		EQU R22
IAH		EQU R23
DD		EQU R24
ID		EQU R25
CAL		EQU R26
CAH 	EQU R27
MULL 	EQU R28
MULH 	EQU R29
STATUS 	EQU R30
CONTROL EQU R31

TIMER_COUNT_0 EQU $FFF0
TIMER_COUNT_1 EQU $FFF1
TIMER_COUNT_2 EQU $FFF2
TIMER_STATUS EQU $FFF3

KB_DATA EQU $FFF4
KB_STATUS EQU $FFF5

V_MODE EQU $FFF6
VGA_MODE EQU $FFF7	;THESE TWO ARE THE SAME REGISTER

MSC_P_CONTROL EQU $FFF8
MSC_P_PAGE EQU $FFF9
MSC_D_CONTROL EQU $FFFA
MSC_D_PAGE EQU $FFFB

MSC_CMD_RESET EQU $01
MSC_CMD_FLUSH EQU $02
MSC_CMD_PREFETCH EQU $04
MSC_CMD_ENABLE EQU $08

HEX_INDICATORS EQU $FFFC	;BITS 14:12 CONTROL THE DECIMAL POINTS
HEX_DISPLAY EQU $FFFD	;THESE TWO ARE THE SAME REGISTER

UART_DATA EQU $FFFE
UART_STATUS EQU $FFFF

INTERRUPT_STATUS EQU $FFEE
INTERRUPT_CONTROL EQU $FFEF

XGRI_STATUS EQU $FFE0
XGRI_PAR EQU $FFE1
XGRI_AAR EQU $FFE2
XGRI_PDR EQU $FFE3
XGRI_ADR EQU $FFE4
