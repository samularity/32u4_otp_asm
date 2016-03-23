#include <avr/io.h>        // Defines I/O port aliases
#define _SFR_ASM_COMPAT 1  // Not sure when/if this is needed

.section .text   ;definiert a code section

;avr asm referenzmanual http://www.atmel.com/Images/doc1022.pdf
;mix c and asm code http://www.atmel.com/images/doc42055.pdf

;extern char crypt (char *text, char *key); 	
;text parameter in r25:r24
;key parameter in r22:r23		
;Registers assignment
;Z register R30 and R31
;Y register R28 and R29
;X register R26 and R27

.global crypt
crypt:
     movw	r30, r24 ;kopiere register paar (ins Z register)
     ld	    r25, Z ;lade zeichen aus dem array
     tst	r25  ;prüfe ob zeichen '\0'
     breq	ende ; goto ende wenn '\0'
     mov	r26, r22 ;copy r22 nach r26 (ins X register)
     mov	r27, r23 ;copy r23 nach r27 (ins X register)
     loop:
        ld	r24, X+  ;lade key und inkrementiere danach
        eor	r25, r24 ;bitweise xor text^key, gespeicher in r25
        st	Z+, r25  ;speichere r25 in z und inkrementiere danach
        ld	r25, Z   ;lade den nächsten text und vergliche auf stringende
        cpse	r25, r1 ;r1 ist immer NULL, vergleiche und überspringe nächsten befehl wenn r25 Null 
        rjmp	loop ; nochmal
        ret
ende:
    ret