
CC=gcc
CFLAGS=-std=c99 -O3 -march=native -Wall -Werror

FILES=src/main.c src/events.c src/commands/ping.c

all: $(FILES)
	$(CC) $(CFLAGS) -o CountingBot $^ -pthread -ldiscord -lcurl
