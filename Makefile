CC = gcc
CFLAGS = -g -Wall -lm

all: exercicio1a exercicio1b exercicio1c exercicio1d exercicio2a exercicio2b exercicio2c

run: all
	./exercicio1a
	./exercicio1b
	./exercicio1c
	./exercicio1d
	./exercicio2a
	./exercicio2b
	./exercicio2c

exercicio1a: exercicio1a.c 
		$(CC) $(CFLAGS) exercicio1a.c -o exercicio1a

exercicio1b: exercicio1b.c 
		$(CC) $(CFLAGS) exercicio1b.c -o exercicio1b

exercicio1c: exercicio1c.c 
		$(CC) $(CFLAGS) exercicio1c.c -o exercicio1c

exercicio1d: exercicio1d.c 
		$(CC) $(CFLAGS) exercicio1d.c -o exercicio1d

exercicio2a: exercicio2a.c
		$(CC) $(CFLAGS) exercicio2a.c -o exercicio2a

exercicio2b: exercicio2b.c
		$(CC) $(CFLAGS) exercicio2b.c -o exercicio2b

exercicio2c: exercicio2c.c
		$(CC) $(CFLAGS) exercicio2c.c -o exercicio2c

clean:
	rm -f *.o
