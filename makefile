CC = gcc
DEFINES =
DEBUG = -g3 -O0
WERROR =
WERROR = -Werror
CFLAGS = -Wall -Wshadow -Wunreachable-code -Wredundant-decls \
 -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes \
 -Wdeclaration-after-statement -Wextra -Werror -Wpedantic \
 $(WERROR) $(DEBUG) $(DEFINES)
sieve = sieve.c


TARGETS=sieve sieve.o 

all: $(TARGETS)

sieve: $(sieve)
	$(CC) $(sieve) $(CFLAGS) -o $@ -lm

clean:
	rm -f $(TARGETS)

tar:
	tar cvfa quesmith_Lab3.tar.gz *.[ch] makefile

git checkin:
	if [ ! -d .git ] ; then git init; fi
	git add *.[ch] ?akefile
	git commit -m "comment"
	git push