//
// Created by cc on 2021/4/30.
//

#ifndef BB_RAN_H
#define BB_RAN_H
#include <string>
#include <random>
#include <array>

namespace bb{
    class Ran{
    public:
        std::random_device seed; //種子產生器
        std::mt19937 engine = std::mt19937(seed()); //使用mt19937引擎
        std::array<char,26> str_en={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
        std::array<std::string,26> str_zh={"橣","芛","崤","滢","珏","茥","芪","瑾","龖","齉","龘","雦","爨","軉","灪","麤","鬻","彠","钥","鉴","韘","讋","正","道","题","展"};
        //随机整数
        int getInt();
        int getInt(const int &max);
        int getInt(const int &min,const int &max);
        //随机浮点数
        float getFloat();
        float getFloat(const float &max);
        float getFloat(const float &min,const float &max);
        double getDouble();
        double getDouble(const int &max);
        double getDouble(const int &min,const int &max);
        //随机字符
        std::string getString();
        std::string getString(const int &max);
        std::string getString(const int &min,const int &max);
        //随机中文
        std::string getZh();
        std::string getZh(const int &max);
        std::string getZh(const int &min,const int &max);
    };
}

#endif //BB_RAN_H