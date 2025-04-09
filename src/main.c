#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main() {
	// Disable output buffering
	setbuf(stdout, NULL);
 	setbuf(stderr, NULL);

	// You can use print statements as follows for debugging, they'll be visible when running tests.
	printf("Logs from your program will appear here!\n");

	// Uncomment this block to pass the first stage
	
	 int server_fd, client_addr_len;
	 struct sockaddr_in client_addr;
	
	 server_fd = socket(AF_INET, SOCK_STREAM, 0);
	 if (server_fd == -1) {
	 	printf("Socket creation failed: %s...\n", strerror(errno));
	 	return 1;
	 }
	
	// // Since the tester restarts your program quite often, setting SO_REUSEADDR
	// // ensures that we don't run into 'Address already in use' errors
	 int reuse = 1;
	 if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
	 	printf("SO_REUSEADDR failed: %s \n", strerror(errno));
	 	return 1;
	 }
	
	 struct sockaddr_in serv_addr = { .sin_family = AF_INET ,
	 								 .sin_port = htons(4221),
	 								 .sin_addr = { htonl(INADDR_ANY) },
	 								};
	
	 if (bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0) {
	 	printf("Bind failed: %s \n", strerror(errno));
	 	return 1;
	 }
	
	 int connection_backlog = 5;
	 if (listen(server_fd, connection_backlog) != 0) {
	 	printf("Listen failed: %s \n", strerror(errno));
	 	return 1;
	 }
	
	 printf("Waiting for a client to connect...\n");
	 client_addr_len = sizeof(client_addr);
	
	int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);

	 printf("Client connected\n");
		
	char* buff ="HTTP/1.1 200 OK\r\n\r\n";

	 
	char buf[1024];

	int recived = read(client_fd, buf, 1023);

	buf[recived]= '\0';

	char* line = strtok(buf, "\r\n");
	char method[8], path[1024], version[16];
	sscanf(line, "%s %s %s", method, path, version);
	

	char* echo_str = path+6;
	int content_length = strlen(echo_str);
	char res[2048];
	int len = sprintf(res, "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
        "%s", content_length, echo_str);
	if(strcmp(path, "/") == 0 ){
	
	write(client_fd , buff, strlen(buff));
	}
	

	else if(strncmp(path , "/echo/", 6) == 0){
	write(client_fd, res, strlen(res));
	}

	else{
	write(client_fd, "HTTP/1.1 404 Not Found\r\n\r\n", 26) ;
	}
	
	close(server_fd);

	return 0;
}
