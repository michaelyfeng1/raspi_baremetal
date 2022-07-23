C_SRC := $(shell find . -name "*.c") 
ASM_SRC := $(shell find . -name "*.S")

C_OBJ := $(patsubst %.c, %.o, $(C_SRC))
ASM_OBJ := $(patsubst %.S, %.o, $(ASM_SRC))

RPI_VERSION ?= 3

INCLUDES := -I ./include

CC := aarch64-none-elf-gcc
CASM := aarch64-none-elf-as

CFLAG := -c -g -O0 -DRPI_VERSION=$(RPI_VERSION)\
			-nostdlib -nostartfiles \
			-ffreestanding -mgeneral-regs-only

OUTPUT_DIR = ./obj
BIN_DIR = ./bin
ELF_FILE = kernel8.elf
BIN_FILE = kernel8.img

DEF_FILES = $(C_OBJ:%.o=%.d)
DEF_FILES += $(ASM_OBJ:%.o=%.d)
-include $(DEF_FILES)

.PHONY: build link run clean

%.o:%.c
	$(CC) $(CFLAG) $(INCLUDES) -MMD $< -o $@

%.o:%.S
	$(CC) -c -g2 -O0 $(INCLUDES) -MMD $< -o $@

build: $(ASM_OBJ) $(C_OBJ)

copy:
	aarch64-none-elf-ld -nostdlib  -T linker.ld $(ASM_OBJ) $(C_OBJ) -o $(ELF_FILE)
	aarch64-none-elf-objcopy $(ELF_FILE) --strip-all -O binary  $(BIN_FILE)

qemu:
	qemu-system-aarch64 \
		-M raspi3b	\
		-m 1024M	\
		-kernel kernel8.img		\
		-serial stdio 
		

all: build copy

dump:
	aarch64-linux-gnu-objdump -D $(ELF_FILE) > a.asm

clean:
	rm ./src/boot/*.o
	rm ./src/device/*.o
	rm ./src/utils/*.o
	rm ./src/interrupt/*.o
	rm ./src/fs/*.o
	rm ./src/mm/*.o


	rm ./src/boot/*.d
	rm ./src/device/*.d
	rm ./src/utils/*.d
	rm ./src/interrupt/*.d
	rm ./src/fs/*.d
	rm ./src/mm/*.d

	rm *.o
	rm *.d
	rm *img
	rm *elf