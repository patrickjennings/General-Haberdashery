CFLAGS:=-O2 -Wall -Werror -march=native -mtune=native
LIBS:=-lpthread -lgmp
CC:=gcc
PROJECT:=machin_pi

all: machin

machin:
	$(CC) $(CFLAGS) $(LIBS) -o $(PROJECT) machin_pi.c

chudnovsky:
	$(CC) $(CFLAGS) -lm -lgmp -o chudnovsky_pi gmp-chudnovsky.c

clean:
	rm -f machin_pi chudnovsky_pi
