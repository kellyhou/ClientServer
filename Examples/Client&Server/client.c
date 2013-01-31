#include<sys/socket.h>
#include<sys/fcntl.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<stdio.h>

#define MAXLINE	128

//-----------------------------------------------------------------------------------

char 		UserName[20];
char 		Password[10];

//-----------------------------------------------------------------------------------

void Encryption(char *str);		//加密函数
void login(char *buff);			//用户发送登录信息
void ShareFile(char *buff);		//用户发送共享文件信息
void SearchFile(char *buff);		//用户发送查找文件信息

//-----------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	int			sockfd, n;
	char			recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;
	char 			buff[MAXLINE + 1];
	
	if (argc != 1)
		perror("usage: a.out <IPaddress>");
	
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		perror("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(2000);	/* daytime server */
	servaddr.sin_addr.s_addr  = inet_addr("127.0.0.1");
	//if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
	//	printf("inet_pton error for %s", argv[1]);

	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
		perror("connect error");
	memset(buff,0,sizeof(buff));
	login(buff);
	write(sockfd,buff,strlen(buff)+1);

	if ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		printf("%s\n",recvline);
	
	}
	if (n < 0)
		perror("read error");
	int quit=1;
	if(recvline[0]=='L')
		quit=0;
	while(quit==0)
	{
		printf("Enter your choice(S for Share, F for Search, Q for Quit): ");
		char c=getchar();
		switch(c)
		{
		    case 's':
		    case 'S':
			ShareFile(buff);
			write(sockfd,buff,strlen(buff)+1);

			if ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
				printf("%s\n",recvline);
	
			}
			if (n < 0)
				perror("read error");
			break;
		case 'f':
		case 'F':
			SearchFile(buff);
			write(sockfd,buff,strlen(buff)+1);
			while( (n = read(sockfd, recvline, MAXLINE)) > 0) {
				printf("%s",recvline);
				if(recvline[4]=='\n')
				break;
			}
			if (n < 0)
				perror("read error\n");
			
			break;
		case 'q':
		case 'Q':
			strcpy(buff,"QUIT\n");
			write(sockfd,buff,strlen(buff)+1);
			quit=1;
			break;
		default:
			break;
			
		}
	}
	close(sockfd);
	exit(0);
}
//-----------------------------------------------------------------------------------
//加密函数
void Encryption(char *str)
{
	srand(time(0));
	int a=rand()%10;
	int i=strlen(str);
	while(i)
	{
		str[i]=str[i-1]+a;
		i--;
	}
	str[0]=(char)(a+'0');
}
//-----------------------------------------------------------------------------------
//用户发送登录信息
void login(char *buff)
{
	strcpy(buff,"LGIN");
	printf("UserName: ");
	scanf("%s",UserName);

	printf("Password: ");
	scanf("%s",Password);	
	Encryption(Password);

	strcat(buff,UserName);
	strcat(buff,"#");
	strcat(buff,Password);
	strcat(buff,"\n");
}
//-----------------------------------------------------------------------------------
//用户发送共享文件信息
void ShareFile(char *buff)
{
	char 	PORT[10];
	char 	Filename[40];
	int 	fnum=0;

	strcpy(buff,"SHRE");
	printf("Portnum: ");
	scanf("%s",PORT);
	strcat(buff,PORT);
	strcat(buff,"#");
	
	printf("The number of file do you want to share: ");
	scanf("%d",&fnum);
	printf("File list to share :\n");
	int i=0,len;
	for(;i<fnum;i++)
	{
		scanf("%s",Filename);
		len=strlen(Filename);
		if(Filename[len-1]=='t'&&Filename[len-2]=='x'&&Filename[len-3]=='t'&&Filename[len-4]=='.')
		{
			if(i!=fnum-1)
				Filename[len++]='#';
			else
				Filename[len++]='\n';
			Filename[len]=0;
			strcat(buff,Filename);
		}
		else
		printf("EROR! The file must be .txt file!\n");
	}
}
//-----------------------------------------------------------------------------------
//用户发送查找文件信息
void SearchFile(char *buff)
{
	char 	Filename[40];
	strcpy(buff,"SRCH");
	printf("Enter the file name do you want to search: ");
	scanf("%s",Filename);
	strcat(buff,Filename);
	strcat(buff,"\n");
}
