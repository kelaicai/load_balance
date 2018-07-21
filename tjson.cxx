#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sstream>
#include<unistd.h>
#include"cppjson.h"
using namespace std;

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
    json::Value val;
    std::string str=get_data("./dispatch.json");
    std::istringstream ss(str);
    val.load_all(ss);
    std::cout<<val.get("dispatch").as_object()["ip"].as_string()<<std::endl;
    std::cout<<val.get("dispatch").as_object()["port"].as_integer()<<std::endl;
        
}


