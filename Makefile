CC = gcc

all: selserv selclie

selserv: selserv.c socklib.c
	$(CC) -o selserv selserv.c socklib.c

selclie: selclie.c
	$(CC) -o selclie selclie.c

clean:
	rm -rf selserv
	rm -rf selclie
