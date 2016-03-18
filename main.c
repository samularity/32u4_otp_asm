#include <avr/io.h>
#include <avr/power.h>
#include <util/delay.h>
#include <string.h>

#include "usb_serial.h"

extern void eann(void);
extern void eaus(void);
extern char add_two(char, char);
extern char crypt (void *text, void *key,char len); 

void my_C_function(int i);

void sleep_ms(uint16_t ms);

int main() {
	// internen clock teiler deaktivieren
	clock_prescale_set(clock_div_1);
 	MCUCR = (1 << JTD);					//jtag ausschalten
	MCUCR = (1 << JTD);					//this need to be executed twice 
		

	DDRE |= (1 << PE6);             // pin 6 von port E als output schalten
	DDRE &= ~(1 << DDE2);           // Pin E2 als eingang
	

	usb_init(); 
	sleep_ms(250);

	uint16_t counter = 0;
	char inputbuffer[250]={0};

	char text[50] = {1,2,3,4,5,6,7,8,9,10,'\0' };
	char key[50] =  {11,12,13,14,15,16,17,18,19,20,'\0' };
 
	char *ptr_txt = text;
	char *ptr_key = key;
	 
	while(1) {      
		
		if (usb_serial_available())
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
			
			if ( (counter>= 20) || (inputbuffer[counter-1]==13) ) //sind 20 zeichern erreicht oder enter ger�ckt
			{
				inputbuffer[counter-1]='\0'; //enter entfernen und string terminieren
				usb_send_str(inputbuffer);
				
				
				if (strncmp(inputbuffer,"cry",3)==0)
				{  
					
					usb_send_str("\r\n*txt:");
					usb_send_int((int)ptr_txt);
					usb_send_str("\r\n*kex:");
					usb_send_int((int)ptr_key);
					usb_send_str("\r\nstart\r\n");
					usb_serial_flush_output();
					crypt(text,key,5);
				}
				
				usb_send_str("\r\nText:");
				usb_send_str(ptr_txt);				
				usb_send_str("\r\nKey:");
				usb_send_str(ptr_key);
				usb_send_str("\r\n");

				
				memset	(&inputbuffer,0,250);//memset fills array &inputbuffer with 0's
				counter =0; //z�hler zur�cksetzten string soll wieder ab 0 gef�llt werden
				
	
				}//if stringende 
	
	
		}//if usbserial availab�le	
		// endlosschleife f�rs blinken
		_delay_ms(50);             // warte 500ms
		PORTE^=(1<<PINE6);	
	}
	return 0;
}

void sleep_ms(uint16_t ms){
	while(ms){
		ms--;
		_delay_ms(1);
	}
}

void my_C_function(int i)
{	
	asm volatile ("sei");
	usb_send_str("\r\nasm:");
	usb_send_int(i);
	usb_serial_flush_output();
	asm volatile ("cli");
}