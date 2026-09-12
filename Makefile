CC=arm-linux-gnueabihf-gcc

CFLAGS=-Iinclude

LDFLAGS= \
-Llibs \
-Wl,--allow-shlib-undefined \
-Wl,-rpath-link,libs \
-lSDL2

TARGET=telmistore

SRC=main.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)