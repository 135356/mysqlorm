//
// Created by 邦邦 on 2022/7/8.
//
#ifndef BB_FILE_H
#define BB_FILE_H
#include <string>
#include <vector>
#include <sys/stat.h> //S_IRWXU
#include <unistd.h> //关闭文件描述符（close）

namespace bb{
    class file{
    public:
        static size_t mkdir_pF(std::string &dir){
            std::vector<std::string> dir_list{};
            std::string str;
            for(auto &v:dir){
                if(v == '.'){}else if(v == '/'){
                    if(!str.empty()){
                        dir_list.push_back(str);
                        str += '/';
                    }
                }else{
                    str+=v;
                }
            }
            if(!str.empty()){
                dir_list.push_back(str);
            }
            for(auto &v:dir_list){
                if(access(("./"+v).c_str(),F_OK) == -1){
                    mkdir(("./"+v).c_str(),S_IRWXU);
                }
            }
            return dir_list.size();
        }
    };
}

#endif //BB_FILE_H
