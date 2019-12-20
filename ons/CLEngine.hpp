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
#include "CLWin.hpp"
#include "CLVarLink.hpp"

enum ONS_TRANS_MODE {
    ONS_TRANS_MODE_ALPHA          = 1,
    ONS_TRANS_MODE_TOPLEFT        = 2,
    ONS_TRANS_MODE_COPY           = 3,
    ONS_TRANS_MODE_STRING         = 4,
    ONS_TRANS_MODE_DIRECT         = 5,
    ONS_TRANS_MODE_PALLETTE       = 6,
    ONS_TRANS_MODE_TOPRIGHT       = 7,
    ONS_TRANS_MODE_MASK           = 8,
};

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
    
    void reset();
    void load(const std::string& path);
    void addArc(ONS_ARC arc);
    void addTab(const std::string& name);
    void addTxt(const std::string& name, const std::string& charset, int mode);
    void getPathFiles(std::vector<std::string>& list, const std::string& ext1, const std::string& ext2);

    void execute();
    void response(int num);
    
public:
    void print(int anim, int time);
    bool isFile(const std::string& name);
    CLData getData(const std::string& name);
    
public:
    CLOns* ons;
    CLWin* win;
    uint8_t kTable[256];
    std::string path;
    std::string code;
    std::vector<CLTxt> txts;
    std::vector<std::string> stack;
    
    std::map<int, CLImg> imgs;
    std::map<std::string, CLNsa*> nsas;
};

#endif /* CLEngine_hpp */
