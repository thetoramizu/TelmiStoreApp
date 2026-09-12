CC=arm-linux-gnueabihf-gcc

CFLAGS=-Iinclude

LDFLAGS=-Llibs -lSDL2

TARGET=telmistore

SRC=main.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)