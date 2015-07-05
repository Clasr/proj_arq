/*
Simple udp client
Silver Moon (m00n.silv3r@gmail.com)
*/
#include<stdio.h>
#include <stdlib.h>
#include<winsock2.h>
#include "port.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define SERVER "127.0.0.1"
#define BUFLEN 2048  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

int main(void)
{
	int comand = 0;
	struct sockaddr_in myaddr, remaddr;
	int fd, slen = sizeof(remaddr);
	char *server = "127.0.0.1";
	char buf[BUFLEN];
	int recvlen;

	WSADATA wsa;

	//Initialise winsock
	printf("\nInicializando Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Falhou. Codigo de erro : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	printf("Inicializado.\n");

	//create socket
	if ((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() falhou com codigo de erro : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//setup address structure
	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(PORT);
	myaddr.sin_addr.S_un.S_addr = inet_addr(SERVER);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}

	//start communication
	while (1)
	{
		//receber comando
		printf("\nDigite:\n \t0 para ler temperatura atual\n\t1 para temperatura maxima\n\t2 para temperatura minima\n");
		scanf("%d", &comand);

		/* now let's send the messages */

		switch (comand) {
		case 0:
			printf("Enviando comando to %s port %d\n", server, SERVICE_PORT);
			buf[0] = '0';
			buf[1] = '1';
			buf[2] = '2';
			int j;
			printf("Temperatura: ");
			for (j = 0; j < 3; j++)
			{
				printf("%s", buf[j]);
			}
			printf("\ncomando: \"%c\"\n", buf[0]);
			if (sendto(fd, buf, 1, 0, (struct sockaddr *)&remaddr, slen) == -1)
				perror("sendto");

			printf("waiting on port %d\n", SERVICE_PORT);
			recvlen = recvfrom(fd, buf, BUFLEN, 0, (struct sockaddr *)&remaddr, &slen);
			printf("received %d bytes\n", recvlen);
			if (recvlen > 0) {
				buf[recvlen] = 0;
				printf("Temperatura: \"%sC\"\n", buf[BUFLEN]);
			}
			break;

		case 1:
			printf("Enviando comando para %s port %d\n", server, SERVICE_PORT);
			buf[0] = '1';
			printf("comando: \"%c\"\n", buf[0]);
			if (sendto(fd, buf, 1, 0, (struct sockaddr *)&remaddr, slen) == -1)
				perror("sendto");

			printf("waiting on port %d\n", SERVICE_PORT);
			recvlen = recvfrom(fd, buf, BUFLEN, 0, (struct sockaddr *)&remaddr, &slen);
			printf("received %d bytes\n", recvlen);
			if (recvlen > 0) {
				buf[recvlen] = 0;
				printf("Temperatura: \"%sC\"\n", buf[BUFLEN]);
			}
			break;
		case 2:
			printf("Enviando comando para %s port %d\n", server, SERVICE_PORT);
			buf[0] = '2';
			printf("comando: \"%c\"\n", buf[0]);
			if (sendto(fd, buf, 1, 0, (struct sockaddr *)&remaddr, slen) == -1)
				perror("sendto");

			printf("waiting on port %d\n", SERVICE_PORT);
			recvlen = recvfrom(fd, buf, BUFLEN, 0, (struct sockaddr *)&remaddr, &slen);
			printf("received %d bytes\n", recvlen);
			if (recvlen > 0) {
				buf[recvlen] = 0;
				printf("Temperatura: \"%sC\"\n", buf[BUFLEN]);
			}
			break;

		}
	}

	closesocket(fd);
	WSACleanup();

	return 0;
}
