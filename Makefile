#CC           = avr-gcc
#CFLAGS       = -Wall -mmcu=atmega16 -Os -Wl,-Map,desafio.map
#OBJCOPY      = avr-objcopy
CC           = gcc
CFLAGS       = -Wall -Os -fopenmp -Wl,-Map,desafio.map
OBJCOPY      = objcopy

# include path to AVR library
INCLUDE_PATH = /usr/lib/avr/include
# splint static check
SPLINT       = splint desafio.c aes.c -I$(INCLUDE_PATH) +charindex -unrecog

.SILENT:
.PHONY:  lint clean


rom.hex : desafio.out
	# copy object-code to new image and format in hex
	$(OBJCOPY) -j .text -O ihex desafio.out rom.hex

desafio.o : desafio.c
	# compiling desafio.c
	$(CC) $(CFLAGS) -c desafio.c -o desafio.o

aes.o : aes.h aes.c
	# compiling aes.c
	$(CC) $(CFLAGS) -c aes.c -o aes.o

desafio.out : aes.o desafio.o
	# linking object code to binary
	$(CC) $(CFLAGS) aes.o desafio.o -o desafio.out

small: desafio.out
	$(OBJCOPY) -j .text -O ihex desafio.out rom.hex

clean:
	rm -f *.OBJ *.LST *.o *.gch *.out *.hex *.map

lint:
	$(call SPLINT)
