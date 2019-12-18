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

void CLWin::setwindow(int sx, int sy, int lw, int ln, int wh, int ww, int ws, int ls, int wt, int bold, int shad, int bgc, int lx, int ly, int rx, int ry) {
}
void CLWin::spbtn(int id, int val) {
    imgs[id].v = val;
    cout << val << imgs[id].data << endl;
}

void CLWin::textoff() {
}
