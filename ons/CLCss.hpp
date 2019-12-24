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
    int top;
    int left;
    int width;
    int height;
    bool transparent;
    CLColor color; // 字体颜色
    CLColor backgroud_color;
    std::string background;
    std::string border;
    std::string display;
    std::string font_size;
    std::string font_style;
    std::string font_family;
    std::string font_weight;
    std::string line_height; // 行间距
    std::string margin;
    std::string padding;
    std::string position = "relative"; // absolute relative fixed static inherit
    std::string text_align;
};

#endif /* CLCss_hpp */
