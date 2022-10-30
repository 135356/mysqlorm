#ifndef BB_SHAREMEMORY_H
#define BB_SHAREMEMORY_H
#include <iostream>
#include <cstring>
#include <sys/shm.h>

namespace bb{
    struct ShareMemory{
        key_t shmkey{};
        int shmid{};
        void *shmaddr{};
        size_t shmsize{1024};
        void openKey(const key_t &key=IPC_PRIVATE);
        void openId(const int &id);
        void close();
        //写
        void write(void *buf,const size_t &size);
        //删
        void del();
        //读
        void *get();
    };
    class ShareMemoryKey:public ShareMemory{
        ShareMemoryKey(const key_t &key=IPC_PRIVATE,const size_t &size=1024){
            shmsize = size;
            openKey(key);
        }
        ~ShareMemoryKey(){
            close();
        }
    public:
        //key为负数
        static ShareMemoryKey &obj(const key_t &key=IPC_PRIVATE,const size_t &size=1024){
            static ShareMemoryKey bb_share_memory_key(key,size);
            return bb_share_memory_key;
        }
    };
    class ShareMemoryId:public ShareMemory{
        ShareMemoryId(const int &id){
            openId(id);
        }
        ~ShareMemoryId(){
            close();
        }
    public:
        static ShareMemoryId &obj(const int &id){
            static ShareMemoryId bb_share_memory_id(id);
            return bb_share_memory_id;
        }
    };
}

#endif