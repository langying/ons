//
//  CLEngine.cpp
//  CLite
//
//  Created by 韩琼 on 2019/11/27.
//  Copyright © 2019 韩琼. All rights reserved.
//

#include <dirent.h>

#include "CLLex.hpp"
#include "CLVar.hpp"
#include "CLEngine.hpp"

using namespace std;

void ons_dump(std::string& func, CLVar* args, void* obj) {
    CLOns* ons = (CLOns*)obj;
    ons->root->trace(">  ");
}

void ons_nsa(CLVar* func, void* data) {
    CLEngine* engine = (CLEngine*)data;
    vector<string> list;
    engine->getPathFiles(list, ".nsa", ".NSA");
    for (string& name : list) {
        engine->loadNsa(name);
    }
}

CLEngine::~CLEngine() {
    if (ons) {
        delete ons;
    }
    for (auto it : nsas) {
        delete it.second;
    }
    nsas.clear();
}

CLEngine::CLEngine() {
    ons = NULL;
}

void CLEngine::load(const char* _path) {
    this->path = _path;
    // !TODO: 检查加密文件
    
    vector<string> list;
    getPathFiles(list, ".txt", ".TXT");
    for (string& name : list) {
        std::string pathfile = path + "/" + name;
        std::ifstream file(pathfile);
        std::istreambuf_iterator<char> begin(file);
        std::istreambuf_iterator<char> end;
        std::string text(begin, end);
        loadTxt(name, text);
    }
    
    ons = new CLOns(code);
    ons->addFunction("nsa", ons_nsa, this);
    ons->jump("*define");
}


void CLEngine::loadNsa(const string& name) {
    nsas[name] = new CLNsa(path + "/" + name);
}
void CLEngine::loadTxt(const string& name, const string& text) {
    CLTxt txt(name, text);
    if (code.length()){
        code += '\n';
    }
    code += text;
    txts.push_back(txt);
}

void CLEngine::execute() {
    ons->execute();
}

void CLEngine::response(int num) {
    ons->response(num);
}

void CLEngine::getPathFiles(vector<string>& list, const string& ext1, const string& ext2) {
    DIR* dir;
    struct dirent *ptr;
    if ((dir = opendir(path.c_str())) == NULL) {
        std::cout << "Open dir error:" << path;
        return;
    }
    
    while ((ptr = readdir(dir))) {
        std::string name = ptr->d_name;
        if (ptr->d_type != DT_REG || name == "." || name == "..") {
            continue;
        }
        if (endWith(name, ext1) || endWith(name, ext1)) {
            list.push_back(name);
        }
    }
    closedir(dir);
    sort(list.begin(), list.end());
}
