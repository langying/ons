//
//  CLImg.cpp
//  ons
//
//  Created by 韩琼 on 2019/12/16.
//  Copyright © 2019 韩琼. All rights reserved.
//

#include "CLImg.hpp"
#include "CLLex.hpp"
#include "CLVar.hpp"

void CLImg::setData(const std::string& val, CLOns* ons) {
    if (val[0]==':' && val[1]=='s') {
        size_t p = val.find(';');
        size_t idx = (p==std::string::npos) ? 3 : p+1;
        CLLex l(val.substr(idx));
        while (l.tk &&l.tk!=LEX_ID) {
            l.match(l.tk);
        }
        if (l.tk==LEX_ID) {
            auto lns = ons->root->findChildOrCreate(l.tkStr);
            data = val.substr(0, p+1) + lns->var->getString();
        }
        else {
            data = val;
        }
    } else {
        data = val;
    }
}

