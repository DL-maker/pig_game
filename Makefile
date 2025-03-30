CC = gcc
CFLAGS = -W -Wall -Werror -std=c99
LDFLAGS = 

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

EXEC = $(BIN_DIR)/cochon

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: setup $(EXEC)

setup:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

obj/game.o: src/game.c
obj/%.o: src/%.c
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o

mrproper: clean
	rm -f $(EXEC)

rebuild: mrproper all

run: all
	$(EXEC)

.PHONY: all setup clean mrproper rebuild run