#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFLENGTH 256
#define PORT 31337

//function to serve requests, passed an open socket descriptor
void handleRequest(int clnt) {
	char request[BUFLENGTH];

	//read request
	if (read(clnt, request, BUFLENGTH) == -1) {
		perror("socket read error");
		exit(0);
	}
	//print request
	printf("Request received: %s\n", request);
	//check if starts with "GET"
	if (strncmp("GET", request, 3) == 0) {
		 //write 404
		write(clnt, "HTTP/1.0 404 Not Found\r\n\r\n", BUFLENGTH);
	}
	//close socket
	close(clnt);
}

int main() {
	int srvsock, clntsock;
	unsigned int adr_length;
	struct sockaddr_in clnt_adr, serv_adr;

	if ((srvsock=socket(AF_INET, SOCK_STREAM, 0)) == -1) {//create socket
		perror("socket creation error");
		exit(0);
	}

	//set address info
	memset(&serv_adr, 0, sizeof(serv_adr));
	//internet family again
	serv_adr.sin_family = AF_INET;
	//don't care about source address
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	//set port
	serv_adr.sin_port = htons(PORT);
	//bind socket to address and port
	if (bind(srvsock, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) == -1) {
		perror("socket binding error");
		close(srvsock);
		exit(0);
	}
	//set up a connection queue: socket to listen on, max connections in queue
	if (listen(srvsock, 5) == -1) {
		perror("socket listening error");
		close(srvsock);
		exit(0);
	}
	//loop forever as servers often do
	while (1) {
		adr_length = sizeof(clnt_adr);

		//wait for a connection, then assign the connection to a new socket descriptor
		//note that here clnt_adr is filled in for us by the accept call, and has client info
		if ((clntsock=accept(srvsock, (struct sockaddr *) &clnt_adr, &adr_length)) == -1) {
			perror ("socket accept error");
			close(srvsock);
			exit(0);
		}
		//handle the request
		handleRequest(clntsock);
		//NOTE: in my pseudo-code I had a fork here, but I didn't bother to code it for this
		//example.  In reality, a child process (or even more realistically, a new thread)
		//would handle the request so the server could accept new connections.
	}
	return 0;
}
