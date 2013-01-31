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
#include <sys/select.h>

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
	if( argc != 3 ) {
		fprintf( stderr, "%s <ipaddr> <filename>\n", argv[0] );
		exit( 1 );
	}	

	int sock_fd = socket( AF_INET, SOCK_DGRAM, 0 );
	if( sock_fd == -1 ) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons( PORT_NUM ); 
	server_addr.sin_addr.s_addr = inet_addr( argv[1] );

	int fd = open( argv[2], O_RDONLY );
	if( fd == -1 ) {
		perror("open");
		exit( 1 );
	}

	struct packet * sendpacket = malloc( 1500 );
	struct packet * recv_ack_packet = malloc( sizeof(struct packet) );

	int seq = 1;
	int read_count;
	int ret;
	while( 1 ) {
			read_count = read( fd, sendpacket->buf, 1400 );
			if( read_count == -1 ) {
				perror("read");
				exit( 1 );
			} else if( read_count == 0 ) {
				strcpy( sendpacket->buf, "exit" );	
				read_count = 5;
			}

			while( 1 ) {
				sendpacket->code = DATA;
				sendpacket->seq = seq;
				ret = sendto( sock_fd, sendpacket,read_count+8 , 0, 
				( struct sockaddr * )&server_addr, sizeof(struct sockaddr_in ) );
				if( ret == -1 ) {
					perror("sendto");
					close( sock_fd );
					exit( 3 );
				} else if( ret != read_count + 8 ) {
					printf("send %d\n", ret );
				}

				fd_set readset;
				FD_ZERO( &readset );

				FD_SET( sock_fd, &readset );
				struct timeval tv;
				tv.tv_sec = 4;
				tv.tv_usec = 0;
				ret = select( sock_fd+1, &readset, NULL, NULL, &tv );
				if( ret == -1 ) {
					perror("select");
					exit( 3 );
				} else if( ret > 0 ) {
					recvfrom( sock_fd, recv_ack_packet, sizeof(struct packet),
							0, NULL, NULL );
					if( recv_ack_packet->seq == seq && 
						recv_ack_packet->code == ACK ) {
						break;	
					}
				}
			}
			if( !strcmp( sendpacket->buf, "exit" ) )
				break;
			seq ++;
	}	

	printf("send finish.\n");

	close( fd );
	close( sock_fd );
	return 0;
}

