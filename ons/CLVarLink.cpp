//
//  CLVarLink.cpp
//  CLite
//
//  Created by 韩琼 on 2019/11/28.
//  Copyright © 2019 韩琼. All rights reserved.
//

#include "CLVar.hpp"
#include "CLVarLink.hpp"

using namespace std;

CLVarLink::~CLVarLink() {
#if DEBUG_MEMORY
    mark_deallocated(this);
#endif
    var->unref();
}

CLVarLink::CLVarLink(const CLVarLink &link) {
#if DEBUG_MEMORY
    mark_allocated(this);
#endif
    this->owned = false;
    this->name = link.name;
    this->var = link.var->ref();
    this->nextSibling = 0;
    this->prevSibling = 0;
}

CLVarLink::CLVarLink(CLVar *var, const std::string &name) {
#if DEBUG_MEMORY
    mark_allocated(this);
#endif
    this->owned = false;
    this->name = name;
    this->var = var->ref();
    this->nextSibling = 0;
    this->prevSibling = 0;
}

int CLVarLink::getIntName() {
    return atoi(name.c_str());
}

void CLVarLink::setIntName(int n) {
    char sIdx[64];
    sprintf_s(sIdx, sizeof(sIdx), "%d", n);
    name = sIdx;
}

void CLVarLink::replaceWith(CLVar *xin) {
    CLVar *old = var;
    var = xin->ref();
    old->unref();
}

void CLVarLink::replaceWith(CLVarLink *xin) {
    if (xin) {
        replaceWith(xin->var);
    } else {
        replaceWith(new CLVar());
    }
}
