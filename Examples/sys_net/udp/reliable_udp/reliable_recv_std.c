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

	FILE * fp = fopen( "save_file", "w" );
	if( fp == NULL ) {
		perror("creat");
		exit( 1 );
	}
	setvbuf( fp, NULL, _IOFBF, 1024 * 1024 * 10 );

	struct sockaddr_in client;
	int client_addr_len = sizeof(struct sockaddr);
	struct packet * recvpacket = malloc( 1500 );
	struct packet * ackpacket = malloc( sizeof(struct packet) );
	
	int seq = 0;

	time_t front, after;

	int recv_all_bytes = 0;

	int flag = 0;

	while( 1 ) {
		ret = recvfrom( sock_fd, recvpacket, 1480, 0, 
				( struct sockaddr * )&client, &client_addr_len );

		if( flag == 0 ) {
			time( &front );
			flag = 1;
		}
		if( ret == -1 ) {
			perror("recvfrom");
			close( sock_fd );
			exit( 3 );
		}

		ackpacket->code = ACK;
		ackpacket->seq = recvpacket->seq;
		sendto( sock_fd, ackpacket, sizeof(struct packet), 0,
				( struct sockaddr * )&client, client_addr_len );

		if( !strcmp( recvpacket->buf, "exit" ) )
			break;
		if( recvpacket->seq == seq + 1 ) {
			fwrite( recvpacket->buf, ret-8, 1, fp );
			recv_all_bytes += ret;
			seq ++;
		}
	}

	time( &after );

	printf("平均每秒%fk\n",recv_all_bytes * 1.0 /(after-front )/1024 );

	fclose( fp );
	close( sock_fd );
	return 0;
}

