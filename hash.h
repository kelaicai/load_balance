/*************************************************************************
	> File Name: hash.h
	> Author: 
	> Mail: 
	> Created Time: 2018年06月23日 星期六 13时06分28秒
 ************************************************************************/
#include<iostream>
#ifndef _HASH_H
#define _HASH_H
#endif

#include<map>
#include<list>
#include<vector>
#include<string>
#include"md5.h"
#include<iostream>

using namespace std;

//get md5 value
class Md5
{
    public:
    string get_md5(const string &str)
    {
        return MD5(str).toStr();
    }
};

//Created real_node and virtual_nod connection
template<typename T>
class Nodes
{
    public:
    //real key
    string real_key;
    //virtual Node
    vector<string>  others;
    T main_msg;
    Nodes(string key,const T &msg);
    Nodes(){  }
};

template<typename T>
class RealNodes
{
    public:
    typedef string (*Construct)(const T&,int);
    map<string,Nodes<T>> mapnodes;
    static Construct constructMsg;
    //add node and return keys
    
    string add_node(const T&msg);
    string add_vnode(string key,int num);
    static void setFunc(Construct cfun);
    vector<string> del_node(const T &msg);
    T get_msg(const string &key);
};

template<typename T>
typename RealNodes<T>::Construct RealNodes<T>::constructMsg=NULL;

template<typename T>
class MHash
{
    private:
    //key is node id, value is the real server
    map<string,string> hashmap;
    //real nodes
    RealNodes<T> rnodes;
    int init_num;
    public:
    MHash(int num,const vector<T> &msg);
    void add_server(const T &msg);
    void del_server(const T &msg);
    T find(const T &msg);
    void show_server();
};


//nodes
template<typename T>
Nodes<T>::Nodes(string key,const T& msg):real_key(key),main_msg(msg)
{
    
}
//RealNodes
template<typename T>
T RealNodes<T>::get_msg(const string &key)
{
    return mapnodes[key].main_msg;
}
template<typename T>
string RealNodes<T>::add_node(const T &msg)
{
    string res_key=Md5().get_md5(msg);
    mapnodes[res_key]=Nodes<T>(res_key,msg);
    return res_key;
}

template<typename T>
string RealNodes<T>::add_vnode(string key,int  num)
{
    const T &new_msg=RealNodes<T>::constructMsg(mapnodes[key].main_msg,num);
    string new_key=Md5().get_md5(new_msg);
    mapnodes[key].others.push_back(new_key);
    return new_key;
}

template<typename T>
void RealNodes<T>::setFunc(Construct cfun)
{
    constructMsg=cfun;
}


template<typename T>
vector<string> RealNodes<T>::del_node(const T &msg)
{
    string k=Md5().get_md5(msg);
    typename map<string,Nodes<T>>::iterator it=mapnodes.find(k);
    vector<string> resvec=it->others;
    resvec.push_back(k);
    mapnodes.erase(it);
    return resvec;
}

template<typename T>
MHash<T>::MHash(int num,const vector<T> &msg):init_num(num)
{
    if(num!=msg.size())
    return;

    string key;
    string new_key;
    int vnode_num=3;
    typename vector<T>::const_iterator cti=msg.begin();
    for(;cti!=msg.end();++cti)
    {
        key=rnodes.add_node(*cti);
        hashmap[key]=key;
        for(int i=0;i<3;++i)
        {
            new_key=rnodes.add_vnode(key,i);
            hashmap[new_key]=key;   //create connection
        }
    }
}

template<typename T>
void MHash<T>::add_server(const T &msg)
{
    string key=rnodes.add_node(msg);
    hashmap[key]=key;
    string new_key;
    for(int i=0;i<3;++i)
    {
        new_key=rnodes.add_vnode(key,i);
        hashmap[new_key]=key;
    }
}

template<typename T>
void MHash<T>::del_server(const T &msg)
{
    vector<string> vnode_key=rnodes.del_node(msg);
    for(int i=0;i<vnode_key.size();++i)
    {
        hashmap.erase(hashmap.find(vnode_key[i]));
    }
}

template<typename T>
T MHash<T>::find(const T &msg)
{
    string dest_key=Md5().get_md5(msg);
    typename map<string,string>::const_iterator cti=hashmap.begin();
    for(;cti!=hashmap.end();++cti)
    {
        if(cti->first>dest_key);
        {
            break;
        }
    }
    if(cti==hashmap.end())
    {
        cti=hashmap.begin();
    }
    return rnodes.get_msg(cti->second);
}

template<typename T>
void MHash<T>::show_server()
{
    cout<<"ALL NODES IS"<<endl;
    typename map<string,string>::const_iterator cti=hashmap.begin();
    for(;cti!=hashmap.end();++cti)
    {
        cout<<cti->first<<"---->"<<rnodes.get_msg(cti->second)<<endl;
    }
}


