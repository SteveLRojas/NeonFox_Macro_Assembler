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
;* DE1_PROGRAMMER.INC            HEADER FILE      *
;* Copyright (C) 2021 Esteban Looser-Rojas.       *
;* Contains register defines for the NeonFox CPU  *
;* and address defines for platform peripherals.  *
;* Addresses are for the DE1 programmer platform. *
;**************************************************

;Address map for IO space:
; 0x0000 to 0xFFD7	unused
; 0xFFD8 to 0xFFDF	IOMM (read write)
; 0xFFE0 to 0xFFE7	unused
; 0xFFE8 to 0xFFED	unused
; 0xFFEE to 0xFFEF	interrupt controller (read write)
; 0xFFF0 to 0xFFF3	timer module (read write)
; 0xFFF4 to 0xFFF5	keyboard module (read write)
; 0xFFF6 to 0xFFF7	unused
; 0xFFF8 to 0xFFFB	unused
; 0xFFFC to 0xFFFD	hex display (write only)
; 0xFFFE to 0xFFFF	RS-232 module	(read write)

; RS-232 module address map
; 0 data register
; 1 status register
;		bit 0: TX overwrite (LSB)
;		bit 1: RX overwrite
;		bit 2: TX ready
;		bit 3: RX ready
;		bit 4: TX queue empty
;		bit 5: RX queue full
;		bit 6: reserved
;		bit 7: busy flag

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

;Address map for data and program space:
; 0x00000000 to 0x00001FFF	16KB main memory
; 0x00002000 to 0xFFFFFFFF	Not implemented

; REMEMBER: PROGRAM AND DATA MEMORIES ARE BOTH WORD ADDRESSABLE!

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
RAL 	EQU R28
RAH 	EQU R29
STATUS 	EQU R30
ZERO	EQU R31

TIMER_COUNT_0 EQU $FFF0
TIMER_COUNT_1 EQU $FFF1
TIMER_COUNT_2 EQU $FFF2
TIMER_STATUS EQU $FFF3

KB_DATA EQU $FFF4
KB_STATUS EQU $FFF5

HEX_INDICATORS EQU $FFFC
LED_INDICATORS EQU $FFFD

UART_DATA EQU $FFFE
UART_STATUS EQU $FFFF

INTERRUPT_STATUS EQU $FFEE
INTERRUPT_CONTROL EQU $FFEF

IOMM_WRITE_ADDR_HIGH 	EQU $FFD8
IOMM_WRITE_ADDR_LOW 	EQU $FFD9
IOMM_WRITE_ADDR_INC 	EQU $FFDA
IOMM_MEM_DATA 			EQU $FFDB
IOMM_READ_ADDR_HIGH 	EQU $FFDC
IOMM_READ_ADDR_LOW 		EQU $FFDD
IOMM_READ_ADDR_INC	 	EQU $FFDE
IOMM_STATUS 			EQU $FFDF

ROM_END		EQU $1FFF		;PAD FILE TO 8K WORDS
BUFFER_SIZE	EQU $1000		;BUFFER SIZE IN WORDS
MEMORY_SIZE	EQU $00400000	;MEMORY SIZE IN BYTES

;VARIABLES
SERIAL_NUM_WORDS_H	EQU $1FFF
SERIAL_NUM_WORDS_L	EQU $1FFE

FLASH_NUM_WORDS_H	EQU $1FFD
FLASH_NUM_WORDS_L	EQU $1FFC

FLASH_ADDRESS_H	EQU $1FFB
FLASH_ADDRESS_L	EQU $1FFA

FLASH_BYTE_ADDR_H	EQU $1FF9
FLASH_BYTE_ADDR_L	EQU $1FF8

WORDS_REMAINING_H	EQU $1FF7
WORDS_REMAINING_L	EQU $1FF6

BUFFER_END		EQU $1FF5
BUFFER_START	EQU $0FF6
