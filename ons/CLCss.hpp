//
//  CLCss.hpp
//  ons
//
//  Created by 韩琼 on 2019/12/12.
//  Copyright © 2019 韩琼. All rights reserved.
//

#ifndef CLCss_hpp
#define CLCss_hpp

#include "base.hpp"

class CLCss {
public:
    CLSize font;
    CLSize size;
    CLPoint origin;
    uint32_t color1, color2, color3; // 高光，正常，低光
    uint32_t bgColor;
    bool bold;
    bool shadow;
    bool newline;
    bool transparent;
};

#endif /* CLCss_hpp */
