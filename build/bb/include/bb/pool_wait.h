//
// Created by 邦邦 on 2022/4/22.
//

#ifndef BB_POOL_WAIT_H
#define BB_POOL_WAIT_H
#include <vector>
#include <list>
#include <mutex>
#include <future>
#include <thread>
#include <functional>

namespace bb{ //当全部线程阻塞时，空出线程后将没有运行的项目运行
    class pool_wait{
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
        size_t run_size{}; //正在运行的项目数量
        bool state_ = true; //为假停止阻塞
        explicit pool_wait(const unsigned short &max=3);
        ~pool_wait();
        void run(unsigned short &i);
        void push(const std::function<void()> &f,unsigned short i);
    public:
        static pool_wait &obj(const unsigned short &max=3);
        void push(const std::function<void()> &f);
        void close();
    };
}
#endif //BB_POOL_WAIT_H
