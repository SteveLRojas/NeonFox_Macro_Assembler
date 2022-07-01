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
;* neonfox_programmer.asm          SOURCE FILE    *
;* Copyright (C) 2022 Esteban Looser-Rojas.       *
;* Contains firmware for the NeonFox ROM          *
;* programmer project.                            *
;**************************************************

;INCLUDE "NEXYS2_PROGRAMMER.INC"
INCLUDE "DE1_PROGRAMMER.INC"

;VARIABLES ARE DEFINED IN THE PLATFORM HEADER FILE.

;CONSTANTS
CMD_READ		EQU $01
CMD_WRITE		EQU $02
CMD_UNLOCK_ALL	EQU $04
CMD_LOCK_ALL	EQU $05
CMD_GET_STATUS	EQU $06
CMD_ERASE_ALL	EQU $07
CMD_TEST		EQU $09

;#############################################################################
	ORG 0
	BRA INT_RESET
	NOP
	RET	;HSYNC
	NOP
	RET ;VSYNC
	NOP
	RET	;UART RX
	NOP
	RET	;UART TX
	NOP
	RET	;KB RX
	NOP
	RET	;TIMER
	NOP
	RET	;INT7
	NOP
	RET	;INT8
	NOP
	RET ;INT9
	NOP
	RET	;INT10
	NOP
	RET	;INT11
	NOP
	RET	;INT12
	NOP
	RET	;INT13
	NOP
	RET	;INT14
	NOP
	RET ;INT15
	NOP
;#############################################################################

;#############################################################################
INT_RESET
;SET ALL REGISTERS TO ZERO
	MOVE W, ZERO, AUX0
	MOVE W, ZERO, AUX1
	MOVE W, ZERO, AUX2
	MOVE W, ZERO, AUX3
	MOVE W, ZERO, R0
	MOVE W, ZERO, R1
	MOVE W, ZERO, R2
	MOVE W, ZERO, R3
	MOVE W, ZERO, R4
	MOVE W, ZERO, R5
	MOVE W, ZERO, R6
	MOVE W, ZERO, R7
	MOVE W, ZERO, R8
	MOVE W, ZERO, R9
	MOVE W, ZERO, R10
	MOVE W, ZERO, R11
	MOVE W, ZERO, R12
	MOVE W, ZERO, R13
	MOVE W, ZERO, R14
	MOVE W, ZERO, R15
	MOVE W, ZERO, DAH
	MOVE W, ZERO, DAL
	MOVE W, ZERO, IAL
	MOVE W, ZERO, CAH
	MOVE W, ZERO, CAL

	LIM H, AUX1, `LH HEX_INDICATORS
	LIM L, AUX1, `LL HEX_INDICATORS
	MOVE W, AUX1, IAL
	LIM W, AUX0, $01
	MOVE W, AUX0, ID

	LIM H, AUX1, `HH FLASH_INIT
	LIM L, AUX1, `HL FLASH_INIT
	LIM H, AUX0, `LH FLASH_INIT
	LIM L, AUX0, `LL FLASH_INIT
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	LIM H, AUX1, `LH HEX_INDICATORS
	LIM L, AUX1, `LL HEX_INDICATORS
	MOVE W, AUX1, IAL
	LIM W, AUX0, $02
	MOVE W, AUX0, ID

	LIM H, AUX1, `HH MAIN
	LIM L, AUX1, `HL MAIN
	LIM H, AUX0, `LH MAIN
	LIM L, AUX0, `LL MAIN
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	JMP
	NOP
;#############################################################################

;INCLUDE LIBRARIES
;INCLUDE "NEXYS2_FLASH.INC"
INCLUDE "DE1_FLASH.INC"
INCLUDE "NEONFOX_SERIAL.INC"

;#############################################################################
MAIN
	;WAIT FOR PING ($DEADBEEF)
	LIM H, AUX1, `HH SERIAL_GET_BYTE
	LIM L, AUX1, `HL SERIAL_GET_BYTE
	LIM H, AUX0, `LH SERIAL_GET_BYTE
	LIM L, AUX0, `LL SERIAL_GET_BYTE
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	;COMPARE WITH $DE, IF NO MATCH GO BACK TO MAIN
	LIM L, AUX0, $DE
	XOR L, R11, R11
	BRNZ MAIN
	NOP

COM_PING
	CALL	;SERIAL_GET_BYTE
	NOP

	;COMPARE WITH $AD, IF NO MATCH GO BACK TO MAIN
	LIM L, AUX0, $AD
	XOR L, R11, R11
	BRNZ MAIN
	NOP

	CALL	;SERIAL_GET_BYTE
	NOP

	;COMPARE WITH $BE, IF NO MATCH GO BACK TO MAIN
	LIM L, AUX0, $BE
	XOR L, R11, R11
	BRNZ MAIN
	NOP

	CALL	;SERIAL_GET_BYTE
	NOP

	;COMPARE WITH $EF, IF NO MATCH GO BACK TO MAIN
	LIM L, AUX0, $EF
	XOR L, R11, R11
	BRNZ MAIN
	NOP

	;ACK PING
	LIM H, AUX1, `HH SERIAL_SEND_BYTE
	LIM L, AUX1, `HL SERIAL_SEND_BYTE
	LIM H, AUX0, `LH SERIAL_SEND_BYTE
	LIM L, AUX0, `LL SERIAL_SEND_BYTE
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL

	LIM L, AUX0, $55
	MOVE L, AUX0, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	LIM L, AUX0, $AA
	MOVE L, AUX0, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	LIM L, AUX0, $F0
	MOVE L, AUX0, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	LIM L, AUX0, $0F
	MOVE L, AUX0, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	;SEND MEMORY SIZE
	LIM L, AUX0, `HH MEMORY_SIZE
	MOVE L, AUX0, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	LIM L, AUX0, `HL MEMORY_SIZE
	MOVE L, AUX0, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	LIM L, AUX0, `LH MEMORY_SIZE
	MOVE L, AUX0, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	LIM L, AUX0, `LL MEMORY_SIZE
	MOVE L, AUX0, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	LIM H, AUX1, `LH HEX_INDICATORS
	LIM L, AUX1, `LL HEX_INDICATORS
	MOVE W, AUX1, IAL
	LIM H, AUX0, $BE
	LIM L, AUX0, $EF
	MOVE W, AUX0, ID

	;GET COMMAND
M_COMMAND
	LIM H, AUX1, `HH SERIAL_GET_BYTE
	LIM L, AUX1, `HL SERIAL_GET_BYTE
	LIM H, AUX0, `LH SERIAL_GET_BYTE
	LIM L, AUX0, `LL SERIAL_GET_BYTE
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	LIM L, AUX0, $DE
	XOR L, R11, ZERO
	BRZ COM_PING
	NOP

	; NOT PING
	LIM L, AUX0, CMD_READ
	XOR L, R11, ZERO
	BRNZ M_NOT_READ
	NOP

	LIM H, AUX1, `HH COM_READ
	LIM L, AUX1, `HL COM_READ
	LIM H, AUX0, `LH COM_READ
	LIM L, AUX0, `LL COM_READ
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	BRA M_COMMAND
	NOP

M_NOT_READ
	LIM L, AUX0, CMD_WRITE
	XOR L, R11, ZERO
	BRNZ M_NOT_WRITE
	NOP

	LIM H, AUX1, `HH COM_WRITE
	LIM L, AUX1, `HL COM_WRITE
	LIM H, AUX0, `LH COM_WRITE
	LIM L, AUX0, `LL COM_WRITE
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	BRA M_COMMAND
	NOP

M_NOT_WRITE
	LIM L, AUX0, CMD_UNLOCK_ALL
	XOR L, R11, ZERO
	BRNZ M_NOT_UNLOCK_ALL
	NOP

	LIM H, AUX1, `HH COM_UNLOCK_ALL
	LIM L, AUX1, `HL COM_UNLOCK_ALL
	LIM H, AUX0, `LH COM_UNLOCK_ALL
	LIM L, AUX0, `LL COM_UNLOCK_ALL
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	BRA M_COMMAND
	NOP

M_NOT_UNLOCK_ALL
	LIM L, AUX0, CMD_LOCK_ALL
	XOR L, R11, ZERO
	BRNZ M_NOT_LOCK_ALL
	NOP

	LIM H, AUX1, `HH COM_LOCK_ALL
	LIM L, AUX1, `HL COM_LOCK_ALL
	LIM H, AUX0, `LH COM_LOCK_ALL
	LIM L, AUX0, `LL COM_LOCK_ALL
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	BRA M_COMMAND
	NOP

M_NOT_LOCK_ALL
	LIM L, AUX0, CMD_GET_STATUS
	XOR L, R11, ZERO
	BRNZ M_NOT_GET_STATUS
	NOP

	LIM H, AUX1, `HH COM_GET_STATUS
	LIM L, AUX1, `HL COM_GET_STATUS
	LIM H, AUX0, `LH COM_GET_STATUS
	LIM L, AUX0, `LL COM_GET_STATUS
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	BRA M_COMMAND
	NOP

M_NOT_GET_STATUS
	LIM L, AUX0, CMD_ERASE_ALL
	XOR L, R11, ZERO
	BRNZ M_NOT_TEST
	NOP

	LIM H, AUX1, `HH COM_ERASE_ALL
	LIM L, AUX1, `HL COM_ERASE_ALL
	LIM H, AUX0, `LH COM_ERASE_ALL
	LIM L, AUX0, `LL COM_ERASE_ALL
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	BRA M_COMMAND
	NOP

M_NOT_ERASE_ALL
	LIM L, AUX0, CMD_TEST
	XOR L, R11, ZERO
	BRNZ M_NOT_TEST
	NOP

	LIM H, AUX1, `HH COM_TEST
	LIM L, AUX1, `HL COM_TEST
	LIM H, AUX0, `LH COM_TEST
	LIM L, AUX0, `LL COM_TEST
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	BRA M_COMMAND
	NOP

M_NOT_TEST
	LIM H, AUX1, `HH SERIAL_SEND_BYTE
	LIM L, AUX1, `HL SERIAL_SEND_BYTE
	LIM H, AUX0, `LH SERIAL_SEND_BYTE
	LIM L, AUX0, `LL SERIAL_SEND_BYTE
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL

	MOVE W, ZERO, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	CALL	;SERIAL_SEND_BYTE
	NOP

	CALL	;SERIAL_SEND_BYTE
	NOP

	CALL	;SERIAL_SEND_BYTE
	NOP

	BRA MAIN
	NOP
;#############################################################################

;#############################################################################
; NAME:	COM_READ
; INPUTS:	NONE
; MODIFIES: AUX0, AUX1, AUX2, R2, R3, R4, R5, R6, R11, CAH, CAL, DAL, IAL
; RETURNS:	NONE
;#############################################################################
COM_READ
	LIM H, AUX1, `LH HEX_INDICATORS
	LIM L, AUX1, `LL HEX_INDICATORS
	MOVE W, AUX1, IAL
	LIM H, AUX0, $C0
	LIM L, AUX0, $01
	MOVE W, AUX0, ID

	; GET NUM BYTES AND RESPOND TO REQUEST
	LIM H, AUX1, `HH SERIAL_GET_BYTE
	LIM L, AUX1, `HL SERIAL_GET_BYTE
	LIM H, AUX0, `LH SERIAL_GET_BYTE
	LIM L, AUX0, `LL SERIAL_GET_BYTE
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL

	; GET FIRST BYTE -> AUX1 HIGH
	CALL	;SERIAL_GET_BYTE
	NOP
	MOVE L, R11, R2	;SAVE SO WE CAN SEND IT BACK LATER
	MOVE S, R11, R11
	MOVE H, R11, AUX1

	; GET SECOND BYTE -> AUX1 LOW
	CALL	;SERIAL_GET_BYTE
	NOP
	MOVE L, R11, R3	;SAVE SO WE CAN SEND IT BACK LATER
	MOVE L, R11, AUX1

	; GET THIRD BYTE -> AUX2 HIGH
	CALL	;SERIAL_GET_BYTE
	NOP
	MOVE L, R11, R4
	MOVE S, R11, R11
	MOVE H, R11, AUX2

	; GET FOURTH BYTE -> AUX2 LOW
	CALL	;SERIAL_GET_BYTE
	NOP
	MOVE L, R11, R5
	MOVE L, R11, AUX2

	; DIVIDE 4-BYTE NUMBER BY 2
	ROR W, AUX2, AUX2	;ROTATE LOW REG (BIT 0 BECOMES BIT 15)
	LIM H, AUX0, $7F
	LIM L, AUX0, $FF
	AND W, AUX2, AUX2	;CLEAR BIT 15 OF LOW REG

	ROR W, AUX1, AUX1	;ROTATE HIGH REG
	NOT W, AUX0, AUX0
	AND W, AUX1, AUX0	;GOT BIT WE SHIFTED OUT FROM HIGH REG - STORE IN AUX0, 15
	OR W, AUX2, AUX2	;PUT BIT IN LOW REG, 15

	LIM H, AUX0, $7F
	LIM L, AUX0, $FF
	AND W, AUX1, AUX1 	;CLEAR BIT 15 OF HIGH REG

	LIM H, AUX0, `LH WORDS_REMAINING_H
	LIM L, AUX0, `LL WORDS_REMAINING_H
	MOVE W, AUX0, DAL
	MOVE W, AUX1, DD

	LIM H, AUX0, `LH WORDS_REMAINING_L
	LIM L, AUX0, `LL WORDS_REMAINING_L
	MOVE W, AUX0, DAL
	MOVE W, AUX2, DD

	LIM H, AUX1, `HH SERIAL_SEND_BYTE
	LIM L, AUX1, `HL SERIAL_SEND_BYTE
	LIM H, AUX0, `LH SERIAL_SEND_BYTE
	LIM L, AUX0, `LL SERIAL_SEND_BYTE
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL

	MOVE L, R2, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	MOVE L, R3, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	MOVE L, R4, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	MOVE L, R5, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	; SET FLASH ADDRESS
	LIM H, AUX1, `LH FLASH_ADDRESS_H
	LIM L, AUX1, `LL FLASH_ADDRESS_H
	MOVE W, AUX1, DAL
	MOVE W, ZERO, DD

	LIM H, AUX1, `LH FLASH_ADDRESS_L
	LIM L, AUX1, `LL FLASH_ADDRESS_L
	MOVE W, AUX1, DAL
	MOVE W, ZERO, DD

	LIM H, AUX1, `HH FLASH_SET_ADDRESS_READ
	LIM L, AUX1, `HL FLASH_SET_ADDRESS_READ
	LIM H, AUX0, `LH FLASH_SET_ADDRESS_READ
	LIM L, AUX0, `LL FLASH_SET_ADDRESS_READ
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

COMR_LOOP
	; STORE WORDS REMAINING IN R4, R3
	LIM H, AUX1, `LH WORDS_REMAINING_H
	LIM L, AUX1, `LL WORDS_REMAINING_H
	MOVE W, AUX1, DAL
	MOVE W, DD, R4

	LIM H, AUX1, `LH WORDS_REMAINING_L
	LIM L, AUX1, `LL WORDS_REMAINING_L
	MOVE W, AUX1, DAL
	MOVE W, DD, R3

	;UPDATE HEX DISPLAY
	LIM H, AUX1, `LH HEX_INDICATORS
	LIM L, AUX1, `LL HEX_INDICATORS
	MOVE W, AUX1, IAL
	MOVE W, R4, ID

	; STORE BUFFER SIZE IN R6, R5
	LIM H, AUX0, `LH BUFFER_SIZE
	LIM L, AUX0, `LL BUFFER_SIZE
	MOVE W, ZERO, R6
	MOVE W, AUX0, R5

	; SUBTRACT WORDS REMAINING FROM BUFFER SIZE
	MOVE W, R3, AUX0
	SUB W, R5, R5
	MOVE W, R4, AUX0
	SUBC W, R6, R6

 	; CHECK IF NUMBER OF BYTES IS GREATER THAN BUFFER SIZE
	BITT W, STATUS, 3	;CHECK CARRY FLAG
	BRZ COMR_GREATER
	NOP

	; STORE WORDS REMAINING IN R4, R3
	LIM H, AUX1, `LH WORDS_REMAINING_H
	LIM L, AUX1, `LL WORDS_REMAINING_H
	MOVE W, AUX1, DAL
	MOVE W, DD, R4

	LIM H, AUX1, `LH WORDS_REMAINING_L
	LIM L, AUX1, `LL WORDS_REMAINING_L
	MOVE W, AUX1, DAL
	MOVE W, DD, R3

	BRA COMR_TRANSFER

COMR_GREATER
	; STORE MAX TRANSFER SIZE IN R4, R3
	LIM H, AUX0, `LH BUFFER_SIZE
	LIM L, AUX0, `LL BUFFER_SIZE
	MOVE W, ZERO, R4
	MOVE W, AUX0, R3

COMR_TRANSFER
	; SET NUM WORDS TO TRANSFER
	LIM H, AUX1, `LH FLASH_NUM_WORDS_H
	LIM L, AUX1, `LL FLASH_NUM_WORDS_H
	MOVE W, AUX1, DAL
	MOVE W, R4, DD

	LIM H, AUX1, `LH FLASH_NUM_WORDS_L
	LIM L, AUX1, `LL FLASH_NUM_WORDS_L
	MOVE W, AUX1, DAL
	MOVE W, R3, DD

	LIM H, AUX1, `LH SERIAL_NUM_WORDS_H
	LIM L, AUX1, `LL SERIAL_NUM_WORDS_H
	MOVE W, AUX1, DAL
	MOVE W, R4, DD

	LIM H, AUX1, `LH SERIAL_NUM_WORDS_L
	LIM L, AUX1, `LL SERIAL_NUM_WORDS_L
	MOVE W, AUX1, DAL
	MOVE W, R3, DD

	; READ WORDS
	LIM H, AUX1, `HH FLASH_GET_N_WORDS
	LIM L, AUX1, `HL FLASH_GET_N_WORDS
	LIM H, AUX0, `LH FLASH_GET_N_WORDS
	LIM L, AUX0, `LL FLASH_GET_N_WORDS
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	; SEND WORDS
	LIM H, AUX1, `HH SERIAL_SEND_N_WORDS
	LIM L, AUX1, `HL SERIAL_SEND_N_WORDS
	LIM H, AUX0, `LH SERIAL_SEND_N_WORDS
	LIM L, AUX0, `LL SERIAL_SEND_N_WORDS
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	; STORE WORDS REMAINING IN R4, R3
	LIM H, AUX1, `LH WORDS_REMAINING_H
	LIM L, AUX1, `LL WORDS_REMAINING_H
	MOVE W, AUX1, DAL
	MOVE W, DD, R4

	LIM H, AUX1, `LH WORDS_REMAINING_L
	LIM L, AUX1, `LL WORDS_REMAINING_L
	MOVE W, AUX1, DAL
	MOVE W, DD, R3

	; STORE WORDS TRANSFERED IN R6, R5
	LIM H, AUX1, `LH FLASH_NUM_WORDS_H
	LIM L, AUX1, `LL FLASH_NUM_WORDS_H
	MOVE W, AUX1, DAL
	MOVE W, DD, R6

	LIM H, AUX1, `LH FLASH_NUM_WORDS_L
	LIM L, AUX1, `LL FLASH_NUM_WORDS_L
	MOVE W, AUX1, DAL
	MOVE W, DD, R5

	; SUBTRACT WORDS TRANSFERED FROM BYTES REMAINING
	MOVE W, R5, AUX0
	SUB W, R3, R3
	MOVE W, R6, AUX0
	SUBC W, R4, R4

	; UPDATE NUMBER OF WORDS REMAINING
	LIM H, AUX1, `LH WORDS_REMAINING_H
	LIM L, AUX1, `LL WORDS_REMAINING_H
	MOVE W, AUX1, DAL
	MOVE W, R4, DD

	LIM H, AUX1, `LH WORDS_REMAINING_L
	LIM L, AUX1, `LL WORDS_REMAINING_L
	MOVE W, AUX1, DAL
	MOVE W, R3, DD

	; CHECK IF BYTES REMAINING IS ZERO
	MOVE W, R4, AUX0
	OR W, R3, AUX0
	BRNZ COMR_LOOP
	NOP

	RET
	NOP
;#############################################################################

;#############################################################################
; NAME:	COM_WRITE
; INPUTS:	NONE
; MODIFIES: AUX0, AUX1, AUX2, R3, R4, R5, R6, R11, CAH, CAL, DAL, IAL
; RETURNS:	NONE
;#############################################################################
COM_WRITE
	LIM H, AUX1, `LH HEX_INDICATORS
	LIM L, AUX1, `LL HEX_INDICATORS
	MOVE W, AUX1, IAL
	LIM H, AUX0, $C0
	LIM L, AUX0, $02
	MOVE W, AUX0, ID

	;GET NUM BYTES
	LIM H, AUX1, `HH SERIAL_GET_BYTE
	LIM L, AUX1, `HL SERIAL_GET_BYTE
	LIM H, AUX0, `LH SERIAL_GET_BYTE
	LIM L, AUX0, `LL SERIAL_GET_BYTE
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL

	; GET FIRST BYTE -> AUX1 HIGH
	CALL	;SERIAL_GET_BYTE
	NOP
	MOVE S, R11, R11
	MOVE H, R11, AUX1

	; GET SECOND BYTE -> AUX1 LOW
	CALL	;SERIAL_GET_BYTE
	NOP
	MOVE L, R11, AUX1

	; GET THIRD BYTE -> AUX2 HIGH
	CALL	;SERIAL_GET_BYTE
	NOP
	MOVE S, R11, R11
	MOVE H, R11, AUX2

	; GET FOURTH BYTE -> AUX2 LOW
	CALL	;SERIAL_GET_BYTE
	NOP
	MOVE L, R11, AUX2

	; DIVIDE 3-BYTE NUMBER BY 2
	ROR W, AUX2, AUX2	;ROTATE LOW REG (BIT 0 BECOMES BIT 15)
	LIM H, AUX0, $7F
	LIM L, AUX0, $FF
	AND W, AUX2, AUX2	;CLEAR BIT 15 OF LOW REG

	ROR W, AUX1, AUX1	;ROTATE HIGH REG
	NOT W, AUX0, AUX0
	AND W, AUX1, AUX0	;GOT BIT WE SHIFTED OUT FROM HIGH REG - STORE IN AUX0, 15
	OR W, AUX2, AUX2	;PUT BIT IN LOW REG, 15

	LIM H, AUX0, $7F
	LIM L, AUX0, $FF
	AND W, AUX1, AUX1 	;CLEAR BIT 15 OF HIGH REG

	LIM H, AUX0, `LH WORDS_REMAINING_H
	LIM L, AUX0, `LL WORDS_REMAINING_H
	MOVE W, AUX0, DAL
	MOVE W, AUX1, DD

	LIM H, AUX0, `LH WORDS_REMAINING_L
	LIM L, AUX0, `LL WORDS_REMAINING_L
	MOVE W, AUX0, DAL
	MOVE W, AUX2, DD

	;SET FLASH ADDRESS
	LIM H, AUX1, `LH FLASH_ADDRESS_H
	LIM L, AUX1, `LL FLASH_ADDRESS_H
	MOVE W, AUX1, DAL
	MOVE W, ZERO, DD

	LIM H, AUX1, `LH FLASH_ADDRESS_L
	LIM L, AUX1, `LL FLASH_ADDRESS_L
	MOVE W, AUX1, DAL
	MOVE W, ZERO, DD

	LIM H, AUX1, `HH FLASH_SET_ADDRESS_WRITE
	LIM L, AUX1, `HL FLASH_SET_ADDRESS_WRITE
	LIM H, AUX0, `LH FLASH_SET_ADDRESS_WRITE
	LIM L, AUX0, `LL FLASH_SET_ADDRESS_WRITE
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

COMW_LOOP
	;STORE WORDS REMAINING IN R4, R3
	LIM H, AUX1, `LH WORDS_REMAINING_H
	LIM L, AUX1, `LL WORDS_REMAINING_H
	MOVE W, AUX1, DAL
	MOVE W, DD, R4

	LIM H, AUX1, `LH WORDS_REMAINING_L
	LIM L, AUX1, `LL WORDS_REMAINING_L
	MOVE W, AUX1, DAL
	MOVE W, DD, R3

	;UPDATE HEX DISPLAY
	LIM H, AUX1, `LH HEX_INDICATORS
	LIM L, AUX1, `LL HEX_INDICATORS
	MOVE W, AUX1, IAL
	MOVE W, R4, ID

	;STORE BUFFER SIZE IN R6, R5
	LIM H, AUX0, `LH BUFFER_SIZE
	LIM L, AUX0, `LL BUFFER_SIZE
	MOVE W, ZERO, R6
	MOVE W, AUX0, R5

	;SUBTRACT WORDS REMAINING FROM BUFFER SIZE
	MOVE W, R3, AUX0
	SUB W, R5, R5
	MOVE W, R4, AUX0
	SUBC W, R6, R6

	;CHECK IF NUMBER OF BYTES IS GREATER THAN BUFFER SIZE
	BITT W, STATUS, 3	;CHECK CARRY FLAG
	BRZ COMW_GREATER
	NOP

	;STORE WORDS REMAINING IN R4, R3
	LIM H, AUX1, `LH WORDS_REMAINING_H
	LIM L, AUX1, `LL WORDS_REMAINING_H
	MOVE W, AUX1, DAL
	MOVE W, DD, R4

	LIM H, AUX1, `LH WORDS_REMAINING_L
	LIM L, AUX1, `LL WORDS_REMAINING_L
	MOVE W, AUX1, DAL
	MOVE W, DD, R3

	BRA COMW_TRANSFER

COMW_GREATER
	;STORE MAX TRANSFER SIZE IN R4, R3
	LIM H, AUX0, `LH BUFFER_SIZE
	LIM L, AUX0, `LL BUFFER_SIZE
	MOVE W, ZERO, R4
	MOVE W, AUX0, R3

COMW_TRANSFER
	;REQUEST BYTES
	MOVE W, R4, AUX1 ;COPY BLOCK SIZE (WORDS) TO AUX1, AUX2
	MOVE W, R3, AUX2	

	MOVE W, AUX2, AUX0 ;CALCULATE BLOCK SIZE IN BYTES
	ADD W, AUX2, AUX2
	MOVE W, AUX1, AUX0
	ADDC W, AUX1, AUX1			;MULTIPLY BY 2

	LIM H, AUX0, `HH SERIAL_SEND_BYTE
	LIM L, AUX0, `HL SERIAL_SEND_BYTE
	MOVE W, AUX0, CAH
	LIM H, AUX0, `LH SERIAL_SEND_BYTE
	LIM L, AUX0, `LL SERIAL_SEND_BYTE
	MOVE W, AUX0, CAL

	MOVE S, AUX1, R0			;SEND BLOCK SIZE (BYTES) VIA SERIAL 
	CALL	;SERIAL_SEND_BYTE
	NOP

	MOVE L, AUX1, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	MOVE S, AUX2, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	MOVE L, AUX2, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	;SET NUM WORDS TO TRANSFER
	LIM H, AUX1, `LH FLASH_NUM_WORDS_H
	LIM L, AUX1, `LL FLASH_NUM_WORDS_H
	MOVE W, AUX1, DAL
	MOVE W, R4, DD

	LIM H, AUX1, `LH FLASH_NUM_WORDS_L
	LIM L, AUX1, `LL FLASH_NUM_WORDS_L
	MOVE W, AUX1, DAL
	MOVE W, R3, DD

	LIM H, AUX1, `LH SERIAL_NUM_WORDS_H
	LIM L, AUX1, `LL SERIAL_NUM_WORDS_H
	MOVE W, AUX1, DAL
	MOVE W, R4, DD

	LIM H, AUX1, `LH SERIAL_NUM_WORDS_L
	LIM L, AUX1, `LL SERIAL_NUM_WORDS_L
	MOVE W, AUX1, DAL
	MOVE W, R3, DD

	;READ WORDS
	LIM H, AUX1, `HH SERIAL_GET_N_WORDS
	LIM L, AUX1, `HL SERIAL_GET_N_WORDS
	LIM H, AUX0, `LH SERIAL_GET_N_WORDS
	LIM L, AUX0, `LL SERIAL_GET_N_WORDS
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	;SEND WORDS
	LIM H, AUX1, `HH FLASH_SEND_N_WORDS
	LIM L, AUX1, `HL FLASH_SEND_N_WORDS
	LIM H, AUX0, `LH FLASH_SEND_N_WORDS
	LIM L, AUX0, `LL FLASH_SEND_N_WORDS
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	;STORE WORDS REMAINING IN R4, R3
	LIM H, AUX1, `LH WORDS_REMAINING_H
	LIM L, AUX1, `LL WORDS_REMAINING_H
	MOVE W, AUX1, DAL
	MOVE W, DD, R4

	LIM H, AUX1, `LH WORDS_REMAINING_L
	LIM L, AUX1, `LL WORDS_REMAINING_L
	MOVE W, AUX1, DAL
	MOVE W, DD, R3

	;STORE WORDS TRANSFERED IN R6, R5
	LIM H, AUX1, `LH FLASH_NUM_WORDS_H
	LIM L, AUX1, `LL FLASH_NUM_WORDS_H
	MOVE W, AUX1, DAL
	MOVE W, DD, R6

	LIM H, AUX1, `LH FLASH_NUM_WORDS_L
	LIM L, AUX1, `LL FLASH_NUM_WORDS_L
	MOVE W, AUX1, DAL
	MOVE W, DD, R5

	;SUBTRACT WORDS TRANSFERED FROM BYTES REMAINING
	MOVE W, R5, AUX0
	SUB W, R3, R3
	MOVE W, R6, AUX0
	SUBC W, R4, R4

	;UPDATE NUMBER OF WORDS REMAINING
	LIM H, AUX1, `LH WORDS_REMAINING_H
	LIM L, AUX1, `LL WORDS_REMAINING_H
	MOVE W, AUX1, DAL
	MOVE W, R4, DD

	LIM H, AUX1, `LH WORDS_REMAINING_L
	LIM L, AUX1, `LL WORDS_REMAINING_L
	MOVE W, AUX1, DAL
	MOVE W, R3, DD

	;CHECK IF BYTES REMAINING IS ZERO
	MOVE W, R4, AUX0
	OR W, R3, AUX0
	BRNZ COMW_LOOP
	NOP

	RET
	NOP
;#############################################################################

;#############################################################################
COM_VERIFY
	LIM H, AUX1, `HH SERIAL_SEND_BYTE
	LIM L, AUX1, `HL SERIAL_SEND_BYTE
	LIM H, AUX0, `LH SERIAL_SEND_BYTE
	LIM L, AUX0, `LL SERIAL_SEND_BYTE
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL

	LIM L, AUX0, $E0
	MOVE L, AUX0, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	LIM L, AUX0, $00
	MOVE L, AUX0, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	RET
	NOP
;#############################################################################

;#############################################################################
COM_UNLOCK_ALL
	LIM H, AUX1, `LH HEX_INDICATORS
	LIM L, AUX1, `LL HEX_INDICATORS
	MOVE W, AUX1, IAL
	LIM H, AUX0, $C0
	LIM L, AUX0, $04
	MOVE W, AUX0, ID

	LIM H, AUX1, `HH FLASH_UNLOCK_ALL_BLOCKS
	LIM L, AUX1, `HL FLASH_UNLOCK_ALL_BLOCKS
	LIM H, AUX0, `LH FLASH_UNLOCK_ALL_BLOCKS
	LIM L, AUX0, `LL FLASH_UNLOCK_ALL_BLOCKS
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	LIM H, AUX1, `HH SERIAL_SEND_BYTE
	LIM L, AUX1, `HL SERIAL_SEND_BYTE
	LIM H, AUX0, `LH SERIAL_SEND_BYTE
	LIM L, AUX0, `LL SERIAL_SEND_BYTE
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL

	LIM L, AUX0, $55
	MOVE L, AUX0, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	LIM L, AUX0, $AA
	MOVE L, AUX0, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	RET
	NOP
;#############################################################################

;#############################################################################
COM_LOCK_ALL
	LIM H, AUX1, `LH HEX_INDICATORS
	LIM L, AUX1, `LL HEX_INDICATORS
	MOVE W, AUX1, IAL
	LIM H, AUX0, $C0
	LIM L, AUX0, $05
	MOVE W, AUX0, ID

	LIM H, AUX1, `HH FLASH_LOCK_ALL_BLOCKS
	LIM L, AUX1, `HL FLASH_LOCK_ALL_BLOCKS
	LIM H, AUX0, `LH FLASH_LOCK_ALL_BLOCKS
	LIM L, AUX0, `LL FLASH_LOCK_ALL_BLOCKS
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	LIM H, AUX1, `HH SERIAL_SEND_BYTE
	LIM L, AUX1, `HL SERIAL_SEND_BYTE
	LIM H, AUX0, `LH SERIAL_SEND_BYTE
	LIM L, AUX0, `LL SERIAL_SEND_BYTE
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL

	LIM L, AUX0, $55
	MOVE L, AUX0, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	LIM L, AUX0, $AA
	MOVE L, AUX0, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	RET
	NOP
;#############################################################################

;#############################################################################
COM_GET_STATUS
	LIM H, AUX1, `HH SERIAL_SEND_BYTE
	LIM L, AUX1, `HL SERIAL_SEND_BYTE
	LIM H, AUX0, `LH SERIAL_SEND_BYTE
	LIM L, AUX0, `LL SERIAL_SEND_BYTE
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL

	LIM L, AUX0, $E0
	MOVE L, AUX0, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	LIM L, AUX0, $00
	MOVE L, AUX0, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	RET
	NOP
;#############################################################################

;#############################################################################
COM_ERASE_ALL
	LIM H, AUX1, `LH LED_INDICATORS
	LIM L, AUX1, `LL LED_INDICATORS
	MOVE W, AUX1, IAL
	LIM H, AUX0, $C0
	LIM L, AUX0, $07
	MOVE W, AUX0, ID

	LIM H, AUX1, `HH FLASH_ERASE_ALL_BLOCKS
	LIM L, AUX1, `HL FLASH_ERASE_ALL_BLOCKS
	LIM H, AUX0, `LH FLASH_ERASE_ALL_BLOCKS
	LIM L, AUX0, `LL FLASH_ERASE_ALL_BLOCKS
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	LIM H, AUX1, `HH SERIAL_SEND_BYTE
	LIM L, AUX1, `HL SERIAL_SEND_BYTE
	LIM H, AUX0, `LH SERIAL_SEND_BYTE
	LIM L, AUX0, `LL SERIAL_SEND_BYTE
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL

	LIM L, AUX0, $55
	MOVE L, AUX0, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	LIM L, AUX0, $AA
	MOVE L, AUX0, R0
	CALL	;SERIAL_SEND_BYTE
	NOP

	RET
	NOP
;#############################################################################

;#############################################################################
COM_TEST
	LIM H, AUX1, `LH HEX_INDICATORS
	LIM L, AUX1, `LL HEX_INDICATORS
	MOVE W, AUX1, IAL
	LIM H, AUX0, $C0
	LIM L, AUX0, $07
	MOVE W, AUX0, ID

	RET
	NOP
;#############################################################################

;#############################################################################
	ORG ROM_END
	DATA $FFFF
;##########################################################################EOF