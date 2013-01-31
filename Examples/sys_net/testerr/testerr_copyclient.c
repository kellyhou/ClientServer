#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char **argv)
{
    int socket_fd;
    int server_port = 5858;
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0 );

    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr("10.1.1.173");

    char buf[1024] = "";
    int server_addr_len = sizeof(struct sockaddr_in);
    int fd = 0;
    fd = open("bb.txt", O_CREAT|O_RDWR);

    gets(buf);
    sendto(socket_fd, buf, sizeof(buf), 0, 
			(struct sockaddr*)&server_addr,sizeof(struct sockaddr_in));
    	
    time_t t1, t2;
    char recvmsg[1024] = "";
    int ret, sumret = 0;
    t1 = time(NULL);
    while(1){
	ret = recvfrom(socket_fd, recvmsg,sizeof(recvmsg),0,(struct sockaddr*)&server_addr, &server_addr_len);
	sumret += ret;
	printf("ret = %d\n", ret);
        printf("revice msg: %s \n", recvmsg);
        write(fd, recvmsg,  ret );
    	if(!strncmp(recvmsg, "ok!!!", 5)){
	    printf("the file trans ok!!!");
	    break;
	}
    }

    t2 = time(NULL) - t1;
    printf("sumret = %d trans file rate: %f\n", sumret, sumret * 1.0 /t2);
    close(socket_fd);
    close(fd);
	
    return 0;
}
