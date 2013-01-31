#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT_NUM 6666

int main( int argc, char ** argv )
{
	
	int sock_fd = socket( AF_INET, SOCK_DGRAM, 0 );
	if( sock_fd == -1 ) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons( PORT_NUM ); 
	myaddr.sin_addr.s_addr = inet_addr( "192.168.20.2" );

	int ret = bind( sock_fd, ( struct sockaddr * )&myaddr, sizeof(struct sockaddr_in) );

	if( ret == -1 ) {
		perror("bind");
		exit( 2 );
	}

	struct sockaddr_in client;
	int client_addr_len = sizeof(struct sockaddr);
	char buf[128];

	while( 1 ) {
		bzero( buf, sizeof(buf) );
		ret = recvfrom( sock_fd, buf, sizeof(buf), 0, ( struct sockaddr * )&client, &client_addr_len );
	
		if( ret == -1 ) {
			perror("recvfrom");
			close( sock_fd );
			exit( 3 );
		}

		printf("recvfrom client ip = %s port = %d buf = %s\n",
			inet_ntoa( client.sin_addr ), ntohs( client.sin_port ), buf );
			
		ret = sendto( sock_fd, buf, ret, 0, ( struct sockaddr *)&client, sizeof(struct sockaddr_in) ); 
		if( ret == -1 ) {
			perror("sendto" );
			close( sock_fd );
			exit( 4 );
		}
		if( !strcmp( buf, "exit" ) )
			break;
	}

	close( sock_fd );
	return 0;
}

