#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

//Port would be 80 for a real web server, but only root can use ports < 1024
#define PORT 31337
#define BUFLENGTH 256

int main(int argc, char *argv[]) {
	int clientsocket;
	struct sockaddr_in serv_adr;
	struct hostent *host;
	//create a read/write buffer
	char response[BUFLENGTH];

	//check that server is specified
	if (argc < 2) {
		printf("The server must be specified\n");
		printf("Usage: %s <hostname> \n", argv[0]);
		exit(1);
	}
	//get host address from name
	host = gethostbyname(argv[1]);
	if (host == (struct hostent *) NULL) {
		perror("gethostbyname: ");
		exit(0);
	}
	//init serv_adr memory to 0
	memset(&serv_adr, 0, sizeof(serv_adr));
	//set internet family
	serv_adr.sin_family = AF_INET;
	//set host information from above
	memcpy(&serv_adr.sin_addr, host->h_addr, host->h_length);
	//set port
	serv_adr.sin_port = htons(PORT);
	//create socket: internet family, stream based, let computer decide network method
	if ((clientsocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket creation error");
		exit(0);
	}

	//connect to host socket: socket to connect, server info from above, address struct size
	if (connect(clientsocket, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) == -1) {
		perror("socket connection error");
		close(clientsocket);
		exit(0);
	}

	//send HTTP request for index.html: socket to write to, string, max length
	write(clientsocket, "GET /index.html HTTP/1.0", BUFLENGTH);

	//read reply: socket to read from, char array to write to, max length
	if (read(clientsocket, response, BUFLENGTH) == -1) {
		//wait for and read response
		perror("socket read error");
		exit(0);
	}
	//print response
	printf("%s", response);
	//close socket
	close(clientsocket);

	return 0;
}
