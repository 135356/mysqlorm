// tcp/ip协议
// Created by 邦邦 on 2022/6/17.
//
#ifndef BB_IPTCP_H
#define BB_IPTCP_H
#include <arpa/inet.h> //字节转换
#include <unistd.h> //关闭文件描述符（close）
#include <netinet/in.h> //网络结构体
#include <thread>
#include <vector>
#include "bb/Log.h"

namespace bb {
    class IpTcp{
    public:
        int socket_{}; //套接字
        struct sockaddr_in addr_{}; //结构体
        explicit IpTcp(const std::string &ip, const int &port,const unsigned &timeout=0);
        ~IpTcp();

        /** 结构体设置
         * ip={}为本机ip
         * port=0为不绑定端口，port=1为绑定预设端口,port>1绑定指定端口
         * type=4为ipV4协议类型，type=6为ipV6协议类型
         * second>0设置超时时间
         * addr_on=0不允许端口复用,addr_on=1允许端口复用
         * */
        void addrF(const std::string &ip={},const int &port=0,const int &type=4,const time_t &second=0,const int &addr_on=0);
    };
}

#endif //BB_IPTCP_H
