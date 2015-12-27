FORMAT = ihex
MCU = attiny45
AVRDUDE_MCU = t45
CC = avr-gcc

F_CPU = 8000000UL
GIT_VERSION = $(shell git log --max-count=1 --oneline | head -c7)

CFLAGS += -Wall -g -Os -mmcu=$(MCU) -std=gnu99 -pedantic-errors -D F_CPU=$(F_CPU) -D GIT_VERSION=\"$(GIT_VERSION)\"
LDFLAGS +=
OBJCOPY = avr-objcopy
FUSES = -U lfuse:w:0x63:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
PROGRAMMER = usbasp

TARGET = tinytarget

SOURCES = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, ${SOURCES})

.PHONY: all program fuses clean

all: $(TARGET).hex

program: $(TARGET).hex
	avrdude -p $(AVRDUDE_MCU) -c $(PROGRAMMER) -U flash:w:$(TARGET).hex

fuses:
	avrdude -p $(AVRDUDE_MCU) -c $(PROGRAMMER) $(FUSES)


$(TARGET).elf: $(OBJS)
	$(CC) -mmcu=$(MCU) -o $@ $(OBJS)

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	-rm -f $(TARGET).hex $(TARGET).elf $(OBJS)
