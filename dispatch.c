#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<time.h>
#include<unistd.h>
#include<stdbool.h>
#include"hash.h"
#include"cppjson.h"
#include<sstream>
#include"ep_event.h"
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
void  send_dns_response(int sockfd,char* ip)
{
    int res=send(sockfd,ip,127,0);
    printf("res=%d",res);
}
string func(const string&str, int num) { 
  char buff[100] = "";
  sprintf(buff, "%s#%d", str.c_str(), num);
  return buff;
}

std::string get_data(const char *file_name)
{
    assert(file_name!=NULL);
    int fd=open(file_name,O_RDONLY);
    if(fd==-1)
    {
        perror("open error");
        return "";
    }
    std::string str;
    while(true)
    {
        char buff[128]={0};
        if(!read(fd,buff,127))
        {
            break;
        }
        str+=buff;
    }
    return str;
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
    set_noblocking(sockfd);
    int epfd=epoll_create(MAXFD);
    assert(epfd!=-1);

    epoll_add(epfd,sockfd);
	
	int serve_fd=socket(AF_INET,SOCK_STREAM,0);
    // connect to serve 7802
    RealNodes<string>::setFunc(func);
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

    json::Value val;
    int pipefd[2];
    res=pipe(pipefd):
    string str4json=get_data("./dispatch.json");
    std::istringstream ss(str4json);
    val.load_all(ss);
	struct sockaddr_in caddr;
	vector<string> vec;
    vec.push_back("127.0.0.1:7802");
    vec.push_back("127.0.0.1:7803");
    vec.push_back("127.0.0.1:7804");
    MHash<string> hash(3,vec);
    socklen_t len=sizeof(caddr);
	memset(&caddr,0,len);
    struct epoll_event events[MAXFD];
    while(1)
    {
        int n=epoll_wait(epfd,events,MAXFD,1000);
        if(n<0)
        {
            perror("epoll wait error");
        }
        if(n==0)
        {
            printf("timeout\n");
        }
        if(n>0)
        {
            for(int i=0;i<n;i++)
            {
                int fd=events[i].data.fd;
                if(fd==-1)
                {
                    continue;
                }
                if(events[i].events&EPOLLIN)
                {
                    if(fd==sockfd)
                    {
                        struct sockaddr_in caddr;
                        int len=sizeof(caddr);
                        int c=accept(fd,(struct sockaddr*)&caddr,&len);
                        if(c<0)
                        {
                            continue;
                        }                 
                        printf("accept=%d\n",c);
                        epoll_add(epfd,c);
                        char pip_buf[10]={0};
                        sprintf(pip_buf,"%d#",c);
                        if(write(pipefd[1],pip_buf,strlen(pip_buf))<0)
                        {
                            perror("write error");
                            break;//  need to add some codes
                        }
                    }
                    else   // need to wake up a thread to execute event
                    {
                        char buff[128]={0};
                        if((recv(events[i].data.fd,buff,1,0))<0=)
                        {
                            epoll_del(epfd,events[i].data.fd);
                            close(fd);
                        }
                        printf("buff=%s\n",buff);
                       // send(events[i].data.fd,"ok",127,0);
                    }
                }
            }
        }   
    }
/*	while(true)
	{
		int c=accept(sockfd,(struct sockaddr*)&caddr,&len);
		if(c<0)
		{
			continue;
		}
	
		fflush(stdout);
    //  int devied_port_choice=rand()%3;
        char* ptr;
     //   char* ip[]={"127.0.0.1#7802","127.0.0.1#7803","127.0.0.1#7804"};
    //    send_dns_response(c,ip[devied_port_choice]);
        char msg[1024]={0};
        char ipbuf[32]={0};
        int port=ntohs(caddr.sin_port);
        strcpy(ipbuf,inet_ntoa(caddr.sin_addr));
        sprintf(msg,"%s:%d",ipbuf,port);
        printf("msg=%s\n",msg);
        string record= hash.find(msg);
        cout<<record<<endl;
        int code=301;
        string url="http://";
        url=url+record;
        url=url+"/";
        send_response(c,code,url.c_str());
        
		close(c);
    }
    */
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



