//
//  CLImg.hpp
//  ons
//
//  Created by 韩琼 on 2019/12/16.
//  Copyright © 2019 韩琼. All rights reserved.
//

#ifndef CLImg_hpp
#define CLImg_hpp

#include "base.hpp"

typedef struct _CLAnim {
    int anim;
    int time;
} CLAnim;

class CLImg {
public:
    int id;
    int a;
    CLPoint origin;
    
    int v; // 点击返回值
    std::string data;
    CLAnim anim;
};

class CLOgg {
public:
    int id;
    int status;
    std::string file;
};

#endif /* CLImg_hpp */
