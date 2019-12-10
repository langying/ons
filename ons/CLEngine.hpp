//
//  CLEngine.hpp
//  CLite
//
//  Created by 韩琼 on 2019/11/27.
//  Copyright © 2019 韩琼. All rights reserved.
//

#ifndef CLEngine_hpp
#define CLEngine_hpp

#include "CLNsa.hpp"
#include "CLOns.hpp"
#include "CLVarLink.hpp"

class CLTxt {
public:
    size_t size;
    std::string name;
    CLTxt(const std::string& name, const std::string& text) {
        this->name = name;
        this->size = text.length();
    }
};

class CLEngine {
public:
    ~CLEngine();
    CLEngine();
    
    void load(const char* path);
    void loadNsa(const std::string& name);
    void loadTxt(const std::string& name, const std::string& text);
    void getPathFiles(std::vector<std::string>& list, const std::string& ext1, const std::string& ext2);

public:
    void execute();
    void response(int num);
    
protected:
    CLOns* ons;
    std::string path;
    std::string code;
    std::vector<CLTxt> txts;
    std::vector<std::string> stack;
    std::map<std::string, CLNsa*> nsas;
};

#endif /* CLEngine_hpp */
