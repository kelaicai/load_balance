/*************************************************************************
	> File Name: ep_event.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月08日 星期日 16时04分12秒
 ************************************************************************/

#include<stdio.h>
#include"ep_event.h"
int set_noblocking(int fd)
{
    int old_option=fcntl(fd,F_GETFL);
    int new_option=old_option | O_NOBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}
void epoll_add(int epfd,int fd)
{
    struct epoll_event ev;
    ev.events=EPOLLIN | EPOLLET | EPOLLHUP;
    ev.data.fd=fd;
    if(epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev)==-1)
    {
        perror("epoll_ctl error");
    }
}

void epoll_del(int epfd,int fd)
{
    if(epoll_ctl(epfd,EPOLL_CTL,fd,NULL)==-1)
    {
        perror("epoll_ctl_del errorr");
    }
}
