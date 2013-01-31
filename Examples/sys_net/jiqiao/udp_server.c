#define _LARGEFILE64_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
/*#include <linux/in.h>*/
#include <netinet/in.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <dirent.h>
#include "head.h"
typedef struct savefile savefile_t;
struct savefile {
	int fd;
	struct sockaddr_in client;
} ;

int main(int argc, char **argv)
{
	int sfd = socket( PF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in server;

	server.sin_family = AF_INET;
	server.sin_port = UDP_PORT;
	server.sin_addr.s_addr = htonl( 0 );
	bind( sfd, (struct sockaddr* )& server, sizeof(server));
	/*chroot("dir");*/
	struct sockaddr_in client;
	char buf[ BUF_LEN ];
	socklen_t socklen = sizeof(client);
	savefile_t savefile[100];
	bzero( savefile, 100 * sizeof( savefile_t));
	while (1){ 
		bzero(buf, BUF_LEN);
		recvfrom( sfd, buf, sizeof(buf), 0,
				(struct sockaddr *)&client, &socklen);
		if( !strcmp (buf, "exit")){

		}
		if( !strncmp (buf, "file", 4)){  //open a file 
			open_file( savefile, buf + 4, &client );	
		}else  if( !strncmp (buf, "zhu ", 4)){
			save_file( savefile, buf + 4, &client );	
			/*printf(" savefile ip = %s, %s\n", inet_ntoa( client.sin_addr ), buf);	*/
		}else {
			printf("ip = %s, %s\n", inet_ntoa( client.sin_addr ), buf);	
		}
	}
	close(sfd);
}
int save_file( savefile_t *savefile, char *buf, const struct sockaddr_in * client )
{
	int i = 0;	
	for (i = 0; i < 100; i ++) {
		if( savefile[i].fd != 0 && 
				savefile[i].client.sin_addr.s_addr == client->sin_addr.s_addr &&
				savefile[i].client.sin_port == client->sin_port ){
			int size = atoi( buf );
			/*int size = */
			if ( size == 0){
				close( savefile[i].fd); 
				savefile[i].fd =0;
			}else {
				write( savefile[i].fd, buf + 16, size );
				/*printf("收到了 %d,个字节\n", size );*/
			}
			break;
		}	
	}
}
int open_file( savefile_t *savefile, char *buf, const struct sockaddr_in * client )
{
	int i = 0;
	while ( savefile[i].fd  != 0){
		i ++;
	}
	savefile[i].fd = open( buf, O_CREAT | O_WRONLY | O_TRUNC , 0777);
	/*off64_t file_size = atoi( buf + 60);*/
	int file_size = atoi( buf + 60);
	/*lseek64 (savefile[i].fd , file_size-1, SEEK_SET);*/
	lseek (savefile[i].fd , file_size-1, SEEK_SET);
	write (savefile[i].fd, " ", 1);
	lseek (savefile [i].fd, 0, SEEK_SET );
	memcpy(&savefile[i].client, client, sizeof(struct sockaddr_in));
	printf("ip = %s,create file ##%s##\n", inet_ntoa( savefile[i].client.sin_addr ), buf );	
	return 0;
}
