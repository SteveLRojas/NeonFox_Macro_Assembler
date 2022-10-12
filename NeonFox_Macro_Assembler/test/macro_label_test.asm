;macro label test for neonfox macro assembler

INCLUDE "NEONFOX.INC"

    org 0
    
    nop
    nop
    nop
    nop
    
    nop
    nop
    nop
    nop
    
    nop
    nop
    nop
    nop
    
    nop
    nop
    nop
    nop
    
    nop
    nop
    nop
    nop
    
    nop
    nop
    nop
    nop
    
    nop
    nop
    nop
    nop
    
    nop
    nop
    bra main
    nop
    
hello_str
	data 'H'
	data 'e'
	data 'l'
	data 'l'
	data 'o'
	data ' '
	data 'w'
	data 'o'
	data 'r'
	data 'l'
	data 'd'
	data '\0'
	
macro m_ldwi sel, word, register
    lim l, aux0, sel l word
    lim h, aux0, sel h word
    move w, aux0, register
endmacro m_ldwi

macro m_ldwlp address, register
    lim l, aux0, `ll address
    lim h, aux0, `lh address
    move w, aux0, dal
    lim l, aux0, `hl address
    lim h, aux0, `hh address
    move w, aux0, dah
    move w, dd, register
endmacro m_ldwlp
	
main
    m_ldwi `l, foo, cal
    m_ldwi `h, foo, cah
    nop
    bra main
    
foo
    m_ldwi `l, 1000, aux0
    add w, r1, r1
    ret
    nop
    
