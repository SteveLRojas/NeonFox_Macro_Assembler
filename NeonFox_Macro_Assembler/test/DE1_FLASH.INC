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
;* DE1_FLASH.INC            LIBRARY FILE          *
;* Copyright (C) 2022 Esteban Looser-Rojas.       *
;* Contains flash driver for the DE1 flash        *
;* programmer project based on NeonFox CPU.       *
;**************************************************

;#############################################################################
FLASH_INIT
	; CLEAR WRITE ADDRESS
	LIM H, AUX1, `LH IOMM_WRITE_ADDR_HIGH
	LIM L, AUX1, `LL IOMM_WRITE_ADDR_HIGH
	MOVE W, AUX1, IAL
	MOVE W, ZERO, ID

	LIM H, AUX1, `LH IOMM_WRITE_ADDR_LOW
	LIM L, AUX1, `LL IOMM_WRITE_ADDR_LOW
	MOVE W, AUX1, IAL
	MOVE W, ZERO, ID

	;SEND RESET COMMAND
	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $F0
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FI_WAIT_STATUS
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FI_WAIT_STATUS	;IF NOT SET KEEP WAITING

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_INC
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_INC
	MOVE W, AUX0, IAL
	MOVE W, ZERO, ID	;DISABLE AUTO INCREMENT

	LIM H, AUX0, `LH IOMM_READ_ADDR_INC
	LIM L, AUX0, `LL IOMM_READ_ADDR_INC
	MOVE W, AUX0, IAL
	MOVE W, ZERO, ID	;DISABLE AUTO INCREMENT

	RET
	NOP
;#############################################################################

;#############################################################################
FLASH_SET_ADDRESS_WRITE
	LIM H, AUX0, `LH FLASH_ADDRESS_H		
	LIM L, AUX0, `LL FLASH_ADDRESS_H
	MOVE W, AUX0, DAL
	MOVE W, DD, R4

	LIM H, AUX0, `LH FLASH_ADDRESS_L
	LIM L, AUX0, `LL FLASH_ADDRESS_L
	MOVE W, AUX0, DAL
	MOVE W, DD, R3

	MOVE W, R4, AUX1 ;COPY BLOCK SIZE (WORDS) TO AUX1, AUX2
	MOVE W, R3, AUX2	

	MOVE W, AUX2, AUX0 ;CALCULATE BLOCK SIZE IN BYTES
	ADD W, AUX2, AUX2
	MOVE W, AUX1, AUX0
	ADDC W, AUX1, AUX1			;MULTIPLY BY 2

	LIM H, AUX0, `LH FLASH_BYTE_ADDR_H		
	LIM L, AUX0, `LL FLASH_BYTE_ADDR_H
	MOVE W, AUX0, DAL
	MOVE W, AUX1, DD

	LIM H, AUX0, `LH FLASH_BYTE_ADDR_L
	LIM L, AUX0, `LL FLASH_BYTE_ADDR_L
	MOVE W, AUX0, DAL
	MOVE W, AUX2, DD

	RET
	NOP
;#############################################################################

;#############################################################################
FLASH_SET_ADDRESS_READ
	;SET AUTO INCREMENT FOR READ
	LIM H, AUX0, `LH IOMM_READ_ADDR_INC
	LIM L, AUX0, `LL IOMM_READ_ADDR_INC
	MOVE W, AUX0, IAL
	LIM W, AUX0, $00
	MOVE W, AUX0, ID	;DISABLE AUTO INCREMENT

	LIM H, AUX0, `LH FLASH_ADDRESS_H		
	LIM L, AUX0, `LL FLASH_ADDRESS_H
	MOVE W, AUX0, DAL
	MOVE W, DD, R4

	LIM H, AUX0, `LH FLASH_ADDRESS_L
	LIM L, AUX0, `LL FLASH_ADDRESS_L
	MOVE W, AUX0, DAL
	MOVE W, DD, R3

	MOVE W, R4, AUX1 ;COPY BLOCK SIZE (WORDS) TO AUX1, AUX2
	MOVE W, R3, AUX2	

	MOVE W, AUX2, AUX0 ;CALCULATE BLOCK SIZE IN BYTES
	ADD W, AUX2, AUX2
	MOVE W, AUX1, AUX0
	ADDC W, AUX1, AUX1			;MULTIPLY BY 2

	LIM H, AUX0, `LH FLASH_BYTE_ADDR_H		
	LIM L, AUX0, `LL FLASH_BYTE_ADDR_H
	MOVE W, AUX0, DAL
	MOVE W, AUX1, DD

	LIM H, AUX0, `LH FLASH_BYTE_ADDR_L
	LIM L, AUX0, `LL FLASH_BYTE_ADDR_L
	MOVE W, AUX0, DAL
	MOVE W, AUX2, DD

	LIM H, AUX0, `LH IOMM_READ_ADDR_HIGH
	LIM L, AUX0, `LL IOMM_READ_ADDR_HIGH
	MOVE W, AUX0, IAL
	MOVE W, AUX1, ID

	LIM H, AUX0, `LH IOMM_READ_ADDR_LOW
	LIM L, AUX0, `LL IOMM_READ_ADDR_LOW
	MOVE W, AUX0, IAL
	MOVE W, AUX2, ID

	RET
	NOP
;#############################################################################

;#############################################################################
FLASH_SET_BYTE_ADDR_WRITE
	LIM H, AUX0, `LH FLASH_BYTE_ADDR_H		
	LIM L, AUX0, `LL FLASH_BYTE_ADDR_H
	MOVE W, AUX0, DAL
	MOVE W, DD, AUX1

	LIM H, AUX0, `LH FLASH_BYTE_ADDR_L
	LIM L, AUX0, `LL FLASH_BYTE_ADDR_L
	MOVE W, AUX0, DAL
	MOVE W, DD, AUX2

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_HIGH
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_HIGH
	MOVE W, AUX0, IAL
	MOVE W, AUX1, ID

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_LOW
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_LOW
	MOVE W, AUX0, IAL
	MOVE W, AUX2, ID

	RET
	NOP
;#############################################################################

;#############################################################################
FLASH_SET_BYTE_ADDR_READ
	;SET AUTO INCREMENT FOR READ
	LIM H, AUX0, `LH IOMM_READ_ADDR_INC
	LIM L, AUX0, `LL IOMM_READ_ADDR_INC
	MOVE W, AUX0, IAL
	LIM W, AUX0, $00
	MOVE W, AUX0, ID	;DISABLE AUTO INCREMENT

	LIM H, AUX0, `LH FLASH_BYTE_ADDR_H		
	LIM L, AUX0, `LL FLASH_BYTE_ADDR_H
	MOVE W, AUX0, DAL
	MOVE W, DD, AUX1

	LIM H, AUX0, `LH FLASH_BYTE_ADDR_L
	LIM L, AUX0, `LL FLASH_BYTE_ADDR_L
	MOVE W, AUX0, DAL
	MOVE W, DD, AUX2
	
	LIM H, AUX0, `LH IOMM_READ_ADDR_HIGH
	LIM L, AUX0, `LL IOMM_READ_ADDR_HIGH
	MOVE W, AUX0, IAL
	MOVE W, AUX1, ID

	LIM H, AUX0, `LH IOMM_READ_ADDR_LOW
	LIM L, AUX0, `LL IOMM_READ_ADDR_LOW
	MOVE W, AUX0, IAL
	MOVE W, AUX2, ID		;WRITING TO IOMM_READ_ADDR_LOW TRIGGERS A READ

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FSBAR_WAIT
	BITT W, ID, 1		;CHECK READ READY FLAG
	BRZ FSBAR_WAIT		;IF NOT SET KEEP WAITING
	NOP
	
	RET
	NOP
;#############################################################################

;#############################################################################
FLASH_GET_WORD
	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
	MOVE W, ZERO, ID	;WRITE TO STATUS REGISTER TO INITIATE READ
FGW_WAIT_L
	BITT W, ID, 1	;CHECK READ READY FLAG
	BRZ FGW_WAIT_L	;IF NOT SET KEEP WAITING
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	MOVE L, ID, R11

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
	MOVE W, ZERO, ID	;WRITE TO STATUS REGISTER TO INITIATE READ
FGW_WAIT_H
	BITT W, ID, 1	;CHECK READ READY FLAG
	BRZ FGW_WAIT_H	;IF NOT SET KEEP WAITING
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	MOVE S, ID, AUX0
	RET
	MOVE H, AUX0, R11
;#############################################################################

;#############################################################################
FLASH_SEND_BYTE
	LIM H, AUX1, `HH FLASH_SET_BYTE_ADDR_READ
	LIM L, AUX1, `HL FLASH_SET_BYTE_ADDR_READ
	LIM H, AUX0, `LH FLASH_SET_BYTE_ADDR_READ
	LIM L, AUX0, `LL FLASH_SET_BYTE_ADDR_READ
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_HIGH
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_HIGH
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $00
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_LOW
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_LOW
	MOVE W, AUX0, IAL
	LIM H, AUX0, $0A
	LIM L, AUX0, $AA
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $AA
	MOVE W, AUX0, ID	;SEND WORD PROGRAM COMMAND

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FSW_WAIT_FIRST
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FSW_WAIT_FIRST	;IF NOT SET KEEP WAITING

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_HIGH
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_HIGH
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $00
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_LOW
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_LOW
	MOVE W, AUX0, IAL
	LIM H, AUX0, $05
	LIM L, AUX0, $55
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $55
	MOVE W, AUX0, ID	;SEND WORD PROGRAM COMMAND

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FSW_WAIT_SECOND
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FSW_WAIT_SECOND	;IF NOT SET KEEP WAITING

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_HIGH
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_HIGH
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $00
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_LOW
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_LOW
	MOVE W, AUX0, IAL
	LIM H, AUX0, $0A
	LIM L, AUX0, $AA
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $A0
	MOVE W, AUX0, ID	;SEND WORD PROGRAM COMMAND

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FSW_WAIT_THIRD
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FSW_WAIT_THIRD	;IF NOT SET KEEP WAITING

	LIM H, AUX1, `HH FLASH_SET_BYTE_ADDR_WRITE
	LIM L, AUX1, `HL FLASH_SET_BYTE_ADDR_WRITE
	LIM H, AUX0, `LH FLASH_SET_BYTE_ADDR_WRITE
	LIM L, AUX0, `LL FLASH_SET_BYTE_ADDR_WRITE
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	MOVE L, R0, ID	;SEND BYTE

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FSW_WAIT_DATA
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FSW_WAIT_DATA	;IF NOT SET KEEP WAITING

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
	MOVE W, ZERO, ID	;WRITE TO STATUS REGISTER TO INITIATE READ
FSW_WAIT_PREV
	BITT W, ID, 1		;CHECK READ READY FLAG
	BRZ FSW_WAIT_PREV	;IF NOT SET KEEP WAITING
	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	MOVE W, ID, AUX2	;READ STATUS (BIT 6) INTO AUX2

FSW_WAIT_BUSY
	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
	MOVE W, ZERO, ID	;WRITE TO STATUS REGISTER TO INITIATE READ
FSW_WAIT_STATUS
	BITT W, ID, 1		;CHECK READ READY FLAG
	BRZ FSW_WAIT_STATUS	;IF NOT SET KEEP WAITING
	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	MOVE W, ID, AUX0	;READ STATUS INTO AUX0
	XOR W, AUX2, AUX1	;COMPARE WITH PREV STATUS
	BITT W, AUX1, 6
	BRNZ FSW_WAIT_BUSY
	MOVE W, AUX0, AUX2	;UPDATE PREV STATUS

	RET
	NOP
;#############################################################################

;#############################################################################
FLASH_ERASE_BLOCK
	LIM H, AUX1, `HH FLASH_SET_BYTE_ADDR_READ
	LIM L, AUX1, `HL FLASH_SET_BYTE_ADDR_READ
	LIM H, AUX0, `LH FLASH_SET_BYTE_ADDR_READ
	LIM L, AUX0, `LL FLASH_SET_BYTE_ADDR_READ
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_HIGH
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_HIGH
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $00
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_LOW
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_LOW
	MOVE W, AUX0, IAL
	LIM H, AUX0, $0A
	LIM L, AUX0, $AA
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $AA
	MOVE W, AUX0, ID	;SEND ERASE BLOCK COMMAND

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FEB_WAIT_FIRST
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FEB_WAIT_FIRST	;IF NOT SET KEEP WAITING

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_HIGH
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_HIGH
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $00
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_LOW
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_LOW
	MOVE W, AUX0, IAL
	LIM H, AUX0, $05
	LIM L, AUX0, $55
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $55
	MOVE W, AUX0, ID	;SEND ERASE BLOCK COMMAND

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FEB_WAIT_SECOND
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FEB_WAIT_SECOND	;IF NOT SET KEEP WAITING

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_HIGH
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_HIGH
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $00
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_LOW
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_LOW
	MOVE W, AUX0, IAL
	LIM H, AUX0, $0A
	LIM L, AUX0, $AA
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $80
	MOVE W, AUX0, ID	;SEND ERASE BLOCK COMMAND

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FEB_WAIT_THIRD
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FEB_WAIT_THIRD	;IF NOT SET KEEP WAITING

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_HIGH
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_HIGH
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $00
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_LOW
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_LOW
	MOVE W, AUX0, IAL
	LIM H, AUX0, $0A
	LIM L, AUX0, $AA
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $AA
	MOVE W, AUX0, ID	;SEND ERASE BLOCK COMMAND

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FEB_WAIT_FOURTH
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FEB_WAIT_FOURTH	;IF NOT SET KEEP WAITING

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_HIGH
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_HIGH
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $00
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_LOW
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_LOW
	MOVE W, AUX0, IAL
	LIM H, AUX0, $05
	LIM L, AUX0, $55
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $55
	MOVE W, AUX0, ID	;SEND ERASE BLOCK COMMAND

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FEB_WAIT_FIFTH
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FEB_WAIT_FIFTH	;IF NOT SET KEEP WAITING

	LIM H, AUX1, `HH FLASH_SET_BYTE_ADDR_WRITE
	LIM L, AUX1, `HL FLASH_SET_BYTE_ADDR_WRITE
	LIM H, AUX0, `LH FLASH_SET_BYTE_ADDR_WRITE
	LIM L, AUX0, `LL FLASH_SET_BYTE_ADDR_WRITE
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM W, AUX0, $30
	MOVE L, AUX0, ID	;SEND ERASE COMMAND

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FEB_WAIT_DATA
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FEB_WAIT_DATA	;IF NOT SET KEEP WAITING

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
	MOVE W, ZERO, ID	;WRITE TO STATUS REGISTER TO INITIATE READ
FEB_WAIT_PREV
	BITT W, ID, 1		;CHECK READ READY FLAG
	BRZ FEB_WAIT_PREV	;IF NOT SET KEEP WAITING
	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	MOVE W, ID, AUX2	;READ STATUS (BIT 6) INTO AUX2

FEB_WAIT_BUSY
	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
	MOVE W, ZERO, ID	;WRITE TO STATUS REGISTER TO INITIATE READ
FEB_WAIT_STATUS
	BITT W, ID, 1		;CHECK READ READY FLAG
	BRZ FEB_WAIT_STATUS	;IF NOT SET KEEP WAITING
	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	MOVE W, ID, AUX0	;READ STATUS INTO AUX0
	XOR W, AUX2, AUX1	;COMPARE WITH PREV STATUS
	BITT W, AUX1, 6
	BRNZ FEB_WAIT_BUSY
	MOVE W, AUX0, AUX2	;UPDATE PREV STATUS

	RET
	NOP
;#############################################################################

;#############################################################################
FLASH_LOCK_BLOCK
	LIM H, AUX1, `HH FLASH_SET_BYTE_ADDR_READ
	LIM L, AUX1, `HL FLASH_SET_BYTE_ADDR_READ
	LIM H, AUX0, `LH FLASH_SET_BYTE_ADDR_READ
	LIM L, AUX0, `LL FLASH_SET_BYTE_ADDR_READ
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_HIGH
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_HIGH
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $00
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_LOW
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_LOW
	MOVE W, AUX0, IAL
	LIM H, AUX0, $0A
	LIM L, AUX0, $AA
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $AA
	MOVE W, AUX0, ID	;SEND PPB ENTRY COMMAND

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FLB_WAIT_ENTRY_FIRST
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FLB_WAIT_ENTRY_FIRST	;IF NOT SET KEEP WAITING

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_HIGH
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_HIGH
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $00
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_LOW
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_LOW
	MOVE W, AUX0, IAL
	LIM H, AUX0, $05
	LIM L, AUX0, $55
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $55
	MOVE W, AUX0, ID	;SEND PPB ENTRY COMMAND

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FLB_WAIT_ENTRY_SECOND
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FLB_WAIT_ENTRY_SECOND	;IF NOT SET KEEP WAITING

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_HIGH
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_HIGH
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $00
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_LOW
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_LOW
	MOVE W, AUX0, IAL
	LIM H, AUX0, $0A
	LIM L, AUX0, $AA
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $C0
	MOVE W, AUX0, ID	;SEND PPB ENTRY COMMAND

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FLB_WAIT_ENTRY_THIRD
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FLB_WAIT_ENTRY_THIRD	;IF NOT SET KEEP WAITING

	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $A0
	MOVE W, AUX0, ID	;SEND PPB PROGRAM COMMAND

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FLB_WAIT_PROGRAM_FIRST
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FLB_WAIT_PROGRAM_FIRST	;IF NOT SET KEEP WAITING

	LIM H, AUX1, `HH FLASH_SET_BYTE_ADDR_WRITE
	LIM L, AUX1, `HL FLASH_SET_BYTE_ADDR_WRITE
	LIM H, AUX0, `LH FLASH_SET_BYTE_ADDR_WRITE
	LIM L, AUX0, `LL FLASH_SET_BYTE_ADDR_WRITE
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM W, AUX0, $00
	MOVE L, AUX0, ID	;SEND PPB PROGRAM COMMAND

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FLB_WAIT_PROGRAM_SECOND
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FLB_WAIT_PROGRAM_SECOND	;IF NOT SET KEEP WAITING

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
	MOVE W, ZERO, ID	;WRITE TO STATUS REGISTER TO INITIATE READ
FLB_WAIT_PREV
	BITT W, ID, 1		;CHECK READ READY FLAG
	BRZ FLB_WAIT_PREV	;IF NOT SET KEEP WAITING
	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	MOVE W, ID, AUX2	;READ STATUS (BIT 6) INTO AUX2

FLB_WAIT_BUSY
	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
	MOVE W, ZERO, ID	;WRITE TO STATUS REGISTER TO INITIATE READ
FLB_WAIT_STATUS
	BITT W, ID, 1		;CHECK READ READY FLAG
	BRZ FLB_WAIT_STATUS	;IF NOT SET KEEP WAITING
	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	MOVE W, ID, AUX0	;READ STATUS INTO AUX0
	XOR W, AUX2, AUX1	;COMPARE WITH PREV STATUS
	BITT W, AUX1, 6
	BRNZ FLB_WAIT_BUSY
	MOVE W, AUX0, AUX2	;UPDATE PREV STATUS

	;COMMAND SET EXIT
	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $90
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FLB_WAIT_EXIT_FIRST
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FLB_WAIT_EXIT_FIRST	;IF NOT SET KEEP WAITING

	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $00
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FLB_WAIT_EXIT_SECOND
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FLB_WAIT_EXIT_SECOND	;IF NOT SET KEEP WAITING
	NOP

	RET
	NOP
;#############################################################################

;#############################################################################
FLASH_GET_N_WORDS
	;SET AUTO INCREMENT FOR READ
	LIM H, AUX0, `LH IOMM_READ_ADDR_INC
	LIM L, AUX0, `LL IOMM_READ_ADDR_INC
	MOVE W, AUX0, IAL
	LIM W, AUX0, $01
	MOVE W, AUX0, ID	;ENABLE AUTO INCREMENT

	;GET THE NUMBER OF WORDS
	LIM H, AUX1, `LH FLASH_NUM_WORDS_L
	LIM L, AUX1, `LL FLASH_NUM_WORDS_L
	MOVE W, AUX1, DAL
	MOVE W, DD, R3

	;SET STARTING ADDRESS IN MEMORY
	LIM H, AUX0, `LH BUFFER_START
	LIM L, AUX0, `LL BUFFER_START
	MOVE W, AUX0, R5

	;LOOP N TIMES
	TEST W, R3
	BRNZ FGNB_LOOP
	NOP
	RET

FGNB_LOOP
	;GET WORD
	LIM H, AUX1, `HH FLASH_GET_WORD
	LIM L, AUX1, `HL FLASH_GET_WORD
	LIM H, AUX0, `LH FLASH_GET_WORD
	LIM L, AUX0, `LL FLASH_GET_WORD
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	;WRITE WORD
	MOVE W, R5, DAL
	MOVE W, R11, DD

	;INCREMENT DATA ADDRESS
	LIM W, AUX0, $01
	ADD W, R5, R5

	;DECREMENT COUNTER
	LIM W, AUX0, $01
	SUB W, R3, R3

	;CHECK IF ZERO
	BRNZ FGNB_LOOP
	NOP

	RET
	NOP
;#############################################################################

;#############################################################################
FLASH_SEND_N_WORDS
	;GET THE NUMBER OF WORDS
	LIM H, AUX1, `LH FLASH_NUM_WORDS_L
	LIM L, AUX1, `LL FLASH_NUM_WORDS_L
	MOVE W, AUX1, DAL
	MOVE W, DD, R3

	;SET STARTING ADDRESS IN MEMORY
	LIM H, AUX0, `LH BUFFER_START
	LIM L, AUX0, `LL BUFFER_START
	MOVE W, AUX0, R5

	;LOOP N TIMES
	TEST W, R3
	BRNZ FSNB_LOOP
	NOP
	RET

FSNB_LOOP
	;GET WORD
	MOVE W, R5, DAL
	MOVE W, DD, R6

	;WRITE LOW BYTE
	LIM H, AUX1, `HH FLASH_SEND_BYTE
	LIM L, AUX1, `HL FLASH_SEND_BYTE
	LIM H, AUX0, `LH FLASH_SEND_BYTE
	LIM L, AUX0, `LL FLASH_SEND_BYTE
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	MOVE W, R6, R0
	CALL
	NOP

	;INCREMENT ADDRESS
	LIM H, AUX0, `LH FLASH_BYTE_ADDR_H		
	LIM L, AUX0, `LL FLASH_BYTE_ADDR_H
	MOVE W, AUX0, DAL
	MOVE W, DD, AUX1

	LIM H, AUX0, `LH FLASH_BYTE_ADDR_L
	LIM L, AUX0, `LL FLASH_BYTE_ADDR_L
	MOVE W, AUX0, DAL
	MOVE W, DD, AUX2

	LIM W, AUX0, $01
	ADD W, AUX2, AUX2
	LIM W, AUX0, $00
	ADDC W, AUX1, AUX1

	LIM H, AUX0, `LH FLASH_BYTE_ADDR_H		
	LIM L, AUX0, `LL FLASH_BYTE_ADDR_H
	MOVE W, AUX0, DAL
	MOVE W, AUX1, DD

	LIM H, AUX0, `LH FLASH_BYTE_ADDR_L
	LIM L, AUX0, `LL FLASH_BYTE_ADDR_L
	MOVE W, AUX0, DAL
	MOVE W, AUX2, DD

	;WRITE HIGH BYTE
	LIM H, AUX1, `HH FLASH_SEND_BYTE
	LIM L, AUX1, `HL FLASH_SEND_BYTE
	LIM H, AUX0, `LH FLASH_SEND_BYTE
	LIM L, AUX0, `LL FLASH_SEND_BYTE
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	MOVE S, R6, R0
	CALL
	NOP

	;INCREMENT ADDRESS
	LIM H, AUX0, `LH FLASH_BYTE_ADDR_H		
	LIM L, AUX0, `LL FLASH_BYTE_ADDR_H
	MOVE W, AUX0, DAL
	MOVE W, DD, AUX1

	LIM H, AUX0, `LH FLASH_BYTE_ADDR_L
	LIM L, AUX0, `LL FLASH_BYTE_ADDR_L
	MOVE W, AUX0, DAL
	MOVE W, DD, AUX2

	LIM W, AUX0, $01
	ADD W, AUX2, AUX2
	LIM W, AUX0, $00
	ADDC W, AUX1, AUX1

	LIM H, AUX0, `LH FLASH_BYTE_ADDR_H		
	LIM L, AUX0, `LL FLASH_BYTE_ADDR_H
	MOVE W, AUX0, DAL
	MOVE W, AUX1, DD

	LIM H, AUX0, `LH FLASH_BYTE_ADDR_L
	LIM L, AUX0, `LL FLASH_BYTE_ADDR_L
	MOVE W, AUX0, DAL
	MOVE W, AUX2, DD

	;INCREMENT DATA ADDRESS
	LIM W, AUX0, $01
	ADD W, R5, R5

	;DECREMENT COUNTER
	LIM W, AUX0, $01
	SUB W, R3, R3

	;CHECK IF ZERO
	BRNZ FSNB_LOOP
	NOP

	RET
	NOP
;#############################################################################

;#############################################################################
FLASH_UNLOCK_ALL_BLOCKS
	LIM H, AUX0, `LH IOMM_WRITE_ADDR_HIGH
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_HIGH
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $00
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_LOW
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_LOW
	MOVE W, AUX0, IAL
	LIM H, AUX0, $0A
	LIM L, AUX0, $AA
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $AA
	MOVE W, AUX0, ID	;SEND PPB ENTRY COMMAND

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FUB_WAIT_ENTRY_FIRST
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FUB_WAIT_ENTRY_FIRST	;IF NOT SET KEEP WAITING

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_HIGH
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_HIGH
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $00
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_LOW
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_LOW
	MOVE W, AUX0, IAL
	LIM H, AUX0, $05
	LIM L, AUX0, $55
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $55
	MOVE W, AUX0, ID	;SEND PPB ENTRY COMMAND

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FUB_WAIT_ENTRY_SECOND
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FUB_WAIT_ENTRY_SECOND	;IF NOT SET KEEP WAITING

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_HIGH
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_HIGH
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $00
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_LOW
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_LOW
	MOVE W, AUX0, IAL
	LIM H, AUX0, $0A
	LIM L, AUX0, $AA
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $C0
	MOVE W, AUX0, ID	;SEND PPB ENTRY COMMAND

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FUB_WAIT_ENTRY_THIRD
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FUB_WAIT_ENTRY_THIRD	;IF NOT SET KEEP WAITING

	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $80
	MOVE W, AUX0, ID	;SEND ALL PPB ERASE COMMAND

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FUB_WAIT_ERASE_FIRST
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FUB_WAIT_ERASE_FIRST	;IF NOT SET KEEP WAITING

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_HIGH
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_HIGH
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $00
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_WRITE_ADDR_LOW
	LIM L, AUX0, `LL IOMM_WRITE_ADDR_LOW
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $00
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM W, AUX0, $30
	MOVE L, AUX0, ID	;SEND ALL PPB ERASE COMMAND

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FUB_WAIT_ERASE_SECOND
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FUB_WAIT_ERASE_SECOND	;IF NOT SET KEEP WAITING

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
	MOVE W, ZERO, ID	;WRITE TO STATUS REGISTER TO INITIATE READ
FUB_WAIT_PREV
	BITT W, ID, 1		;CHECK READ READY FLAG
	BRZ FUB_WAIT_PREV	;IF NOT SET KEEP WAITING
	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	MOVE W, ID, AUX2	;READ STATUS (BIT 6) INTO AUX2

FUB_WAIT_BUSY
	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
	MOVE W, ZERO, ID	;WRITE TO STATUS REGISTER TO INITIATE READ
FUB_WAIT_STATUS
	BITT W, ID, 1		;CHECK READ READY FLAG
	BRZ FUB_WAIT_STATUS	;IF NOT SET KEEP WAITING
	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	MOVE W, ID, AUX0	;READ STATUS INTO AUX0
	XOR W, AUX2, AUX1	;COMPARE WITH PREV STATUS
	BITT W, AUX1, 6
	BRNZ FUB_WAIT_BUSY
	MOVE W, AUX0, AUX2	;UPDATE PREV STATUS

	;COMMAND SET EXIT
	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $90
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FUB_WAIT_EXIT_FIRST
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FUB_WAIT_EXIT_FIRST	;IF NOT SET KEEP WAITING

	LIM H, AUX0, `LH IOMM_MEM_DATA
	LIM L, AUX0, `LL IOMM_MEM_DATA
	MOVE W, AUX0, IAL
	LIM H, AUX0, $00
	LIM L, AUX0, $00
	MOVE W, AUX0, ID

	LIM H, AUX0, `LH IOMM_STATUS
	LIM L, AUX0, `LL IOMM_STATUS
	MOVE W, AUX0, IAL
FUB_WAIT_EXIT_SECOND
	BITT W, ID, 0		;CHECK WRITE READY FLAG
	BRZ FUB_WAIT_EXIT_SECOND	;IF NOT SET KEEP WAITING
	NOP

	RET
	NOP
;#############################################################################

;#############################################################################
FLASH_LOCK_ALL_BLOCKS
	;START FLASH ADDRESS AT ZERO
	LIM H, AUX1, `LH FLASH_BYTE_ADDR_H
	LIM L, AUX1, `LL FLASH_BYTE_ADDR_H
	MOVE W, AUX1, DAL
	MOVE W, ZERO, DD

	LIM H, AUX1, `LH FLASH_BYTE_ADDR_L
	LIM L, AUX1, `LL FLASH_BYTE_ADDR_L
	MOVE W, AUX1, DAL
	MOVE W, ZERO, DD

FLAB_LOOP
	LIM H, AUX1, `HH FLASH_LOCK_BLOCK
	LIM L, AUX1, `HL FLASH_LOCK_BLOCK
	LIM H, AUX0, `LH FLASH_LOCK_BLOCK
	LIM L, AUX0, `LL FLASH_LOCK_BLOCK
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	;GET FLASH ADDRESS IN R3, R4
	LIM H, AUX1, `LH FLASH_BYTE_ADDR_H
	LIM L, AUX1, `LL FLASH_BYTE_ADDR_H
	MOVE W, AUX1, DAL
	MOVE W, DD, R3

	LIM H, AUX1, `LH FLASH_BYTE_ADDR_L
	LIM L, AUX1, `LL FLASH_BYTE_ADDR_L
	MOVE W, AUX1, DAL
	MOVE W, DD, R4

	LIM H, AUX0, `LH HEX_INDICATORS
	LIM L, AUX0, `LL HEX_INDICATORS
	MOVE W, AUX0, IAL
	MOVE W, R3, ID

	;DETERMINE BLOCK SIZE
	;TEST W, R3
	BRNZ FLAB_BIG

	;INCREMENT BY 8KB
	LIM H, AUX0, $20
	LIM L, AUX0, $00
	ADD W, R4, R4
	MOVE W, ZERO, AUX0
	ADDC W, R3, R3
	BRA FLAB_UPDATE

FLAB_BIG
	LIM W, AUX0, $01
	ADD W, R3, R3		;INCREMENT BY 64KB

FLAB_UPDATE
	LIM H, AUX1, `LH FLASH_BYTE_ADDR_H
	LIM L, AUX1, `LL FLASH_BYTE_ADDR_H
	MOVE W, AUX1, DAL
	MOVE W, R3, DD

	LIM H, AUX1, `LH FLASH_BYTE_ADDR_L
	LIM L, AUX1, `LL FLASH_BYTE_ADDR_L
	MOVE W, AUX1, DAL
	MOVE W, R4, DD

	BITT W, R3, 6
	BRZ FLAB_LOOP
	NOP

	RET
	NOP
;#############################################################################

;#############################################################################
FLASH_ERASE_ALL_BLOCKS
	;START FLASH ADDRESS AT ZERO
	LIM H, AUX1, `LH FLASH_BYTE_ADDR_H
	LIM L, AUX1, `LL FLASH_BYTE_ADDR_H
	MOVE W, AUX1, DAL
	MOVE W, ZERO, DD

	LIM H, AUX1, `LH FLASH_BYTE_ADDR_L
	LIM L, AUX1, `LL FLASH_BYTE_ADDR_L
	MOVE W, AUX1, DAL
	MOVE W, ZERO, DD

FEAB_LOOP
	LIM H, AUX1, `HH FLASH_ERASE_BLOCK
	LIM L, AUX1, `HL FLASH_ERASE_BLOCK
	LIM H, AUX0, `LH FLASH_ERASE_BLOCK
	LIM L, AUX0, `LL FLASH_ERASE_BLOCK
	MOVE W, AUX1, CAH
	MOVE W, AUX0, CAL
	CALL
	NOP

	;GET FLASH ADDRESS IN R3, R4
	LIM H, AUX1, `LH FLASH_BYTE_ADDR_H
	LIM L, AUX1, `LL FLASH_BYTE_ADDR_H
	MOVE W, AUX1, DAL
	MOVE W, DD, R3

	LIM H, AUX1, `LH FLASH_BYTE_ADDR_L
	LIM L, AUX1, `LL FLASH_BYTE_ADDR_L
	MOVE W, AUX1, DAL
	MOVE W, DD, R4

	LIM H, AUX0, `LH HEX_INDICATORS
	LIM L, AUX0, `LL HEX_INDICATORS
	MOVE W, AUX0, IAL
	MOVE W, R3, ID

	;DETERMINE BLOCK SIZE
	;TEST W, R3
	BRNZ FEAB_BIG

	;INCREMENT BY 8KB
	LIM H, AUX0, $20
	LIM L, AUX0, $00
	ADD W, R4, R4
	MOVE W, ZERO, AUX0
	ADDC W, R3, R3
	BRA FEAB_UPDATE

FEAB_BIG
	LIM W, AUX0, $01
	ADD W, R3, R3		;INCREMENT BY 64KB

FEAB_UPDATE
	LIM H, AUX1, `LH FLASH_BYTE_ADDR_H
	LIM L, AUX1, `LL FLASH_BYTE_ADDR_H
	MOVE W, AUX1, DAL
	MOVE W, R3, DD

	LIM H, AUX1, `LH FLASH_BYTE_ADDR_L
	LIM L, AUX1, `LL FLASH_BYTE_ADDR_L
	MOVE W, AUX1, DAL
	MOVE W, R4, DD

	BITT W, R3, 6
	BRZ FEAB_LOOP
	NOP

	RET
	NOP
;#############################################################################
