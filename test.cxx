#include<iostream>
#include<cstring>
using namespace std;

int main()
{
    char buff[127]={0};
    int fd=0;
    sprintf(buff,"%d#",12);
    cout<<buff<<endl;
    char *p=strtok(buff,"#");
    sscanf(p,"%d",&fd);
    cout<<fd<<endl;
}
