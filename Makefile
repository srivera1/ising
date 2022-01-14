.POSIX:
CC     = gcc -std=c99
CFLAGS = -s -Wall -Wextra -Ofast -march=native -fPIE
LDLIBS = -lm -lpng

ising:
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ src/img.c src/ising.c src/main.c $(LDLIBS)

clean:
	rm -f ising
