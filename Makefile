CC ?= gcc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -pedantic
TARGET := 2048
SRC := $(wildcard src/*.c)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(TARGET) $(TARGET).exe