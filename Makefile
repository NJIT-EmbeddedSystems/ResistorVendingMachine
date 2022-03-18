PORT=/dev/ttyACM0
MCU=atmega2560
INCLUDE_DIR=-Iinclude
ERROR_FLAGS=-Wall -Wextra -Werror -Wshadow -Wno-unused-parameter
CFLAGS=-g $(ERROR_FLAGS) $(INCLUDE_DIR) -mcall-prologues -mmcu=$(MCU) -Os
LDFLAGS=-Wl,-gc-sections -Wl,-relax
CC=avr-gcc
BUILD_DIR=build
SRC_FILES=$(wildcard src/*.c)
OBJECT_FILES=$(addprefix $(BUILD_DIR)/obj/,$(notdir $(SRC_FILES:.c=.o)))

PROJECT=main
PROJECT_BIN=$(BUILD_DIR)/$(PROJECT).bin
PROJECT_HEX=$(BUILD_DIR)/$(PROJECT).hex

all: clean $(PROJECT_BIN) $(PROJECT_HEX) 

clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR)/obj/%.o: src/%.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

$(PROJECT_BIN): $(OBJECT_FILES) | build
	$(CC) $(OBJECT_FILES) $(CFLAGS) $(LDFLAGS) -o $@

$(PROJECT_HEX): $(PROJECT_BIN)
	avr-objcopy -R .eeprom -O ihex $< $@

flash: $(PROJECT_HEX) 
	avrdude -v -D -p $(MCU) -b 115200 -c stk500v2 -P $(PORT) -F -U flash:w:$(PROJECT_HEX)

obj:
	mkdir -p $(BUILD_DIR)/obj

build:
	mkdir -p $(BUILD_DIR)
