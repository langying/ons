//
//  CLWin.cpp
//  ons
//
//  Created by 韩琼 on 2019/12/16.
//  Copyright © 2019 韩琼. All rights reserved.
//

#include "CLWin.hpp"

using namespace std;

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

void CLWin::lsp(CLImg &img) {
    imgs[img.id] = img;
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

void CLWin::spbtn(int id, int val) {
    imgs[id].v = val;
    cout << val << imgs[id].data << endl;
}

