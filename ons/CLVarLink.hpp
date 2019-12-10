//
//  CLVarLink.hpp
//  CLite
//
//  Created by 韩琼 on 2019/11/28.
//  Copyright © 2019 韩琼. All rights reserved.
//

#ifndef CLVarLink_hpp
#define CLVarLink_hpp

#include "base.hpp"


class CLVarLink {
public:
    bool owned;
    std::string name;
    CLVar *var;
    CLVarLink *nextSibling;
    CLVarLink *prevSibling;
    
public:
    ~CLVarLink();
    CLVarLink(const CLVarLink &lns);
    CLVarLink(CLVar *var, const std::string &name = ONS_NAME);
    
    int  getIntName(); ///< Get the name as an integer (for arrays)
    void setIntName(int n); ///< Set the name as an integer (for arrays)
  
    void replaceWith(CLVar *newVar); ///< Replace the Variable pointed to
    void replaceWith(CLVarLink *newVar); ///< Replace the Variable pointed to (just dereferences)
};

#endif /* CLVarLink_hpp */
