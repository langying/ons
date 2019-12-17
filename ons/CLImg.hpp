//
//  CLImg.hpp
//  ons
//
//  Created by 韩琼 on 2019/12/16.
//  Copyright © 2019 韩琼. All rights reserved.
//

#ifndef CLImg_hpp
#define CLImg_hpp

#include "CLOns.hpp"

typedef struct _CLAnim {
    int anim;
    int time;
} CLAnim;

class CLImg {
public:
    int id;
    int w;
    int h;
    int a;
    int n; // num of cells
    int v; // 点击返回值
    bool hidden;
    std::string data;
    CLAnim anim;
    CLPoint origin;
    
    void setData(const std::string& val, CLOns* ons);
};

class CLOgg {
public:
    int id;
    int status;
    std::string file;
};

#endif /* CLImg_hpp */
