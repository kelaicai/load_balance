/*************************************************************************
	> File Name: cli.c
	> Author: 
	> Mail: 
	> Created Time: 2018年06月18日 星期一 17时32分08秒
 ************************************************************************/

#include<stdio.h>
#include<assert.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<stdbool.h>


int port=7801;
int create_socket()
{
    int sockfd=socket(AF_IENT,SOCK_STREAM,0);
    if(sockfd<0)
    {
        perror("socket error");
        return -1;
    }
     struct sockaddr_in saddr;
    bzero(&saddr,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port(port);
    saddr.sin_addr.s_addr=inet_addr("127.0.0.1");

    int con=connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(con<0)
    {
        perror("connect error");
        return -1;
    }
    char buff[256]={0};
    while(1)
    {
        :wq
        if()
    }
    


}
int main()
{
    

}
