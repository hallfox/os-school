CC=gcc
CFLAGS=-c -Wall -g
LDFLAGS=

# Per assignment config
SRC=main.c
BIN=lab1

OBJ=$(SRC:.c=.o)

all: $(SRC) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o $(BIN)
