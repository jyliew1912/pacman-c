CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
TARGET = pacman

all: $(TARGET)

$(TARGET): pacman.c
	$(CC) $(CFLAGS) pacman.c -o $(TARGET) -lm

clean:
	rm -f $(TARGET) $(TARGET).exe *.o