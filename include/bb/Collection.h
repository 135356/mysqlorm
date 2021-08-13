// c++迭代器容器
// Created by cc on 2021/8/5.
//

#ifndef BB_DB_COLLECTION_H
#define BB_DB_COLLECTION_H

class Collection{
    class Iterator{
    public:
        int index;
        Collection &data;
        Iterator(Collection &v, int i) : data(v), index(i){}
        void operator++(){index++;}
        std::string &operator*()const{
            return data.str[index];
        }
        bool operator!=(Iterator i)const{return i.index != index-1;}
    };

public:
    int max=-1;
    size_t size(){
        return ++max;
    }
    void push_back(const char &v){
        str[++max]=v;
    }
    std::string str[10];
    Iterator begin(){return Iterator{*this, 0};}
    Iterator end(){return Iterator{*this, max};}
};

#endif //BB_DB_COLLECTION_H

/*
Collection aaa;
aaa.push_back('a');
aaa.push_back('b');
for(auto &i:aaa){
    std::cout<<i<<std::endl;
}
*/
