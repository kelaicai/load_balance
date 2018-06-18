#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<time.h>
#include<unistd.h>
#include<stdbool.h>
int  port=7801;

int  serve_port=7802;
int create_socket();


int analysis_http_head(char str[]);
void send_response(int fd,int code,const char *url)
{
	char buff[1024]={0};
	char scode[16]={0};
	char status[32]="Move Permanently";

 // reponse proto
	strcat(buff,"HTTP/1.0");
	sprintf(scode,"%d",code);
	strcat(buff," ");
	strcat(buff,scode);
	strcat(buff," ");
	strcat(buff,status);
	strcat(buff,"\r\n");

  //serve msg
	strcat(buff,"Serve: ");
	strcat(buff,"My_serve");
	strcat(buff,"\r\n");
	
  //location
    strcat(buff,"location:");
	strcat(buff,url);
	strcat(buff,"\r\n");

  strcat(buff,"\r\n");

	int res=send(fd,buff,strlen(buff),0);
	printf("send status %d\n",res);
}
int main()
{
	//
    srand(time(0));
 	int sockfd=create_socket();	
 	if(sockfd<0)
 	{
		printf("socket serror");
		return -1;
 	}

	
	int serve_fd=socket(AF_INET,SOCK_STREAM,0);
    // connect to serve 7802
	struct sockaddr_in new_ser;
	memset(&new_ser,0,sizeof(new_ser));
	new_ser.sin_family=AF_INET;
	new_ser.sin_port=htons(serve_port);
	new_ser.sin_addr.s_addr=inet_addr("127.0.0.1");
    
	int client_sockfd=connect(serve_fd,(struct sockaddr*)&new_ser,sizeof(new_ser));
   	if(client_sockfd<0)
	{
		perror("connect error");
		return -1;
	}	
	 int res=listen(sockfd,5);
	assert(res!=-1);

	struct sockaddr_in caddr;
	int len=sizeof(caddr);
	memset(&caddr,0,len);
	while(true)
	{
		int c=accept(sockfd,(struct sockaddr*)&caddr,&len);
		if(c<0)
		{
			continue;
		}
		char buff[128]={0};
	
		int size=recv(c,buff,127,0);
		printf("buff =%s",buff);
		fflush(stdout);
        int code=301; 
        int devied_port_choice=rand()%3;
        printf("choice=%d\n",devied_port_choice);
        char *url[3]={"http://127.0.0.1:7802/","http://127.0.0.1:7803/","http://127.0.0.1:7804/"};

        send_response(c,code,url[devied_port_choice]);
		close(c);
	}
	close(sockfd);
	return 0;
}


int create_socket()
{
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{
		perror("socket error");
		return -1;
	}	
	struct sockaddr_in saddr;
	bzero(&saddr,sizeof(saddr));
    saddr.sin_family=AF_INET;
	saddr.sin_port=htons(port);
	saddr.sin_addr.s_addr=inet_addr("127.0.0.1");

	int res= bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
	if(res<0)
	{
		perror("bind error");
		return -1;
	}
	
	return sockfd;

}



