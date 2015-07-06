CC=gcc
CFLAGS = -g 
# uncomment this for SunOS
# LIBS = -lsocket -lnsl

all: udp-cliente udp-servidor

udp-cliente: udp-cliente.o 
	$(CC) -o udp-cliente udp-cliente.o $(LIBS)

udp-servidor: udp-servidor.o 
	$(CC) -o udp-servidor udp-servidor.o $(LIBS)

udp-cliente.o: udp-cliente.c port.h

udp-servidor.o: udp-cliente.c port.h

clean:
	rm -f udp-cliente udp-servidor udp-cliente.o udp-servidor.o 
