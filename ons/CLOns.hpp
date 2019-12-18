//
//  CLOns.hpp
//  CLite
//
//  Created by 韩琼 on 2019/11/29.
//  Copyright © 2019 韩琼. All rights reserved.
//

#ifndef CLOns_hpp
#define CLOns_hpp

#include "CLLex.hpp"
#include "CLVarLink.hpp"

class CLOns {
public:
    ~CLOns();
    CLOns(const std::string& code);
    void back();
    void jump(const std::string& lable);
    void push(const std::string& label);
    void save();
    void trace();
    void execute();
    void execute(const std::string &code);
    void request(int& val); // 线程间通信时，采用这个函数来回调
    void response(int val); // 线程间通信时，采用这个函数来回调
    void addFunction(const std::string& name, CLCallback func, void* data);

    std::string evaluate(const std::string &code);
    CLVarLink   evaluateComplex(const std::string &code);
    
    CLVar*              getVar(const std::string &path);
    const std::string*  getVariable(const std::string &path);
    bool                setVariable(const std::string &path, const std::string &data);

public:
    CLVar *root;

private:
    CLLex *l;                           /// current lexer
    std::vector<int>           record;  /// 每次跳转买点记录
    std::vector<int>           stacks;  /// 每次跳转买点记录
    std::vector<CLVar*>        scopes;  /// stack of scopes when parsing
    std::promise<int>          btnNum;  /// btnwait num
    std::map<std::string, int> labels;  /// Label Index In Script
    
    std::vector<int> input = {1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 2, 2, 2, 1, 1, 2, 1, 1, 2, 3, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 2, 1, 1};   /// 每次跳转买点记录
    
#ifdef TINYJS_CALL_STACK
    std::vector<std::string> call_stack; /// Names of places called so we can show when erroring
#endif
    
    void block(bool &execute);
    void statement(bool &execute);

    // parsing - in order of precedence
    CLVarLink *factor(bool &execute);
    CLVarLink *unary(bool &execute);
    CLVarLink *term(bool &execute);
    CLVarLink *expression(bool &execute);
    CLVarLink *shift(bool &execute);
    CLVarLink *condition(bool &execute);
    CLVarLink *logic(bool &execute);
    CLVarLink *ternary(bool &execute);
    CLVarLink *base(bool &execute);
    CLVarLink *callFunction(bool &execute, CLVarLink *function);
};

#endif /* CLOns_hpp */
