# Layaal Khellah
# CS333
# PSU Fall 23
# Lab 1


CC = gcc
DEBUG = -g
DEFINES = 

CFLAGS = $(DEBUG) -Wall -Wshadow -Wunreachable-code -Wredundant-decls \
		-Wmissing-declarations -Wold-style-definition -Wmissing-prototypes \
		-Wdeclaration-after-statement -Wextra -Werror -Wpedantic

PROG1 = mystat
PROG2 = mywc
PROGS = $(PROG1) $(PROG2)

all: $(PROGS)

$(PROG1): $(PROG1).o
	$(CC) $(CFLAGS) -o $@ $^

$(PROG1).o: $(PROG1).c
	$(CC) $(CFLAGS) -c $<
 
$(PROG2): $(PROG2).o
	$(CC) $(CFLAGS) -o $@ $^ -lm

$(PROG2).o: $(PROG2).c
	$(CC) $(CFLAGS) -c $<

clean cls: 
	-rm -f $(PROGS) *.o *~ \#*

TAR_FILE = ${LOGNAME}_$(PROG).tar.gz
tar:
	rm -f $(TAR_FILE)
	tar cvzf $(TAR_FILE) *.[ch] [Mm]akefile
