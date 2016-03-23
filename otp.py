#!/usr/bin/env python2
import sys, time

def strxor(text, key):     # xor two strings of different lengths a text, b kex
		return "".join([chr(ord(x) ^ ord(y)) for (x, y) in zip(text,key)])
		
def main():
	print(time.ctime(), 'Program started.')
	print("lade Klartext")
	textfile = open('text.txt','rb') #readonly, binary
	klartext= textfile.read()
	print("lade Keyfile")
	keyfile = open('key.txt','rb') #readonly, binary
	key = keyfile.read()
	
	if len(klartext) > len(key) :
		print("Key zu kurz")
		return
		
	out = strxor(klartext,key);
	
	outfile = open('out.txt','w') #readoverwite, binary
	outfile.write(out)
	
if __name__ == "__main__":
	sys.exit(main())