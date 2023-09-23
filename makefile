PROG = test

SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
EXE = $(BIN_DIR)/$(PROG)

CC = cc
LDLIBS = -lX11 
CFLAGS = -O3 -Wall -Wextra -I/usr/include/X11

.PHONY: all run clean

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

run: $(EXE)
	$(EXE)

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)