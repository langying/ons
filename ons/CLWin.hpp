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
    // 170  文字开始显示的坐标x
    // 310  文字开始显示的坐标y
    // 30  每行显示的文字数目（全角）
    // 10  最大行数
    // 15  字体长
    // 15        字体宽
    // 0        字间距
    // 0        行间距
    // 1        单个字显示的毫秒数
    // 0        是否为粗体
    // 1        是否为阴影
    // #ffffff   窗体颜色 （可以吧#ffffff替换成图片，例如”win.png”,包括半角引号）
    // 170   窗体的左上x坐标
    // 200   左上y坐标
    // 640   右下x坐标
    // 450   右下y坐标
    void setwindow(int sx, int sy, int lw, int ln, int wh, int ww, int ws, int ls, int wt, int bold, int shad, int bgc, int lx, int ly, int rx, int ry);
    void spbtn(int id, int val);
    void textoff();
};

#endif /* CLWin_hpp */
