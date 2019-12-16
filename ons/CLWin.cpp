//
//  CLWin.cpp
//  ons
//
//  Created by 韩琼 on 2019/12/16.
//  Copyright © 2019 韩琼. All rights reserved.
//

#include "CLWin.hpp"

void CLWin::lsp(CLImg &img) {
    imgs[img.id] = img;
}

void CLWin::spbtn(int id, int val) {
    imgs[id].v = val;
}

