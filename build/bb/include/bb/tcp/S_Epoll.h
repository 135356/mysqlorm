// 跨平台的epoll
// Created by 邦邦 on 2022/6/17.
//
#ifndef BB_S_EPOLL_H
#define BB_S_EPOLL_H
#include <cstring>
#include <csignal> //kill
#include <functional>
#include "bb/Log.h"
#include "IpTcp.h"
#include "bb/FloodIP.hpp"

namespace bb {
    class S_Epoll {
        IpTcp *tcp_{};
        const unsigned short EPOLL_MAX_SIZE{2000}; //epoll_fd最大值
        //添加epoll事件(往epoll(内核事件表)里添加事件)
        bool addEventF_(int &epoll_fd,int &client_fd,uint32_t &client_ip,const unsigned &events);
        //删除epoll事件
        bool deleteEventF_(int &epoll_fd, int &client_fd);
        //修改epoll事件
        bool modifyEventF_(int &epoll_fd, int &client_fd,const unsigned &events);
    public:
        S_Epoll(const int &port,const unsigned &timeout){
            tcp_ = new IpTcp({}, port,timeout);
        }
        ~S_Epoll(){
            delete tcp_;
        }
        //epoll使用
        void runF(const std::function<bool(int &client_fd,unsigned &client_ip)> &f);
    };
}

#endif //BB_S_EPOLL_H
