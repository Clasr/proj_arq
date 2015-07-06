/***************************************
*Trablaho de Arquitetura de computadores
*Servidor
*Clarice, Eduardo, Gustavo e Thales
*2015/1
**************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include "port.h"

#define BUFSIZE 2048

// B* 'e definido mo termios.h
#define BAUDRATE B9600
//#define MODEMDEVICE "/dev/ttyUSB1"

#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define FALSE 0
#define TRUE 1

volatile int STOP=FALSE;

int main(int argc, char **argv)
{
	//declaração das variaveis//////////////////////////////////////////////////
	struct sockaddr_in myaddr;	/* our address */
	struct sockaddr_in remaddr;	/* remote address */
	socklen_t addrlen = sizeof(remaddr);		/* length of addresses */
	int recvlen;			/* # bytes received */
	int fd;				/* our socket */
	int slen = sizeof(remaddr);
	unsigned char buf[BUFSIZE];	/* receive buffer */
	int temperatura = 0;
	int temperatura_max = 0;
	int temperatura_min = 0;
	int comand;
	char USB[12];

	int serial,c, res;
	struct termios oldtio,newtio;
	char buf_temp[255];
	////////////////////////////////////////////////////////////////////////////
	//Pedir nome da porta USB///////////////////////////////
	printf("\nDigite o nome da porta USB: \n");
	scanf("%s", USB);
	//configuração porta serial//////////////////////////////////
	serial = open(USB, O_RDWR | O_NOCTTY );
	if (serial <0) {perror(USB); exit(-1); }

	tcgetattr(serial,&oldtio); /* Armazena a configuracao anterior */

	bzero(&newtio, sizeof(newtio)); /* Limpa a struct de configuracao */

	/* Configura o Baudrate, Controle de Fluxo, Bits, paridade */
	newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
	/* Configura os parametros de recebimento de dados */
	newtio.c_iflag = IGNPAR | ICRNL;
	/* Configura os parametros de envio de dados */
	newtio.c_oflag = 0;
	/* Configura a porta no modo canonico */
	newtio.c_lflag = ICANON;
	/* Configuracao de caracteres de controle */
	newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */
	newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
	newtio.c_cc[VERASE]   = 0;     /* del */
	newtio.c_cc[VKILL]    = 0;     /* @ */
	newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */
	newtio.c_cc[VTIME]    = 0;     /* inter-character timer unused */
	newtio.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
	newtio.c_cc[VSWTC]    = 0;     /* '\0' */
	newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */
	newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
	newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
	newtio.c_cc[VEOL]     = 0;     /* '\0' */
	newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
	newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
	newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
	newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
	newtio.c_cc[VEOL2]    = 0;     /* '\0' */

	tcflush(serial, TCIFLUSH);
	tcsetattr(serial,TCSANOW,&newtio);
	//Final da configuracao da porta serial //////////////////////////////////


	//configurações servidor/////////////////////////////////////////////////
	/* create a UDP socket */

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket\n");
		return 0;
	}

	/* bind the socket to any valid IP address and a specific port */

	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(SERVICE_PORT);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}
	/////////////////////////////////////////////////////////////////////////////

	//fazer leitura da temperatura antes do loop para gravar temp_min e temperatura_max
	write(serial, "*", 1);
	res = read(serial,buf,BUFSIZE);
	buf[res-1]=0;
	temperatura_min = atoi(buf);
	temperatura_max = atoi(buf);

	/* LOOP: recebe o comando, lê e responde a temperatura desejada*/
	for (;;) {
		//esperar comando e mostrar qual comando é/////////////////////////////////
		printf("waiting on port %d\n", SERVICE_PORT);
		recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
		printf("received %d bytes\n", recvlen);
		if (recvlen > 0) {
			buf[recvlen] = 0;
			printf("Comando recebido: \"%c\"\n", buf[0]);
		}
		comand = atoi(buf);




			//ler temperatura via serial///////////////////////////////////////////
			write(serial, "*", 1);
			res = read(serial,buf,BUFSIZE);
			buf[res-1]=0;	/* Marca o final da string (para uso com o printf) */
			//printf("%s", buf);
			temperatura = atoi(buf);
			if(temperatura_max < temperatura) {
				temperatura_max = temperatura;
			}
			if(temperatura_min > temperatura) {
				temperatura_min = temperatura;
			}
			printf("temp = %d", temperatura);

		//enviar temperatura////////////////////////////////////////////////////////
		switch (comand) {
			case 0: //responder temperatura atual
				printf("Sending temperatura atual %d\n", SERVICE_PORT);
				sprintf(buf, "%d", temperatura);
				if (sendto(fd, buf, sizeof(buf), 0, (struct sockaddr *)&remaddr, slen)==-1)
				perror("sendto");
				break;
			case 1: //responder temperatura maxima
				printf("Sending temperatura maxima %d\n", SERVICE_PORT);
				sprintf(buf, "%d", temperatura_max);
				if (sendto(fd, buf, sizeof(buf), 0, (struct sockaddr *)&remaddr, slen)==-1)
				perror("sendto");
				break;
			case 2: //responder temperatura minima
				printf("Sending temperatura minima %d\n", SERVICE_PORT);
				sprintf(buf, "%d", temperatura_min);
				if (sendto(fd, buf, sizeof(buf), 0, (struct sockaddr *)&remaddr, slen)==-1)
				perror("sendto");
				break;
		}

	}
	/* never exits */
}
