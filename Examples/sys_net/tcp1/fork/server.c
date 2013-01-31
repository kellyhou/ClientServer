#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "tcp.h"

void sig_child( int sig )
{
	int status;
	int pid = wait( &status );
	printf("pid = %d exit status %d\n", pid, WEXITSTATUS( status ) );
}

int main( int argc, char ** argv )
{
	int listen_fd = socket( AF_INET, SOCK_STREAM, 0 );
	if( listen_fd == -1 ) {
		perror("socket");
		exit( 1 );
	}

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

	signal( SIGCHLD, sig_child );
	while( 1 ) {
		struct sockaddr_in client;
		int client_addr_len = ADDR_LEN;
		int client_com_fd;
		client_com_fd = accept( listen_fd, ( SA * )&client, &client_addr_len );
		if( client_com_fd == -1 ) {
			perror("accept");
			exit( 4 );
		}

		pid_t child_pid = fork();
		if( child_pid == 0 ) {
				close( listen_fd );
				while( 1 ) {
					char recv_buf[1400];
					int ret = recv( client_com_fd, recv_buf, sizeof(recv_buf), 0 );
					if( ret == -1 || ret == 0 )
						break;
					printf("receive from client %s: %s\n", 
							inet_ntoa( client.sin_addr ), recv_buf );
					if( !strcmp( recv_buf, "exit" ) )
						break;
				}
				close( client_com_fd );
				exit( 0 );
		}

		close( client_com_fd );
	}
	close( listen_fd );
	return 0;
}






