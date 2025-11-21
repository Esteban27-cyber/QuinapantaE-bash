CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = mini_bash
SRC = mini_bash.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET)
