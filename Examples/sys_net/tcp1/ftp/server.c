#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include "ftp.h"

#define PORT_NUM 6000
#define ADDR_SIZE ( sizeof( struct sockaddr_in ) )
typedef struct sockaddr sockaddr_t;

void init_addr( struct sockaddr_in * addr, unsigned short port, char * ip )
{
	addr->sin_family = AF_INET;
	addr->sin_port = htons( port );	
	addr->sin_addr.s_addr = inet_addr( ip );
}

void sig_child( int sig )
{
	wait( NULL );	
}

int readn( int fd, void * buf, int len )
{
	char * p = buf;

	int num = 0;
	while( len - num ) {
		num += read( fd, p+num, len - num );	 
		
	}			

	return 0;	
}

int writen( int fd, void * buf, int len )
{
	char * p = buf;

	int num = 0;
	while( len - num ) {
		num += write( fd, p+num, len - num );	 
	}			

	return 0;	
}

int send_err_to_client( int sock_connect_fd, int errno )
{	

}


int do_client( int sock_connect_fd )
{
		
	int code;	
	char filename[FILE_NAMESIZE];
	
	char cmd[250];
	
	char data_buf[1400];
	int ret;

	while( 1 ) {
		bzero( cmd, 250 );
		int ret = recv( sock_connect_fd, &code, sizeof(int), MSG_WAITALL );
		if( ret < 0 ) {
			perror("recv error.");
			break;
		} else if( ret == 0 ) {
			break;			
		}

		switch( code ) {
			case PUT:
				break;	
			case GET:
				break;
			case LS:
				readn( sock_connect_fd, filename, FILE_NAMESIZE );
				strncpy( cmd, "ls ", 3 );
				strcpy( cmd+3, filename );

				*(int*)data_buf = DATA;

				printf("cmd = %s\n", cmd );
				FILE * fp = popen( cmd, "r" );
				if( fp == NULL ) {
					perror("popen");
					exit( 1 );
				}

				while( 1 ) {
					int ret = fread( data_buf+8, 1, 1000, fp );
					if( ret == 0 ) {
						*(int*)data_buf = DATA_END;	
						writen( sock_connect_fd,
						        data_buf, sizeof(int) );
						break;	
					} else if( ret > 0 ) {
						*( int * )(data_buf) = DATA;
						*( int * )(data_buf+4) = ret;
						writen( sock_connect_fd,
						        data_buf, 8 + ret );
					} else {
						perror("fread");
						exit( 1 );
					}
				}
				fclose( fp );
				break;
			case CD:
	
			case DEL:	
				
			case QUIT:
			default:
				break;		
		}
	}
}

int change_root( void )
{
	char cwd[128];
	getcwd( cwd, sizeof(cwd) );
	chroot( cwd );
	chdir( "/" );	
	return 0;
}

int main( int argc, char ** argv )
{
	
	int sock_listen_fd = socket( AF_INET, SOCK_STREAM, 0 );
	
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons( PORT_NUM );
	server_addr.sin_addr.s_addr = htonl( INADDR_ANY );
		
	int reuse = 1;
	setsockopt( sock_listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, 4 );
	int ret = bind( sock_listen_fd, ( sockaddr_t * )&server_addr, ADDR_SIZE );
	if( ret == -1 ) {
		perror("bind");
		exit( 1 );	
	}
	
	//change_root(); 
	listen( sock_listen_fd, 20 );
	
	signal( SIGCHLD, sig_child );

	struct sockaddr_in client_addr;
	int len = ADDR_SIZE;

	while( 1 ) {
		int sock_connect_fd = accept( sock_listen_fd, 	
			( sockaddr_t * )&client_addr, &len );

		printf("client %s have login.\n", inet_ntoa( client_addr.sin_addr ));

		pid_t pid = fork();
		if( pid < 0 ) {
			perror("fork");
			exit( 1 );
		} else if( pid == 0 ) {
			close( sock_listen_fd );
			do_client( sock_connect_fd ); 		
			close( sock_connect_fd );

			printf("client %s have exit.\n",
				 inet_ntoa( client_addr.sin_addr ));
			exit( 0 );
		}
		close( sock_connect_fd );
	}
	close( sock_listen_fd );	
	
	return 0;
}











