# Layaal Khellah
# CS 333 Lab 3: Threads

CC = gcc
DEBUG = -g
DEFINES =

CFLAGS = $(DEBUG) -Wall -Wextra -Wshadow -Wunreachable-code -Wredundant-decls \
		-Wmissing-declarations -Wold-style-definition \
		-Wmissing-prototypes -Wdeclaration-after-statement \
		-Wno-return-local-addr -Wunsafe-loop-optimizations -Wuninitialized -Werror \
		-Wno-unused-parameter
PROG1 = thread_crypt
INCLUDES = thread_crypt.h

TAR_FILE = ${LOGNAME}_$(PROG).tar.gz

PROGS = $(PROG1)

all: $(PROG1)

$(PROG1): $(PROG1).o 
	$(CC) $(CFLAGS) -o $@ $^

$(PROG1).o: $(PROG1).c $(INCLUDES)
	$(CC) $(CFLAGS) -c $<

cls clean:
	-rm -f $(PROGS) *.o *~ \#*

ci:
	if [ ! -d RCS ] ; then mkdir RCS; fi
	ci -t-none -l -m"lazy commit" [Mm]akefile *.[ch]

git get gat:
	if [ ! -d .git ] ; then git init; fi
	git add *.[ch] ?akefile
	git commit -m"git on with it"
 
# to push it out to an offsite repo
# git push 


tar:
	rm -f $(TAR_FILE)
	tar cvzf $(TAR_FILE) *.[ch] [Mm]akefile

