CC      = cc
CFLAGS  = -Wall -Wextra -std=c99 -I src $(shell pkg-config --cflags raylib)
LDFLAGS = $(shell pkg-config --libs raylib) -lm

SRC  = main.c $(wildcard src/**/*.c)
OBJ  = $(SRC:.c=.o)
BIN  = physics-test

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN)

.PHONY: all clean
