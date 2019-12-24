//
//  CLWin.cpp
//  ons
//
//  Created by 韩琼 on 2019/12/16.
//  Copyright © 2019 韩琼. All rights reserved.
//

#include "CLWin.hpp"

using namespace std;

CLWin::CLWin(CLOns* ons) {
    this->ons = ons;
}
void CLWin::br() {
}
void CLWin::csp(int id) {
    if (id < 0) {
        imgs.clear();
        return;
    }
    for (auto it = imgs.begin(); it != imgs.end(); it++) {
        if (id == it->second.id) {
            imgs.erase(it);
            break;
        }
    }
}
void CLWin::csp2(int id) {
    if (id < 0) {
        imgs.clear();
        return;
    }
    for (auto it = imgs.begin(); it != imgs.end(); it++) {
        if (id == it->second.id) {
            imgs.erase(it);
            break;
        }
    }
}
void CLWin::lsp(int id, string ss, int px, int py, int aa) {
    CLImg img;
    img.id = id;
    img.origin.x = px;
    img.origin.y = py;
    img.a = aa;
    img.setData(ss, ons);
    imgs[img.id] = img;
}

void CLWin::lsp2(int id, string ss, int px, int py, int sx, int sy, int rr, int aa) {
}

void CLWin::bgmstop() {
    bgm.status = 0;
}

void CLWin::dwave(CLOgg& ogg) {
    oggs[ogg.id] = ogg;
}

void CLWin::dwavestop(int id) {
    for (auto it = oggs.begin(); it != oggs.end(); it++) {
        if (id == it->second.id) {
            it->second.status = 0;
            break;
        }
    }
}

void CLWin::dwaveloop(CLOgg& ogg) {
    oggs[ogg.id] = ogg;
}

void CLWin::erasetextwindow(int id) {
    
}

void CLWin::jump(char op) {
    
}

void CLWin::msp(int id, int dx, int dy, int da) {
    
}

void CLWin::msp2(int id, int dx, int dy, int sx, int sy, int dr, int da) {
    
}

void CLWin::mpegplay(int id, std::string mov) {
}
/**
 * fh: 文字区字体高度
 * fw: 文字区字体宽度
 * dx: 文字区字间距
 * dy: 文字区行间距
 * bb:  bold
 * hh: 加粗
 * bg: 窗体背景颜色
*/
void CLWin::menusetwindow(int fh, int fw, int dx, int dy, int bb, int hh, int bg) {
    // fh hh
    menu["font-size"     ] = to_string(fw) + ONS_HTML;
    menu["letter-spacing"] = to_string(dx) + ONS_HTML;
    menu["line-height"   ] = to_string(dy) + ONS_HTML;
    menu["font-weight"   ] = bb ? "bold" : "normal";
    menu["background"    ] = "#" + to_string(bg);
}
/**
 * tl : 文字区left
 * tt: 文字区top
 * mc: 文字区每行字数
 * ml: 文字区最大行数
 * fh: 文字区字体高度
 * fw: 文字区字体宽度
 * dx: 文字区字间距
 * dy: 文字区行间距
 * st: 文字区显示速度：毫秒
 * bold:  文字区是否粗体
 * shad: 文字区是否阴影
 * bg: 窗体背景颜色
 * wl: 窗体
 * wt: 窗体
 * wr: 窗体
 * wb: 窗体
 */
void CLWin::setwindow(int tl, int tt, int mc, int ml, int fh, int fw, int dx, int dy, int st, int bold, int shad, int bg, int wl, int wt, int wr, int wb) {
    // fh shadow
    speed = st;
    label["top"     ] = to_string(wt) + ONS_HTML;
    label["left"    ] = to_string(wl) + ONS_HTML;
    label["width"   ] = to_string(wr - wl) + ONS_HTML;
    label["height"  ] = to_string(wb - wt) + ONS_HTML;
    label["background"      ] = "#" + to_string(bg);
    label["font-size"       ] = to_string(fw) + ONS_HTML;
    label["font-weight"     ] = bold ? "bold" : "normal";
    label["letter-spacing"  ] = to_string(dx) + ONS_HTML;
    label["line-height"     ] = to_string(dy) + ONS_HTML;
    label["padding"         ] = to_string(wl-tl) + " 0 " + to_string(wt-tt) + " 0";
}
void CLWin::setwindow(int tl, int tt, int mc, int ml, int fh, int fw, int dx, int dy, int st, int bold, int shad, const string& bg, int wl, int wt, int wr, int wb) {
    setwindow(tl, tt, mc, ml, fh, fw, dx, dy, st, bold, shad, 0, wl, wt, wr, wb);
    label["background"] = "url(" + bg + ")";
}
void CLWin::spbtn(int id, int val) {
    imgs[id].v = val;
    cout << val << imgs[id].data << endl;
}

void CLWin::textoff() {
}
