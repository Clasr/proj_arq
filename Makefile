CC=gcc
CFLAGS = -g 
# uncomment this for SunOS
# LIBS = -lsocket -lnsl

all: udp-cliente udp-servidor

udp-send: udp-cliente.o 
	$(CC) -o udp-cliente udp-cliente.o $(LIBS)

udp-recv: udp-servidor.o 
	$(CC) -o udp-servidor udp-servidor.o $(LIBS)

udp-send.o: udp-cliente.c port.h

udp-recv.o: udp-cliente.c port.h

clean:
	rm -f udp-cliente udp-servidor udp-cliente.o udp-servidor.o 
