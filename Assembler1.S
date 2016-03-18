#include <avr/io.h>             // Defines I/O port aliases
#define _SFR_ASM_COMPAT 1  /* Not sure when/if this is needed */

.extern my_C_function ;void my_C_function(int i);
.extern usb_serial_putchar_nowait ;int8_t usb_serial_putchar_nowait(uint8_t c);


#define temp R15
#define	length R16					
#define ckey R17
#define zero R18

.section .text                  ; Defines a code section

.global eann    ; Makes send_to_portb_in_asm visible in other source files
	;turn on port E6  
	eann:
	    out  0xE, 6
	    ret


.global eaus    ; Makes send_to_portb_in_asm visible in other source files    
	;turn off port E6  
	eaus:
	    out  0xE, 0
	    ret
    

.global add_two

	add_two:
		ADD R24,R22		;val_1 = val_1 + val_2
		CLR	R25			;return val_1
		ret
		
;extern char crypt (char *text, char *key ,len r21); 	
;first parameter in r25:r24
;second parameter in r22:r23.		
;Registers assignment
;Z register R30 and R31
;Y register R28 and R29
;X register R26 and R27

.global crypt
	crypt:	
		
		cli; 
	
		;push XL	;text
		;push XH
		
		;push ZL ;key
		;push ZH
		
		;push	temp 				// Temp register
		;push	ckey
		;push 	zero				//für stringende
		
		ldi zero, 0 ;load zero in r18, used as stringe end detection 
			
		;load pointer for first string (text)
		MOV   XL, R24  
		MOV   XH, R25
	
		;load pointer for second string  (key)
		MOV  ZL, R22
		MOV  ZH, R23 		
	
		MOV length ,r20
	
	
		;debug
			MOV   R25, XH 
			MOV   R24, XL  
			call my_C_function ;call c function	
			
			
			MOV   R25, ZH  
			MOV   R24, ZL  
			call my_C_function ;call c function				
		;debug	
	
		;a post-check loop
		loop:
			tst length ;test if zero
			breq loopend ; wenn 0, dann zu loopend
			dec length
			
			CLR   R25 
			MOV   R24, length  
			call my_C_function ;call c function
			
			;load the next char from flash
			LDS temp, X ;load text
			
			
			CLR   R25 
			MOV   R24, temp  
			call my_C_function ;call c function
			
			tst temp ;compare to zero
			breq loopend ;exit if zero
			
			;reatched here temp is not zero
			ld ckey, Z+ ;load key and increment after
					
			eor temp,ckey ; Bitwise exclusive or between temp and key, stored key
			
			st X+, temp ; store new value in X and increment after
			rjmp loop
		loopend:
			sei;enable interrupts again
			ret