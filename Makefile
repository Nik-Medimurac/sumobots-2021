# ==============================================================================
# Sumobots 2021 Makefile
# ==============================================================================

# ------------------------------------------------------------------------------
# Project Files and Directories
# ------------------------------------------------------------------------------

# Name of the .c file with your main() function
MAIN 		:= main.c

# Other .c files that are used in the project
INCLUDES	:= line_sensor.c motor.c

# Project Directories
SRC			:= src
OBJ 		:= obj
BIN 		:= bin

# Generate target name to name the .elf and .hex files intuitively
TARGET		:= $(basename $(MAIN))

# Generate list of object files
OBJECTS     := $(patsubst %.c,$(OBJ)/%.o,$(INCLUDES))

# ------------------------------------------------------------------------------
# Device Settings
# ------------------------------------------------------------------------------

PORT	  	:= /dev/ttyUSB0

DEVICE     	:= atmega328p
PROGRAMMER 	:= arduino
CLOCK      	:= 16000000UL
BAUDRATE   	:= 115200
FUSES 	   	:= -U lfuse:w:0xd0:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m

# ------------------------------------------------------------------------------
# Toolchain Settings
# ------------------------------------------------------------------------------

CC 			:= avr-gcc
CFLAGS  	:= -Wall -Werror -Wextra -O2 -D F_CPU=$(CLOCK) -mmcu=$(DEVICE)

AVRDUDE 	:= avrdude -c $(PROGRAMMER) -p $(DEVICE) -P $(PORT) -b $(BAUDRATE)

.PHONY: all flash fuse debug clean
.PRECIOUS: $(BIN)/%.elf $(OBJ)/%.o

# ------------------------------------------------------------------------------
# Default Rule - Compile Executable
# ------------------------------------------------------------------------------

all: $(BIN)/$(TARGET).hex

# ------------------------------------------------------------------------------
# Generate Object Files
# ------------------------------------------------------------------------------

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h | obj
	$(CC) $(CFLAGS) -o $@ -c $<

# ------------------------------------------------------------------------------
# Generate Executables
# ------------------------------------------------------------------------------

$(BIN)/%.elf: $(SRC)/$(MAIN) $(OBJECTS) | bin
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/%.hex: $(BIN)/%.elf
	avr-objcopy -j .text -j .data -O ihex $< $@
	avr-size --format=avr --mcu=$(DEVICE) $<

# ------------------------------------------------------------------------------
# Programming Targets
# ------------------------------------------------------------------------------

# Flash hex file to AVR
flash: all
	$(AVRDUDE) -U flash:w:$(BIN)/$(TARGET).hex:i

# Set AVR fuse bits - can only be done when programming with ISP
fuse:
	$(AVRDUDE) $(FUSES)

# ------------------------------------------------------------------------------
# Debug Targets
# ------------------------------------------------------------------------------

debug: CFLAGS += -DDEBUG -fstack-usage
debug: all

# ------------------------------------------------------------------------------
# Generate Missing Directories
# ------------------------------------------------------------------------------

obj:
	mkdir -p $@

bin:
	mkdir -p $@

# ------------------------------------------------------------------------------
# Clean project
# ------------------------------------------------------------------------------

clean:
	rm -rf $(BIN) $(OBJ)
