PORT=/dev/cu.usbserial-A60276HU # osepp uno
# PORT=/dev/cu.usbmodem101 # real uno
ELF=sd

all: $(ELF) flash

clean:
	rm $(ELF)

flash: $(ELF)
	avrdude -c arduino -p atmega328p -P $(PORT) -U "flash:w:$<:e"

$(ELF): sd.c
	avr-gcc -Wall -Wextra -mmcu=atmega328p $< -o $@
# -DF_CPU=16000000UL ?

.phony: all clean flash
