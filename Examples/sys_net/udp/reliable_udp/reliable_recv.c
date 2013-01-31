#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PORT_NUM 6666

enum code {
	FIN, 
	DATA,
	ACK
};

struct packet {
	int code;
	int seq;
	char buf[1];
};

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
	myaddr.sin_addr.s_addr = htonl( INADDR_ANY );

	int ret = bind( sock_fd, ( struct sockaddr * )&myaddr, sizeof(struct sockaddr_in) );

	if( ret == -1 ) {
		perror("bind");
		exit( 2 );
	}

	int fd = creat( "save_file", 0666 );
	if( fd == -1 ) {
		perror("creat");
		exit( 1 );
	}

	struct sockaddr_in client;
	int client_addr_len = sizeof(struct sockaddr);
	struct packet * recvpacket = malloc( 1500 );
	struct packet * ackpacket = malloc( sizeof(struct packet) );
	
	int seq = 0;
	while( 1 ) {
		ret = recvfrom( sock_fd, recvpacket, 1450, 0, 
				( struct sockaddr * )&client, &client_addr_len );

		if( ret == -1 ) {
			perror("recvfrom");
			close( sock_fd );
			exit( 3 );
		} else if( ret != 1408 ) {
			printf("ret = %d\n", ret );
		}

		ackpacket->code = ACK;
		ackpacket->seq = recvpacket->seq;
		sendto( sock_fd, ackpacket, sizeof(struct packet), 0,
				( struct sockaddr * )&client, client_addr_len );

		if( !strcmp( recvpacket->buf, "exit" ) )
			break;
		if( recvpacket->seq == seq + 1 ) {
			write( fd, recvpacket->buf, ret-8 );
			seq ++;
		}
	}

	close( fd );
	close( sock_fd );
	return 0;
}

