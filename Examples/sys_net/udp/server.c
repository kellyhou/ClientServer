#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// server.c

#define PORT 8888


int main(int argc, char **argv)
{
	int sfd = socket( AF_INET, SOCK_DGRAM, 0 ) ;
	
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons( PORT );
	server_addr.sin_addr.s_addr = htonl( INADDR_ANY );

	bind( sfd, ( struct sockaddr * )&server_addr, sizeof(server_addr) );

	char buf[100];
	struct sockaddr_in client_addr;
	int client_addr_len = sizeof(struct sockaddr_in);

	int ret;
	while( 1 ) {
		bzero( buf, sizeof(buf) );
		ret = recvfrom( sfd, buf, sizeof(buf), 0, 
				(struct sockaddr*)(&client_addr),&client_addr_len );	
		printf("ret = %d\n", ret );
		printf("from %s messge: %s\n", inet_ntoa( client_addr.sin_addr ),
			buf );
		if( !strcmp( buf, "exit" ) )
			break;
	}

	close( sfd );
}








