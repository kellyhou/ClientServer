#include<sys/socket.h>
#include<sys/fcntl.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<time.h>
#include<map>
#include<string>
#include<iostream>
#include <pthread.h>
using namespace std;
//-----------------------------------------------------------------------------------

#define MAXLINE	128
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

//-----------------------------------------------------------------------------------
//用户信息
struct node
{
	char UserName[20];
	char Ipaddress[20];
	char portnum[10];
	char Filelist[MAXLINE];
};
//用于多线程传递信息
struct node2
{
	int a;
	char str[20];
};
//用UserName索引Password，快速确定用户合法性
map<string,string>UserInf;
//用文件名索引用户信息，快速查找
multimap<string,node*>SearchFile;

//-----------------------------------------------------------------------------------

void AddUser(char *UserName,char *Password);			//将database中的合法用户添加进来
void Decryption(string &str);					//解密函数
int Login(char *buff,char *message,node *p);			//处理用户登录请求
void ShareFile(char *buff,char *message,node *p);		//处理用户共享文件请求
void Searchfile(char *buff,char *message,int connfd);		//处理用户查找文件请求
void Quit(node *p);						//处理用户推出请求
void *thread(void *arg);					//多线程处理用户请求

//-----------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	
	char 			UserName[20];
	char			Password[10];
	int			listenfd, connfd;
	struct sockaddr_in	servaddr;
	struct sockaddr_in 	client;
	FILE *fileIn;
	
	UserInf.clear();
	SearchFile.clear();
	fileIn=fopen("User.txt","r");
	while(fscanf(fileIn,"%s%s",UserName,Password)!=EOF)
	{
		AddUser(UserName,Password);
	}
	fclose(fileIn);
	
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(2000);

	bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	listen(listenfd, 5);

	for ( ; ; ) {
		socklen_t len=sizeof(client);
		connfd = accept(listenfd, (struct sockaddr *) &client, &len);
		node2* np=new node2;
		np->a=connfd;
		strcpy(np->str,inet_ntoa(client.sin_addr));
		pthread_t a_thread;
		pthread_create(&a_thread, NULL, thread, (void *)np);
	}
	 pthread_mutex_destroy(&mutex1);
}
//-----------------------------------------------------------------------------------
//添加用户
void AddUser(char *UserName,char *Password)
{
	if(UserInf.find(UserName)==UserInf.end())
		UserInf[UserName]=Password;
	else
		printf("User name already exists!\n");
}
//-----------------------------------------------------------------------------------
//解密函数
void Decryption(string &str)
{
	string newstring="";
	int a=(int)str[0]-'0';
	int i=1;
	while(i<str.length())
	{
		newstring+=(char)(str[i++]-a);
	}
	str=newstring;
}
//-----------------------------------------------------------------------------------
//处理用户登录请求
int Login(char *buff,char *message,node *p)
{
	string 	UserName="";
	string 	Password="";
	int i=0;
	while(buff[i]!='#')
	{
		UserName+=buff[i];
		p->UserName[i]=buff[i++];
	}
	i++;
	while(buff[i]!='\n')
		Password+=buff[i++];
	Decryption(Password);
	
	if(UserInf.find(UserName)!=UserInf.end())
	{
		if(UserInf[UserName]==Password)
		{
			strcpy(message,"LRES Login success!\n");
			return 1;
		}
		else
			strcpy(message,"EROR Password ERROR!\n");
	}
	else strcpy(message,"EROR The user doesn't exists!\n");

	return 0;
}
//-----------------------------------------------------------------------------------
//处理用户共享文件请求
void ShareFile(char *buff,char *message,node *p)
{
	int i=0;
	while(buff[i]!='#')
		p->portnum[i]=buff[i++];
	buff[strlen(buff)-1]='#';
	strcpy(p->Filelist,buff+i);
	
	while(buff[i])
	{
		string FileName="";
		while(buff[i]!='#')
		{
			FileName+=buff[i++];
		}
		pthread_mutex_lock(&mutex1);
		SearchFile.insert(pair<string,node*>(FileName,p));
		pthread_mutex_unlock(&mutex1);
		i++;
	}
	strcpy(message,"SHOK\n");
}
//-----------------------------------------------------------------------------------
//处理用户查找文件请求
void Searchfile(char *buff,char *message,int connfd)
{
	buff[strlen(buff)-1]=0;
	
	string FileName="";
	for(int i=0;buff[i];i++)
		FileName+=buff[i];
	multimap<string,node*>::iterator it=SearchFile.find(FileName);
	int cnt=SearchFile.count(FileName);
	if(cnt!=0)
	{
	     
	     for(int i=0;i<cnt;i++,it++)
		{
			node *p=(*it).second;
			strcpy(message,"SRES");
			strcat(message,buff);
	     		strcat(message,"#");
			strcat(message,p->UserName);
			strcat(message,"#");
			strcat(message,p->Ipaddress);
			strcat(message,"#");
			strcat(message,p->portnum);
			strcat(message,"\n");
			write(connfd,message,strlen(message)+1);
			sleep(1);
		}	
	}
	strcpy(message,"SRES\n");
	write(connfd,message,strlen(message)+1);
}
//-----------------------------------------------------------------------------------
//处理用户退出请求
void Quit(node *p)
{
	int i=0;
	while(p->Filelist[i])
	{
		string FileName="";
		while(p->Filelist[i]!='#')
		{
			FileName+=p->Filelist[i++];
		}
		multimap<string,node*>::iterator it=SearchFile.find(FileName);
		if(it!=SearchFile.end())
		{
		     int cnt=SearchFile.count(FileName);
		     for(int j=0;j<cnt;j++,it++)
			{
				node *q=(*it).second;
				if(strcmp(p->UserName,q->UserName)==0)
					SearchFile.erase(it);
			}	
		}	
		i++;
	}
	delete p;
	p=NULL;	
}

//-----------------------------------------------------------------------------------
//多线程处理用户请求
void *thread(void *arg)
{
	string 			type;
	int 			n,connfd;
	char			buff[MAXLINE];
	char 			recvline[MAXLINE];
	node2 *np=(node2*)arg;
	connfd=np->a;
	node *p=new node;
	strcpy(p->Ipaddress,np->str);
	while((n=read(connfd,recvline,MAXLINE))>0)
	{
			int i=0,login;
			type="";
			for(i=0;i<4;i++)
				type+=recvline[i];
			if(type=="LGIN")
			{
				login=Login(recvline+4,buff,p);
				write(connfd,buff,strlen(buff)+1);
				if(login==0)
				{
					delete p;
					break;
				}
				else
					printf("%s Login!\n",p->UserName);
			}
			else if(type=="SHRE")
			{
				ShareFile(recvline+4,buff,p);
				write(connfd,buff,strlen(buff)+1);	
				printf("%s Share File!\n",p->UserName);
			}
			else if(type=="SRCH")
			{
				Searchfile(recvline+4,buff,connfd);
				printf("%s Search File!\n",p->UserName);
			}
			else if(type=="QUIT")
			{	
				printf("%s Quit!\n",p->UserName);
				Quit(p);				
				close(connfd);
			}
	}	
	
}
