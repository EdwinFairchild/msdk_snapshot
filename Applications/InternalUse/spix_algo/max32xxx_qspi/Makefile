TARGET=max32xxx_qspi
ENTRY=algo_write

OFFSET=0x20005000

%.o: %.s
	arm-none-eabi-as -mthumb -mcpu=cortex-m4 ${<} -o ${@}

%.o: %.c
	arm-none-eabi-gcc -mthumb -mcpu=cortex-m4 -Wa,-mimplicit-it=thumb -Os -Wall -c ${<} -o ${@}

%.bin: %.elf
	arm-none-eabi-objcopy -O binary ${<} ${@}

all: $(TARGET)_write_code.c

dump: $(TARGET).o
	arm-none-eabi-objdump -d -s ${<}

$(TARGET).elf: $(TARGET).o
	arm-none-eabi-ld -nostdlib -nostartfiles -Ttext $(OFFSET) --entry $(ENTRY) ${<} -o ${@}
	arm-none-eabi-size ${@}

$(TARGET)_write_code.c: $(TARGET).bin
	python bin2c.py ${<} ${@}

clean:
	rm -rf $(TARGET).bin $(TARGET).elf $(TARGET).o $(TARGET)_write_code.c
