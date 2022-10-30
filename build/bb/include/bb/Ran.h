//
// Created by 邦邦 on 2022/4/22.
//

#ifndef BB_RAN_H
#define BB_RAN_H
#ifdef _MSC_VER
#elif __APPLE__
#include <uuid/uuid.h>
#elif __linux__
#endif
#include <algorithm> //shuffle方法需要
#include <string>
#include <random>
#include <array>

namespace bb{
    class Ran{
    public:
        //随机整数
        static unsigned getInt(const unsigned &min={},const unsigned &max={});
        //随机字符
        static std::string getString(const unsigned &min_number={},const unsigned &max_number={});
        //随机中文
        static std::string getZh(const unsigned &min_number={},const unsigned &max_number={});
        //使用uuid生成机器唯一ID
        //char dst[37]{};
        static char *getID(char *dst){
            #ifdef _MSC_VER
                /*char buffer[64] = { 0 };
                GUID guid;
                if(CoCreateGuid(&guid)){
                    fprintf(stderr, "create guid error ");
                    return '';
                }
                _snprintf(buffer, sizeof(buffer),
                "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
                guid.Data1, guid.Data2, guid.Data3,
                guid.Data4[0], guid.Data4[1], guid.Data4[2],
                guid.Data4[3], guid.Data4[4], guid.Data4[5],
                guid.Data4[6], guid.Data4[7]);
                return buffer;*/
            #elif __APPLE__
                uuid_t uu;
                uuid_generate(uu);
                uuid_unparse(uu,dst);
            return dst;
            #elif __linux__
            #endif
            return nullptr;
        }
        //随机不重复数
        static std::vector<int> getIntArr(const unsigned &max){
            //创建一个数组里面存入0-max数字
            std::vector<int> arr{};arr.reserve(max);for(unsigned i=0;i<max;i++){arr.push_back(i);}
            //随机打乱数组
            std::shuffle(arr.begin(),arr.end(),std::mt19937(std::random_device()()));
            return arr;
        }
        //获取随机数
        static unsigned long long getID1(unsigned int server_id){
            //获取全局唯一ID,server_id为服务的id,当同一个服务部署在多个服务器上时，需要区别
            auto getUniqueId = [](unsigned int server_id){
                unsigned long long seq_id = 0;
                unsigned long long seq = seq_id++ ;
                seq = (seq<<8);
                seq = (seq>>8);
                unsigned long long tag =((unsigned long long)server_id<<56);
                seq = (seq | tag);
                return seq;
            };
            //获取全局唯一ID,且ID为奇数
            unsigned long long id = getUniqueId(server_id);
            while(true){
                if((id & 0x01) == 0x01){
                    return id;
                }else{
                    id = getUniqueId(server_id);
                }
            }
        }
        //两个数拼接
        static long long append(int x, int y){
            long long sum = 0;
            int mid = y;
            sum += x * 10;
            for(;mid/=10;sum*=10);
            sum += y;
            return sum;
        }
    };
}
#endif //BB_RAN_H