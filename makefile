# Project name
PROJECT = life_sym
# Used compiler
CC = gcc
# Directories
SRC= src/
LIB= lib/
BUILD= build/
BIN= bin/
# Necessary libraries
LIBS = -lm

# Compiler flags based on debug mode
# CFLAGS = -Wall -Wextra -Werror -std=gnu99 -pedantic -Wno-missing-field-initializers
DEBUG ?= 0
ifeq ($(DEBUG), 1)
    CFLAGS = -Wall -std=gnu99 -DDEBUG -g
    PROJECT = life_sym_debug
else
    CFLAGS = -Wall -std=gnu99
endif

.PHONY: all clean

all: $(PROJECT)

$(PROJECT): $(BUILD)main.o $(BUILD)misc.o $(BUILD)gen_sym.o $(BUILD)netting.o $(BUILD)png.o $(BUILD)rules.o $(BUILD)argparse.o
		$(CC) $(CFLAGS) $^ -o $(BIN)$(PROJECT) $(LIBS)
		@echo "\nmake completed successfully"

$(BUILD)main.o: $(SRC)main.c
		$(CC) $(CFLAGS) $< -c -o $@ 

$(BUILD)misc.o: $(SRC)misc.c $(SRC)misc.h
		$(CC) $(CFLAGS) $< -c -o $@ 

$(BUILD)gen_sym.o: $(SRC)gen_sym.c $(SRC)gen_sym.h
		$(CC) $(CFLAGS) $< -c -o $@ 

$(BUILD)netting.o: $(SRC)netting.c $(SRC)netting.h
		$(CC) $(CFLAGS) $< -c -o $@ 

$(BUILD)png.o: $(SRC)png.c $(SRC)png.h
		$(CC) $(CFLAGS) $< -c -o $@ 

$(BUILD)rules.o: $(SRC)rules.c $(SRC)rules.h
		$(CC) $(CFLAGS) $< -c -o $@ 

$(BUILD)argparse.o: $(LIB)argparse.c $(LIB)argparse.h
		$(CC) $(CFLAGS) $< -c -o $@ 

clean: 
		-rm -f $(BUILD)*
		-rm -f $(BIN)*
		-rm -f -r results/
		@echo "\nall cleaned"
		