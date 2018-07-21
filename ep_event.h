/*************************************************************************
	> File Name: ep_event.h
	> Author: 
	> Mail: 
	> Created Time: 2018年07月08日 星期日 16时02分43秒
 ************************************************************************/

#ifndef _EP_EVENT_H
#define _EP_EVENT_H
#endif
#include<unistd.h>
#include<assert.h>
#include<sys/epoll.h>
#define MAXFD 10
void epoll_add(int epfd,int fd);
void epoll_del(int epfd,int fd);
