
PROGRAM_NAME="CountingBot"

CC=gcc
CFLAGS=-std=c17 -O3 -march=native -Wall

FILES=$(shell find ./src -name '*.c')

all: $(FILES)
	$(CC) $(CFLAGS) -o $(PROGRAM_NAME) $^ -pthread -ldiscord -lcurl

clean:
	$(RM) $(PROGRAM_NAME)
