//
//  main.cpp
//  CLite
//
//  Created by 韩琼 on 2019/11/27.
//  Copyright © 2019 韩琼. All rights reserved.
//

#include "CLEngine.hpp"

int main(int argc, const char * argv[]) {
    CLEngine engine;
    std::thread th([&engine](){
        engine.load("/Users/hanqiong/Downloads/迅雷下载/ONS/ONS");
        engine.execute();
    });
    
    while (true) {
        char str[16];
        fgets(str, sizeof(str), stdin);
        int num = atoi(str);
        engine.response(num);
    }
    return 0;
}


