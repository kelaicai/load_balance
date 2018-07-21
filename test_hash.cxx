#include "hash.h"
#include <stdio.h>

string func(const string&str, int num) { 
  char buff[100] = "";
  sprintf(buff, "%s#%d", str.c_str(), num);
  return buff;
}
int main() {
  vector<string> vs;
  vs.push_back("192.168.1.34:8000");
  vs.push_back("192.168.1.35:8000");
  vs.push_back("192.168.1.35:8001");
  vs.push_back("192.168.1.36:8003");
  vs.push_back("192.168.1.36:8004");
  vs.push_back("127.0.0.1:8000");

  /*******************************/
  RealNodes<string>::setFunc(func);
  MHash<string> hash(6, vs);
  hash.show_server();

  string str;
  std::cout << "Msg:";
  while (std::cin>>str) {
    std::cout << "GetServer:" << hash.find(str) << std::endl;
    std::cout << "Msg:";
  }
  return 0;
}
