CC=arm-linux-gnueabihf-gcc

CFLAGS=-Iinclude

LDFLAGS=-Llibs \
-lSDL2

all:
	$(CC) $(CFLAGS) main.c -o telmistore $(LDFLAGS)

clean:
	rm -f telmistore