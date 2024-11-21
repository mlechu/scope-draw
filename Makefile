PORT=/dev/cu.usbserial-A60276HU # osepp uno
# PORT=/dev/cu.usbmodem101 # real uno
ELF=sd

all: $(ELF) flash

clean:
	rm $(ELF)

flash: $(ELF)
	avrdude -c arduino -p atmega328p -P $(PORT) -U "flash:w:$<:e"

$(ELF): sd.c
	avr-gcc -Wall -Wextra -Os -mmcu=atmega328p $< -o $@

.phony: all clean flash
