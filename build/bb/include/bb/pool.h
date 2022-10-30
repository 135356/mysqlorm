//
// Created by 邦邦 on 2022/4/22.
//

#ifndef BB_POOL_H
#define BB_POOL_H
#include <vector>
#include <list>
#include <mutex>
#include <future>
#include <thread>
#include <functional>
#include "pool_wait.h"

namespace bb{
    class pool{
        struct task_list{
            task_list():lock(mutex,std::defer_lock){}
            std::thread thread{};
            std::mutex mutex{};
            std::unique_lock<std::mutex> lock;
            std::condition_variable condition{};
            std::list<std::function<void()>> task{};
        };
        unsigned short index{}; //当前下标
        unsigned short work_max; //最大线程数
        std::vector<task_list> f_list{}; //任务
        bool state_ = true; //为假停止阻塞
        explicit pool(const unsigned short &max=3);
        ~pool();
        void run(unsigned short &i);
    public:
        static pool &obj(const unsigned short &max=3);
        //当全部线程阻塞时，空出线程后将没有运行的项目运行
        static pool_wait &objWait(const unsigned short &max=3);
        void push(const std::function<void()> &f);
        void close();
    };
}
#endif //BB_POOL_H
