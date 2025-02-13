CC=gcc

CFLAGS=-Wall -Wextra -std=c99

CHEADER=-Iheader_files

CLINK=-lc -lm

SRC=source_files/*.c

TARGET=image_editor

build: $(TARGET)

image_editor: $(SRC)
	$(CC) $(CHEADER) $(SRC) $(CFLAGS) -o $(TARGET) $(CLINK)

clean:
	rm -f $(TARGET)

.PHONY: clean
