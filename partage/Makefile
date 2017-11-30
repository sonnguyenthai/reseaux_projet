# Makefile (règles implicites)

CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: echoclient6 echoserveur6 echoclient echoserveur tunalloc


echoclient6: echoclient6.c
	$(CC) $(CFLAGS) echoclient6.c -o echoclient6

echoserveur6: echoserveur6.c
	$(CC) $(CFLAGS) echoserveur6.c -o echoserveur6

echoclient: echoclient.c
	$(CC) $(CFLAGS) echoclient.c -o echoclient

echoserveur: echoserveur.c
	$(CC) $(CFLAGS) echoserveur.c -o echoserveur

tunalloc: tunalloc.o
	$(CC) $(CFLAGS) tunalloc.c -o tunalloc

clean:
	rm *.o echoclient6 echoserveur6 echoclient echoserveur tunalloc