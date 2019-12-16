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

class CLWin {
public:
    int savenumber;
    int lookbackcolor;
    
    std::string caption;
    std::string version1;
    std::string version2;
    std::string transmode;
    std::vector<std::string> rmenu;
    std::vector<std::string> savename;
    
    CLCss menu;
    CLCss label;
};

#endif /* CLWin_hpp */
