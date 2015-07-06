/*
        demo-udp-03: udp-send: a simple udp client
	send udp messages
	sends a sequence of messages (the # of messages is defined in MSGS)
	The messages are sent to a port defined in SERVICE_PORT

        usage:  udp-send

        Paul Krzyzanowski
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include "port.h"

#define BUFLEN 2048
#define MSGS 5	/* number of messages to send */

int main(void)
{
	struct sockaddr_in myaddr, remaddr;
	int fd, i, slen=sizeof(remaddr);
	char *server = "127.0.0.1";	/* change this to use a different server */
	char buf[BUFLEN];
	int comand = 0;
	int recvlen;
	socklen_t addrlen = sizeof(remaddr);		/* length of addresses */

	/* create a socket */

	if ((fd=socket(AF_INET, SOCK_DGRAM, 0))==-1)
		printf("socket created\n");

	/* bind it to all local addresses and pick any port number */

	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(0);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}

	/* now define remaddr, the address to whom we want to send messages */
	/* For convenience, the host address is expressed as a numeric IP address */
	/* that we will convert to a binary format via inet_aton */

	memset((char *) &remaddr, 0, sizeof(remaddr));
	remaddr.sin_family = AF_INET;
	remaddr.sin_port = htons(SERVICE_PORT);
	if (inet_aton(server, &remaddr.sin_addr)==0) {
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

	while (1) {
		//receber comando
		printf("\nDigite:\n \t0 para ler temperatura\n\t1 para temperatura maxima\n\t2para temperatura minima\n");
		scanf("%d", &comand);

		/* now let's send the messages */

		switch (comand) {
			case 0:
				printf("Sending comando to %s port %d\n", server, SERVICE_PORT);
				buf[0] = '0';
				printf("comando: \"%c\"\n", buf[0]);
				if (sendto(fd, buf, 1, 0, (struct sockaddr *)&remaddr, slen)==-1)
				perror("sendto");


				printf("waiting on port %d\n", SERVICE_PORT);
				recvlen = recvfrom(fd, buf, BUFLEN, 0, (struct sockaddr *)&remaddr, &addrlen);
				printf("received %d bytes\n", recvlen);
				if (recvlen > 0) {
					buf[recvlen] = 0;
					printf("Temperatura: \"%sC\"\n", buf);
				}
				break;

			case 1:
				printf("Sending comando to %s port %d\n", server, SERVICE_PORT);
				buf[0] = '1';
				printf("comando: \"%c\"\n", buf[0]);
				if (sendto(fd, buf, 1, 0, (struct sockaddr *)&remaddr, slen)==-1)
					perror("sendto");


				printf("waiting on port %d\n", SERVICE_PORT);
				recvlen = recvfrom(fd, buf, BUFLEN, 0, (struct sockaddr *)&remaddr, &addrlen);
				printf("received %d bytes\n", recvlen);
				if (recvlen > 0) {
					buf[recvlen] = 0;
					printf("Temperatura: \"%sC\"\n", buf);
				}
				break;
			case 2:
				printf("Sending comando to %s port %d\n", server, SERVICE_PORT);
				buf[0] = '2';
				printf("comando: \"%c\"\n", buf[0]);
				if (sendto(fd, buf, 1, 0, (struct sockaddr *)&remaddr, slen)==-1)
					perror("sendto");

				printf("waiting on port %d\n", SERVICE_PORT);
				recvlen = recvfrom(fd, buf, BUFLEN, 0, (struct sockaddr *)&remaddr, &addrlen);
				printf("received %d bytes\n", recvlen);
				if (recvlen > 0) {
					buf[recvlen] = 0;
					printf("Temperatura: \"%sC\"\n", buf);
				}
				break;

		}
	}
	close(fd);
	return 0;
}
