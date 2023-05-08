CC="C:\avr\bin\avr-gcc"
LD="C:\avr\bin\avr-ld"
OBJCOPY="C:\avr\bin\avr-objcopy"
OBJDUMP="C:\avr\bin\avr-objdump"
AVRSIZE="C:\avr\bin\avr-size"
OBJISP="C:\avr\bin\avrdude"
MCU=atmega328p
CFLAGS=-Wall -Wextra  -Wundef -pedantic \
		-I C:\avr\avr\include \
		-Os -std=gnu99 -DF_CPU=16000000UL -mmcu=${MCU} -DBAUD=19200
PORT=\\\\.\\COM3
BIN=avrdemo
OUT=${BIN}.hex
SOURCES = main.c millis.c uart.c

DEBUG?=1

ifeq ($(DEBUG), 1)
	OUTPUTDIR=bin/debug
else
	OUTPUTDIR=bin/release
endif

all: $(OUTPUTDIR)  $(OUT) 


%.lss: %.elf
	$(OBJDUMP) -h -S -s $< > $@

%.elf: $(SOURCES)
	$(CC) -mmcu=atmega328p -o $@ $(CFLAGS) $(LDFLAGS)  $(SOURCES)
	$(AVRSIZE) $@


$(OBJS):$(SOURCES)

%.hex: %.elf
	$(OBJCOPY) -O ihex -R .fuse -R .lock -R .user_signatures -R .comment $< $@

isp: ${BIN}.hex
	$(OBJISP) -F -V -c arduino -p ${MCU} -P ${PORT} -U flash:w:$<


clean:
	del "$(OUT)"  *.map *.P *.d

$(OUTPUTDIR): 
	@mkdir "$(OUTPUTDIR)"
		   	
.PHONY: clean dirs