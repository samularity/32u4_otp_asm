//////////////////////////////////////
//      Includes                    //
//////////////////////////////////////
#include <avr/io.h>
#include <avr/power.h>
#include <util/delay.h>
#include <string.h>

#include "usb_serial.h"
#include "millis.h"

#define STR_LEN 250  
//////////////////////////////////////
//      Function Protoypes          //
//////////////////////////////////////
extern void crypt (void *text, void *key);  //prototyp für externe asm funktion
void printInfo (char* text, char *key);

//////////////////////////////////////
//      Main Program                //
//////////////////////////////////////
int main() {
	// internen clock teiler deaktivieren
	clock_prescale_set(clock_div_1);
 	MCUCR = (1 << JTD);					//jtag ausschalten
	MCUCR = (1 << JTD);					//muss 2x direkt hintereinander ausgeführt werden 
	
	DDRE |= (1 << PE6);             // pin 6 von port E als output schalten
	DDRE &= ~(1 << DDE2);           // Pin E2 als eingang
	
    timer_init();
	usb_init(); 
	
	uint16_t counter = 0; //zähle per usb empfangene zeichen
	char inputbuffer[STR_LEN]={'\0'};// speicher für empfangene zeichen
 
    
 	char text[100] = {"Hello World 123" }; //klartext zum verschlüsseln oder verschlüsselter text zum entschlüsseln
    char key[100] =  {"js,jfhalkfsdfjs324r87wzrshif"}; //key zum ver oder entschlüsseln
    
	while(1) {   //loop forever   
		
		if (usb_serial_available()) //usb seriell zeichen empfangen ?
		{
			inputbuffer[counter]= usb_serial_getchar();
			usb_serial_putchar(inputbuffer[counter]);
			usb_serial_flush_input();
			counter++;
		
        	if (inputbuffer[counter-1]==127)// 127 = del-?  console sagt 127=backspace  auf jeden fall löschen...
			{
					if (counter>1)//counter is mind 2 D delete 2 chars from string
						counter-=2; //set couter postion -2  (1 for char to delet an one for delet char)
					else
						counter=0;
			}
			
			if ( (counter>=STR_LEN) || (inputbuffer[counter-1]==13) ) //sind 20 zeichern erreicht oder enter ger�ckt
			{
				inputbuffer[counter-1]='\0'; //enter entfernen und string terminieren
                inputbuffer[counter]='\0'; //enter entfernen und string terminieren
				usb_send_str(inputbuffer);
				
				if (strncmp(inputbuffer,"cry",3)==0)
				{  
                    printInfo (text, key);
					crypt(text,key);
                }
                
                 else if (strncmp(inputbuffer,"key:",4)==0)
                { 
                    if ( strlen(text) <= strlen(inputbuffer+4) ) //key is longer than text
                        {
                        memset	(key,'\0',100); 
                        memcpy	(key, inputbuffer+4,strlen(inputbuffer+4) );
                        }
                    else 
                        usb_send_str("\r\nKey zu kurz!\r\n"); 
                }
                
                else if (strncmp(inputbuffer,"text:",5)==0)
                {
                    if ( strlen(key) >= strlen(inputbuffer+5) )
                    {
                       memset	(text,'\0',100); 
                       memcpy	(	text, inputbuffer+5 , strlen(inputbuffer+5) ); //memcopy inputbuffer+4 -> key	
                    }
                    else 
                       usb_send_str("\r\nKey zu kurz!\r\n");  
                }
                
                
                
                else if ( (inputbuffer[0]=='?') || (strncmp(inputbuffer,"help",4)==0))
                {
                    usb_send_str("\r\n==========================================");
                    usb_send_str("\r\n\tHilfe");
                    usb_send_str("\r\ncry\t\t-xor text und key, stored in key");
                    usb_send_str("\r\ntext:\t\t-set new text");
                    usb_send_str("\r\nkey:\t\t-set new key");
                    usb_send_str("\r\nhelp\t\t-zeige hilfe\r\n");
                    usb_send_str("==========================================");
                }   
                          
                printInfo (text, key);   
				memset	(&inputbuffer,'\0',STR_LEN);//memset fills array &inputbuffer with 0's
				counter =0; //z�hler zur�cksetzten string soll wieder ab 0 gef�llt werden
				}//if stringende 
		}//if usbserial availab�le	
		PORTE^=(1<<PINE6);	
	}// endlosschleife f�rs blinken
	return 0;
}


void printInfo (char* text, char *key){
    usb_send_str("\r\nText:");
    usb_send_str(text);				
    usb_send_str("\r\nKey:");
    usb_send_str(key);
    usb_send_str("\r\n");
    usb_serial_flush_output();
}