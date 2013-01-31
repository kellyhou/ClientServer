#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "gen_single_nohead_pre.h"
#include "tcp.h"

enum message_type {
	LOGIN,
	MESSAGE,
	LOGOUT,
	UPDATE_ALL_ONLINE_PERSON_INFO
};

typedef struct client client_t;
struct client {
	int client_com_fd;
	char login_name[20];
	struct sockaddr_in client_addr;
};

#define PACKET_MAX_SIZE 1400		

int writen( int fd, char * buf, int len )
{							
	while( len > PACKET_MAX_SIZE ) {
		send( fd, buf, PACKET_MAX_SIZE, 0 );		
		buf += PACKET_MAX_SIZE;
		len -= PACKET_MAX_SIZE;
	}	
	send( fd, buf, len, 0 );
}

int qun_fa( node_t * head, char * login_name, char * buf, int len )
{
	node_t * temp = head;
	char send_buf[1500];
	bzero( send_buf, sizeof(send_buf) );

	while( temp != NULL ) {
		client_t * temp_client = temp->data;
		int temp_fd = temp_client->client_com_fd;
		*( int * ) send_buf = MESSAGE;
		strcpy( send_buf + 4, login_name );
		strcat( send_buf + 4, ": ");
		strcat( send_buf + 4, buf );
		int ret = send( temp_fd, send_buf, strlen( send_buf + 4 ) + 4 + 1, 0 );
		temp = temp->next;
	}
	return 0;
}

int update_all_online_person_info( node_t * head, int type )
{

	struct message_head {
		int type;
		int all_data_len;
	} mhead;


	char buf[PACKET_MAX_SIZE] =  { 0 };	
	char * p = buf;

	mhead.type = type; 

	int all_data_len = 0;
	node_t * temp = head;
	client_t * client;

	char * all_data_buf = NULL;
	int single_len;
	while( temp != NULL ) {
		client = temp->data;
		single_len = strlen( client->login_name ) + 4 + 1;
		all_data_len += single_len;
		all_data_buf = realloc( all_data_buf, all_data_len );
		
		*( int * )( all_data_buf + all_data_len -single_len ) = single_len - 4;
		memcpy( all_data_buf + all_data_len - single_len + 4, 
				client->login_name, single_len - 4 ); 

		temp = temp->next;
	}

	mhead.all_data_len = all_data_len;
	
	temp = head;

	while( temp != NULL ) {
		client = temp->data;				
		send( client->client_com_fd, &mhead, sizeof(mhead), 0 );			
		writen( client->client_com_fd, all_data_buf, all_data_len );		
		temp = temp->next;
	}
}

int main( int argc, char ** argv )
{
	int listen_fd = socket( AF_INET, SOCK_STREAM, 0 );
	if( listen_fd == -1 ) {
		perror("socket");
		exit( 1 );
	}

	int reuse = 1;
	int ret;
	ret = setsockopt( listen_fd, SOL_SOCKET, 
		SO_REUSEADDR, &reuse, sizeof(int) );
	if( ret == -1 ) {
		perror("setsockopt");
		exit( 1 );
	}


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

	node_t * head = NULL;
	node_t * temp;

	while( 1 ) {
		fd_set readset;	
		FD_ZERO( &readset );
		int max_fd = listen_fd;

		FD_SET( listen_fd, &readset );

		temp = head;
		while( temp != NULL ) {
			int temp_fd = ((client_t *)temp->data)->client_com_fd;
			if( temp_fd > max_fd )
				max_fd = temp_fd;
			FD_SET( temp_fd, &readset );
			temp = temp->next;
		}

		ret = select( max_fd + 1, &readset, NULL, NULL, NULL );
		if( ret < 0 ) {
			perror("select");
			exit( 4 );
		} else if( ret > 0 ) {
			if( FD_ISSET( listen_fd, &readset ) ) {
				struct sockaddr_in client_addr;
				int client_addr_len = ADDR_LEN;
				int client_com_fd;
				client_com_fd = accept( listen_fd, ( SA * )&client_addr, 
					&client_addr_len );
				if( client_com_fd == -1 ) {
					perror("accept");
					exit( 5 );
				}
			
				client_t client;
				client.client_com_fd = client_com_fd;
				bzero( client.login_name, sizeof(client.login_name) );

				int recv_count = recv( client_com_fd, client.login_name, 
					sizeof(client.login_name), 0 );
				if( recv_count < 0 ) {
					perror("recv");
				}
				memcpy( &client.client_addr, &client_addr, client_addr_len );
				insert_node( &head, &client, sizeof(client_t) );

				update_all_online_person_info( head, LOGIN );  
			}

			temp = head;
			while( temp != NULL ) {
				client_t * temp_client = temp->data;
				int temp_fd = temp_client->client_com_fd;
				char recv_buf[1500];
				int  recv_count;

				if( FD_ISSET( temp_fd, &readset ) ) {
					recv_count = recv( temp_fd, recv_buf, sizeof(recv_buf), 0 );
					if( recv_count == 0 ) {
						close( temp_client->client_com_fd );
						temp = temp->next;
						del_node( &head, temp_client, sizeof(client_t) );	
						update_all_online_person_info( head, LOGOUT );  
						continue;
					}
					qun_fa( head, temp_client->login_name, 
						recv_buf, recv_count + 1 );
				}
			temp = temp->next;
		}
	}
	}
		
	close( listen_fd );
	return 0;
}






