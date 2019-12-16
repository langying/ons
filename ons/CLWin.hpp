//
//  CLWin.hpp
//  ons
//
//  Created by 韩琼 on 2019/12/16.
//  Copyright © 2019 韩琼. All rights reserved.
//

#ifndef CLWin_hpp
#define CLWin_hpp

#include "base.hpp"
#include "CLCss.hpp"

class CLWin {
public:
    int humanz; // 人物立绘的z-order
    int savenumber;
    int effectblank;
    int lookbackcolor;
    int automode_time;
    
    bool filelog;       // 文件跟踪记录
    bool automode;      // 支持自动播放
    bool globalon;      // 支持全局变量
    bool labellog;      // 标签跟踪记录
    bool windowback;    // 使立绘和文字属于同一蒙层
    
    std::string kidoku; // 初次阅读，禁止跳过
    std::string caption;
    std::string textgosub;
    std::string transmode;
    std::string versionstr[2];
    std::vector<std::string> rmenu;
    std::vector<std::string> savename;
    
    CLCss menu;
    CLCss label;
};

#endif /* CLWin_hpp */
