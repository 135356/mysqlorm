//
// Created by 邦邦 on 2022/6/26.
//
#ifndef BB_TOKEN_H
#define BB_TOKEN_H
#include <set>
#include <string>
#include "Ran.h"
#include "Time.h"
#include "ssl/md5.hpp"
#include "ssl/base64.hpp"
#include "Log.h"

namespace bb{
    class Token{
        unsigned max_size_ = 10;
        std::string token_db_path_ = "./token.db";
        std::set<std::string> token_{};
        Token();
        ~Token();
    public:
        static Token &obj();
        void push(const char *accounts,std::string &token);
        void dec(std::string &en_token,std::string &token);
        bool is(std::string &token) const;
        bool rm(std::string &token);
    };
}
#endif //BB_TOKEN_H