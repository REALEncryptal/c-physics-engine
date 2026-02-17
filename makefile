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

TEST_SRC = $(wildcard tests/*.c)
TEST_BIN = $(TEST_SRC:tests/%.c=tests/%)
TEST_CFLAGS = -Wall -Wextra -std=c99 -I src $(shell pkg-config --cflags cunit)
TEST_LDFLAGS = $(shell pkg-config --libs cunit) -lm

tests/%: tests/%.c
	$(CC) $(TEST_CFLAGS) $< -o $@ $(TEST_LDFLAGS)

test: $(TEST_BIN)
	@for t in $(TEST_BIN); do echo "=== $$t ===" && ./$$t; done

clean:
	rm -f $(OBJ) $(BIN) $(TEST_BIN)

.PHONY: all clean test
