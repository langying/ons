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
    int rmode; // 0右键点击无效，1右键点击有效
    int speed; // 每个字显示时间
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
    
    CLOgg bgm;    // 背景音乐
    CLOns* ons;
    
    std::string bg;     // 背景图片
    std::string kidoku; // 初次阅读，禁止跳过
    std::string caption;
    std::string textgosub;
    std::string transmode;
    std::string versionstr[2];
    std::vector<std::string> rmenu;
    std::vector<std::string> savename;
    
    std::map<int, CLImg> imgs;
    std::map<int, CLOgg> oggs;
    std::map<int, CLEffect> effects;
    std::map<std::string, std::string> menu;  // 右键菜单css
    std::map<std::string, std::string> label; // 文本框css

    
public:
    CLWin(CLOns* ons);
    void br();
    void csp(int id);
    void csp2(int id);
    void lsp(int id, std::string ss, int px, int py, int aa);
    void lsp2(int id, std::string ss, int px, int py, int sx, int sy, int rr, int aa);
    void bgmstop();
    void dwave(CLOgg& ogg);
    void dwavestop(int id);
    void dwaveloop(CLOgg& ogg);
    void erasetextwindow(int id);
    void jump(char op);
    void msp(int id, int dx, int dy, int da);
    void msp2(int id, int dx, int dy, int sx, int sy, int dr, int da);
    void mpegplay(int id, std::string mov);
    void menusetwindow(int fh, int fw, int dx, int dy, int bb, int hh, int bg);
    void setwindow(int tl, int tt, int mc, int ml, int fh, int fw, int dx, int dy, int st, int bold, int shad, int bg, int wl, int wt, int wr, int wb);
    void setwindow(int tl, int tt, int mc, int ml, int fh, int fw, int dx, int dy, int st, int bold, int shad, const std::string& bg, int wl, int wt, int wr, int wb);
    void spbtn(int id, int val);
    void textoff();
};

#endif /* CLWin_hpp */
