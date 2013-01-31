#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "tcp.h"

int main( int argc, char ** argv )
{
	int listen_fd = socket( AF_INET, SOCK_STREAM, 0 );
	if( listen_fd == -1 ) {
		perror("socket");
		exit( 1 );
	}

	int reuse = 1;
	setsockopt( listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int) );
	int ret;
	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons( PORT_NUM );
	myaddr.sin_addr.s_addr = htonl( INADDR_ANY );
	ret = bind( listen_fd, ( SA* )&myaddr, ADDR_LEN );
	if( ret == -1 ) {
		perror("bind");
		exit( 2 );
	}

	ret = listen( listen_fd, BACK_LOG );
	if( ret == -1 ) {
		perror("listen");
		exit( 3 );
	}

	struct sockaddr_in client;
	int client_addr_len = ADDR_LEN;
	int client_com_fd;
	client_com_fd = accept( listen_fd, ( SA * )&client, &client_addr_len );
	if( client_com_fd == -1 ) {
		perror("accept");
		exit( 4 );
	}

	char recv_buf[1400];
	recv( client_com_fd, recv_buf, sizeof(recv_buf), 0 );
	printf("receive from client: %s\n", recv_buf );

	close( client_com_fd );
	close( listen_fd );
	return 0;
}






