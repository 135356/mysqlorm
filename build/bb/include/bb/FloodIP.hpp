//
// Created by 邦邦 on 2022/6/28.
//
#ifndef BB_FLOODIP_H
#define BB_FLOODIP_H
#include "Flood.h"

struct FloodIP{
    bb::Flood a10;
    bb::Flood a20;
    bb::Flood b10;
    bb::Flood b20;
    static FloodIP &obj(){
        static FloodIP bb_flood_id;
        return bb_flood_id;
    }
private:
    FloodIP():a10(3600,10),a20(3600,20),b10(1800,10),b20(1800,20){}
    ~FloodIP()=default;
};

#endif //BB_FLOODIP_H
