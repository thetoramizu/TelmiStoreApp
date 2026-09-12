CC=arm-linux-gnueabihf-gcc

CFLAGS=-I/usr/include/SDL2

LDFLAGS=-lSDL2

TARGET=telmistore

SRC=main.c


all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)


clean:
	rm -f $(TARGET)