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
#include "CLImg.hpp"

typedef struct _CLEffect {
    int id;
    int effect;
    int duration;
    std::string image;
} CLEffect;

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
    
    CLCss menu;
    CLCss label;
    
    std::string bg;     // 背景图片
    std::string bgm;    // 背景音乐
    std::string kidoku; // 初次阅读，禁止跳过
    std::string caption;
    std::string textgosub;
    std::string transmode;
    std::string versionstr[2];
    std::vector<std::string> rmenu;
    std::vector<std::string> savename;
    std::map<int, CLImg> imgs;
    std::map<int, CLEffect> effects;

    
public:
    void lsp(CLImg& img);
    void spbtn(int id, int val);
};

#endif /* CLWin_hpp */
