//
//  CLOns.cpp
//  CLite
//
//  Created by 韩琼 on 2019/11/29.
//  Copyright © 2019 韩琼. All rights reserved.
//

#include "CLVar.hpp"
#include "CLOns.hpp"

using namespace std;

/* -----------------------------------------LEX------------------------------------------- */
void ons_undefined(CLVar* func, void* data) {
    CLOns* ons = (CLOns*)data;
    string name = func->getString();
    std::cout << name << '(';
    for (int i = 0, l=func->getParameter(ONS_ARGC)->getInt(); i < l; i++) {
        if (i) {
            std::cout << ',';
        }
        std::string val = func->getParameter(to_string(i))->getString();
        if (val[0]==':' && val[1]=='s') {
            size_t p = val.find(';');
            size_t idx = p==string::npos ? 3 : p+1;
            CLLex l(val.substr(idx));
            while (l.tk &&l.tk!=LEX_ID) {
                l.match(l.tk);
            }
            if (l.tk==LEX_ID) {
                auto lns = ons->root->findChildOrCreate(l.tkStr);
                std::cout << l.tkStr << "_" << lns->var->getString();
            }
            else {
                std::cout << val;
            }
        } else {
            std::cout << val;
        }
    }
    std::cout << ");\n";
}
void lex_btnwait(CLVar* func, void* data) {
    int num = 0;
    ((CLOns*)data)->request(num);
    func->getParameter("0")->setInt(num);
    std::cout << func->getString() << "(" << num <<");\n";
}
void lex_btnwait2(CLVar* func, void* data) {
    lex_btnwait(func, data);
}
void ons_dialog(string text, void* data) {
    std::cout << text << "\n\n";
//    this_thread::sleep_for(chrono::milliseconds(100));
}
void ons_lsp(CLVar* func, void* data) {
    ons_undefined(func, data);
}
void lex_rmenu(CLVar* func, void* data) {
    ons_undefined(func, data);
}
void lex_systemcall(CLVar* func, void* data) {
    ons_undefined(func, data);
}

/* ----------------------------------------MATH-------------------------------------------- */
void math_add(CLVar* func, void* data) {
    CLVar* a = func->getParameter("0");
    CLVar* b = func->getParameter("1");
    if (a->isInt() && b->isInt()) {
        a->setInt(a->getInt() + b->getInt());
    }
    else {
        a->setString(a->getString() + b->getString());
    }
    std::cout << func->getString() << "();\n";
}
void math_atoi(CLVar* func, void* data) {
    CLVar* a = func->getParameter("0");
    CLVar* b = func->getParameter("1");
    a->setInt(atoi(b->getString().c_str()));
    std::cout << func->getString() << "();\n";
}
void math_dec(CLVar* func, void* data) {
    CLVar* a = func->getParameter("0");
    a->setInt(a->getInt() - 1);
    std::cout << func->getString() << "();\n";
}
void math_div(CLVar* func, void* data) {
    CLVar* a = func->getParameter("0");
    CLVar* b = func->getParameter("1");
    a->setInt(a->getInt() / b->getInt());
    std::cout << func->getString() << "();\n";
}
void math_inc(CLVar* func, void* data) {
    CLVar* a = func->getParameter("0");
    a->setInt(a->getInt() + 1);
    std::cout << func->getString() << "();\n";
}
void math_itoa(CLVar* func, void* data) {
    CLVar* a = func->getParameter("0");
    CLVar* b = func->getParameter("1");
    a->setString(to_string(b->getInt()));
    std::cout << func->getString() << "();\n";
}
void math_itoa2(CLVar* func, void* data) {
    math_itoa(func, data);
}
void math_len(CLVar* func, void* data) {
    CLVar* a = func->getParameter("0");
    CLVar* b = func->getParameter("1");
    a->setInt((int)b->getString().length());
    std::cout << func->getString() << "();\n";
}
void math_mid(CLVar* func, void* data) {
    CLVar* dst = func->getParameter("0");
    CLVar* src = func->getParameter("1");
    CLVar* idx = func->getParameter("2");
    CLVar* num = func->getParameter("3");
    dst->setString(src->getString().substr(idx->getInt(), num->getInt()));
    std::cout << func->getString() << "();\n";
}
void math_mod(CLVar* func, void* data) {
    CLVar* a = func->getParameter("0");
    CLVar* b = func->getParameter("1");
    a->setInt(a->getInt() % b->getInt());
    std::cout << func->getString() << "();\n";
}
void math_mov(CLVar* func, void* data) {
    CLVar* a = func->getParameter("0");
    CLVar* b = func->getParameter("1");
    a->copyValue(b);
    std::cout << func->getString() << "();\n";
}
void math_mul(CLVar* func, void* data) {
    CLVar* a = func->getParameter("0");
    CLVar* b = func->getParameter("1");
    a->setInt(a->getInt() * b->getInt());
    std::cout << func->getString() << "();\n";
}
void math_sub(CLVar* func, void* data) {
    CLVar* a = func->getParameter("0");
    CLVar* b = func->getParameter("1");
    a->setInt(a->getInt() - b->getInt());
    std::cout << func->getString() << "();\n";
}

/*-------------------------------------------------------------------------------------------*/
CLOns::~CLOns() {
    ASSERT(!l);
    scopes.clear();
    root->unref();
#if DEBUG_MEMORY
    show_allocated();
#endif
}

CLOns::CLOns(const string& code) {
    l = new CLLex(code);
    while(l->tk) {
        if (l->tk=='\n') {
            l->match('\n');
            if (l->tk==LEX_TAG) {
                labels[l->tkStr] = l->tokenStart;
                l->match(LEX_TAG);
            }
        } else {
            l->match(l->tk);
        }
    }
    
    root = (new CLVar(ONS_DATA, VAR_OBJECT))->ref();
    scopes.push_back(root);
    
    addFunction("add",      math_add,       this);
    addFunction("atoi",     math_atoi,      this);
    addFunction("dec",      math_dec,       this);
    addFunction("div",      math_div,       this);
    addFunction("inc",      math_inc,       this);
    addFunction("itoa",     math_itoa,      this);
    addFunction("itoa2",    math_itoa2,     this);
    addFunction("len",      math_len,       this);
    addFunction("mid",      math_mid,       this);
    addFunction("mod",      math_mod,       this);
    addFunction("mov",      math_mov,       this);
    addFunction("mul",      math_mul,       this);
    addFunction("sub",      math_sub,       this);

    addFunction("btnwait",      lex_btnwait,    this);
    addFunction("btnwait2",     lex_btnwait2,   this);
    addFunction("systemcall",   lex_systemcall, this);
}

void CLOns::back() {
    int pos = stacks.back();
    stacks.pop_back();
    l->reset(pos);

    CLVar* thiz = scopes.back();
    delete thiz;
    scopes.pop_back();
}

void CLOns::jump(const std::string &label) {
    auto it = labels.find(label);
    if (it == labels.end()) {
        throw new CLException("");
    }
    l->reset(it->second);
}

void CLOns::push(const std::string &label) {
    stacks.push_back(l->tokenStart);
    jump(label);
}

void CLOns::save() {
}

void CLOns::trace() {
    root->trace();
}

void CLOns::execute() {
    bool execute = true;
    while (l->tk) {
        statement(execute);
    }
}

void CLOns::execute(const string &code) {
    CLLex *oldLex = l;
    vector<CLVar*> oldScopes = scopes;
    l = new CLLex(code);
#ifdef TINYJS_CALL_STACK
    call_stack.clear();
#endif
    scopes.clear();
    scopes.push_back(root);
    try {
        bool execute = true;
        while (l->tk) {
            statement(execute);
        }
    } catch (CLException *e) {
        ostringstream msg;
        msg << "Error " << e->msg;
#ifdef TINYJS_CALL_STACK
        for (int i=(int)call_stack.size()-1;i>=0;i--) {
            msg << "\n" << i << ": " << call_stack.at(i);
        }
#endif
        msg << " at " << l->getPosition();
        delete l;
        l = oldLex;
        throw new CLException(msg.str());
    }
    delete l;
    l = oldLex;
    scopes = oldScopes;
}

void CLOns::request(int& val) {
    btnNum = std::promise<int>();
    val = btnNum.get_future().get();
}

void CLOns::response(int val) {
    btnNum.set_value(val);
}

void CLOns::addFunction(const string &name, CLCallback func, void *argv) {
    CLVar* var = new CLVar(name, VAR_FUNCTION | VAR_NATIVE);
    var->setCallback(func, argv);
    root->addChild(name, var);
}

string CLOns::evaluate(const string &code) {
    return evaluateComplex(code).var->getString();
}

CLVarLink CLOns::evaluateComplex(const string &code) {
    CLLex *oldLex = l;
    vector<CLVar*> oldScopes = scopes;

    l = new CLLex(code);
#ifdef TINYJS_CALL_STACK
    call_stack.clear();
#endif
    scopes.clear();
    scopes.push_back(root);
    CLVarLink *v = 0;
    try {
        bool execute = true;
        do {
            CLEAN(v);
            v = base(execute);
            if (l->tk!=LEX_EOF) {
                l->match(':');
            }
        } while (l->tk!=LEX_EOF);
    }
    catch (CLException *e) {
        ostringstream msg;
        msg << "Error " << e->msg;
#ifdef TINYJS_CALL_STACK
        for (int i=(int)call_stack.size()-1;i>=0;i--) {
            msg << "\n" << i << ": " << call_stack.at(i);
        }
#endif
        msg << " at " << l->getPosition();
        delete l;
        l = oldLex;
        throw new CLException(msg.str());
    }
    delete l;
    l = oldLex;
    scopes = oldScopes;

    if (v) {
        CLVarLink r = *v;
        CLEAN(v);
        return r;
    }
    // return undefined...
    return CLVarLink(new CLVar());
}

CLVar *CLOns::getVar(const string &path) {
    // traverse path
    size_t prevIdx = 0;
    size_t thisIdx = path.find('.');
    if (thisIdx == string::npos) {
        thisIdx = path.length();
    }
    CLVar *var = root;
    while (var && prevIdx<path.length()) {
        string sub = path.substr(prevIdx, thisIdx-prevIdx);
        CLVarLink* lns = var->findChild(sub);
        var = lns?lns->var:0;
        prevIdx = thisIdx+1;
        thisIdx = path.find('.', prevIdx);
        if (thisIdx == string::npos) {
            thisIdx = path.length();
        }
    }
    return var;
}

const string *CLOns::getVariable(const string &path) {
    CLVar *var = getVar(path);
    if (var) {
        return &var->getString();
    } else {
        return NULL;
    }
}

bool CLOns::setVariable(const std::string &path, const std::string &text) {
    CLVar *var = getVar(path);
    if (var) {
        if (var->isInt()) {
            var->setInt((int)strtol(text.c_str(),0,0));
        } else if (var->isDouble()) {
            var->setDouble(strtod(text.c_str(),0));
        } else {
            var->setString(text.c_str());
        }
        return true;
    }
    else {
        return false;
    }
}

void CLOns::block(bool &execute) {
    l->match('{');
    if (execute) {
        while (l->tk && l->tk!='}') {
            statement(execute);
        }
        l->match('}');
    }
    else {
        int brackets = 1;
        while (l->tk && brackets) {
            if (l->tk == '{') brackets++;
            if (l->tk == '}') brackets--;
            l->match(l->tk);
        }
    }
}

void CLOns::statement(bool &execute) {
    if (l->tk==':' || l->tk=='\n') {
        /* Empty statement - to allow things like ::: \n\n\n */
        l->match(l->tk);
    }
    else if (l->tk==LEX_TAG) {
        while (l->tk && l->tk!='\n') {
            l->match(l->tk);
        }
    }
    else if (l->tk==LEX_TXT) {
        ons_dialog(l->tkStr, this);
        l->match(l->tk);
    }
    else if (l->tk==LEX_ID) {
        CLVarLink* func = root->findChildOrCreate(l->tkStr);
        if (func->var->isFunction() || func->var->isUndefined()) {
            l->match(LEX_ID);
            CLEAN(callFunction(execute, func));
        }
        else {
            CLEAN(base(execute));
        }
        CLEAN(func);
    }
    else if (l->tk==LEX_R_IF) {
        l->match(LEX_R_IF);
        CLVarLink *var = base(execute);
        bool cond = execute && var->var->getBool();
        CLEAN(var);
        
        if (l->tk==',') {
            // 兼容不规范写法
            l->match(l->tk);
        }
        
        if (!cond) {
            while (l->tk && l->tk!='\n') {
                l->match(l->tk);
            }
        }
    } else if (l->tk==LEX_R_FOR) {
        l->match(LEX_R_FOR);
        int idx = 0;
        int pos = l->tokenStart;
        while(true) {
            CLVarLink* ret = factor(execute);
            l->match('=');
            CLVarLink* aaa = base(execute);
            l->match(LEX_R_TO);
            CLVarLink* bbb = base(execute);
            
            ret->var->setInt(aaa->var->getInt() + idx++);
            bool con = ret->var->getInt() > bbb->var->getInt();
            CLEAN(ret);
            CLEAN(aaa);
            CLEAN(bbb);
            if (con) {
                break;
            }
            while(l->tk!=LEX_R_NEXT) {
                statement(execute);
            }
            l->match(LEX_R_NEXT);
            l->reset(pos);
        }
        while(l->tk!=LEX_R_NEXT) {
            l->match(l->tk);
        }
        l->match(LEX_R_NEXT);
    }
    else if (l->tk==LEX_R_DEFSUB) {
        l->match(LEX_R_DEFSUB);
        root->addChildNoDup(l->tkStr, new CLVar("*" + l->tkStr, VAR_FUNCTION));
        l->match(l->tk);
    }
    else if (l->tk==LEX_R_GOTO) {
        l->match(l->tk);
        string label = l->tkStr;
        l->match(l->tk);
        jump(label);
    }
    else if (l->tk==LEX_R_GOSUB) {
        l->match(l->tk);
        statement(execute);
    }
    else if (l->tk==LEX_R_RETURN) {
        l->match(LEX_R_RETURN);
        back();
    }
    else if (l->tk==LEX_R_GETPARAM) {
        l->match(LEX_R_GETPARAM);
        int idx = 0;
        auto func = scopes.back();
        while (l->tk && l->tk!=':' && l->tk!='\n') {
            CLVar* val = func->getParameter(to_string(idx++));
            CLVarLink* lns = root->findChildOrCreate(l->tkStr);
            lns->var->copyValue(val);
            l->match(l->tk);
            if (l->tk==',') {
                l->match(l->tk);
            }
            CLEAN(lns);
        }
    }
    else if (l->tk==LEX_R_NUMALIAS) {
        l->match(LEX_R_NUMALIAS);
        auto xin = root->findChildOrCreate(l->tkStr, VAR_INTEGER);
        
        l->match(LEX_ID);
        l->match(',');
        auto old = root->findChildOrCreate("%" + l->tkStr, VAR_INTEGER);
        
        xin->replaceWith(old);
        l->match(l->tk);
        
        CLEAN(xin);
        CLEAN(old);
    }
    else {
        l->match(LEX_EOF);
    }
}

CLVarLink *CLOns::factor(bool &execute) {
    if (l->tk=='(') {
        l->match('(');
        CLVarLink *a = base(execute);
        l->match(')');
        return a;
    }
    if (l->tk==LEX_ID) {
        CLVar *parent = 0;
        CLVarLink *a = root->findChildOrCreate(l->tkStr);
        l->match(LEX_ID);
        while ( l->tk=='(' || l->tk=='.' || l->tk=='[' ) {
            if (l->tk=='(') { // ------------------------------------- Function Call
                a = callFunction(execute, a);
            }
            else if (l->tk == '.') { // ------------------------------------- Record Access
                l->match('.');
                if (execute) {
                    const string &name = l->tkStr;
                    CLVarLink *child = a->var->findChild(name);
                    if (!child) {
                        /* if we haven't found this defined yet, use the built-in 'length' properly */
                        if (a->var->isArray() && name == "length") {
                            int l = a->var->getArrayLength();
                            child = new CLVarLink(new CLVar(l));
                        }
                        else if (a->var->isString() && name == "length") {
                            int l = (int)a->var->getString().size();
                            child = new CLVarLink(new CLVar(l));
                        }
                        else {
                            child = a->var->addChild(name);
                        }
                    }
                    parent = a->var;
                    a = child;
                }
                l->match(LEX_ID);
            }
            else if (l->tk == '[') { // ------------------------------------- Array Access
                l->match('[');
                CLVarLink* index = base(execute);
                l->match(']');
                if (execute) {
                    CLVarLink *child = a->var->findChildOrCreate(index->var->getString());
                    parent = a->var;
                    a = child;
                }
                CLEAN(index);
            }
            else {
                ASSERT(0);
            }
        }
        return a;
    }
    if (l->tk==LEX_INT || l->tk==LEX_FLOAT) {
        CLVar *a = new CLVar(l->tkStr, ((l->tk==LEX_INT) ? VAR_INTEGER : VAR_DOUBLE));
        l->match(l->tk);
        return new CLVarLink(a);
    }
    if (l->tk==LEX_STR) {
        CLVar *a = new CLVar(l->tkStr, VAR_STRING);
        l->match(LEX_STR);
        return new CLVarLink(a);
    }
    if (l->tk==LEX_TAG) {
        CLVar *a = new CLVar(l->tkStr, VAR_LABEL);
        l->match(LEX_TAG);
        return new CLVarLink(a);
    }
    if (l->tk=='{') {
        CLVar *contents = new CLVar(ONS_DATA, VAR_OBJECT);
        /* JSON-style object definition */
        l->match('{');
        while (l->tk != '}') {
            string id = l->tkStr;
            // we only allow strings or IDs on the left hand side of an initialisation
            if (l->tk==LEX_STR) {
                l->match(LEX_STR);
            }
            else {
                l->match(LEX_ID);
            }
            l->match(':');
            if (execute) {
                CLVarLink *a = base(execute);
                contents->addChild(id, a->var);
                CLEAN(a);
            }
            // no need to clean here, as it will definitely be used
            if (l->tk != '}') {
                l->match(',');
            }
        }
        l->match('}');
        return new CLVarLink(contents);
    }
    if (l->tk=='[') {
        CLVar *contents = new CLVar(ONS_DATA, VAR_ARRAY);
        /* JSON-style array */
        l->match('[');
        int idx = 0;
        while (l->tk != ']') {
            if (execute) {
                char idx_str[16]; // big enough for 2^32
                sprintf_s(idx_str, sizeof(idx_str), "%d",idx);

                CLVarLink *a = base(execute);
                contents->addChild(idx_str, a->var);
                CLEAN(a);
            }
            // no need to clean here, as it will definitely be used
            if (l->tk != ']') l->match(',');
            idx++;
        }
        l->match(']');
        return new CLVarLink(contents);
    }
    // Nothing we can do here... just hope it's the end...
    l->match(LEX_EOF);
    return 0;
}

CLVarLink *CLOns::unary(bool &execute) {
    CLVarLink *a;
    if (l->tk=='!') {
        l->match('!'); // binary not
        a = factor(execute);
        if (execute) {
            CLVar zero(0);
            CLVar *res = a->var->mathsOp(&zero, LEX_EQUAL);
            CREATE_LINK(a, res);
        }
    }
    else {
        a = factor(execute);
    }
    return a;
}

CLVarLink *CLOns::term(bool &execute) {
    CLVarLink *a = unary(execute);
    while (l->tk=='*' || l->tk=='/' || l->tk=='%') {
        int op = l->tk;
        l->match(l->tk);
        CLVarLink *b = unary(execute);
        if (execute) {
            CLVar *res = a->var->mathsOp(b->var, op);
            CREATE_LINK(a, res);
        }
        CLEAN(b);
    }
    return a;
}

CLVarLink *CLOns::expression(bool &execute) {
    bool negate = false;
    if (l->tk=='-') {
        l->match('-');
        negate = true;
    }
    CLVarLink *a = term(execute);
    if (negate) {
        CLVar zero(0);
        CLVar *res = zero.mathsOp(a->var, '-');
        CREATE_LINK(a, res);
    }

    while (l->tk=='+' || l->tk=='-' || l->tk==LEX_PLUSPLUS || l->tk==LEX_MINUSMINUS) {
        int op = l->tk;
        l->match(l->tk);
        if (op==LEX_PLUSPLUS || op==LEX_MINUSMINUS) {
            if (execute) {
                CLVar one(1);
                CLVar *res = a->var->mathsOp(&one, op==LEX_PLUSPLUS ? '+' : '-');
                CLVarLink *oldValue = new CLVarLink(a->var);
                // in-place add/subtract
                a->replaceWith(res);
                CLEAN(a);
                a = oldValue;
            }
        } else {
            CLVarLink *b = term(execute);
            if (execute) {
                // not in-place, so just replace
                CLVar *res = a->var->mathsOp(b->var, op);
                CREATE_LINK(a, res);
            }
            CLEAN(b);
        }
    }
    return a;
}

CLVarLink *CLOns::shift(bool &execute) {
    CLVarLink *a = expression(execute);
    if (l->tk==LEX_LSHIFT || l->tk==LEX_RSHIFT || l->tk==LEX_RSHIFTUNSIGNED) {
        int op = l->tk;
        l->match(op);
        CLVarLink *b = base(execute);
        int shift = execute ? b->var->getInt() : 0;
        CLEAN(b);
        if (execute) {
            if (op==LEX_LSHIFT)         { a->var->setInt(a->var->getInt() << shift); }
            if (op==LEX_RSHIFT)         { a->var->setInt(a->var->getInt() >> shift); }
            if (op==LEX_RSHIFTUNSIGNED) { a->var->setInt(((unsigned int)a->var->getInt()) >> shift); }
        }
    }
    return a;
}

CLVarLink *CLOns::condition(bool &execute) {
    CLVarLink *a = shift(execute);
    CLVarLink *b;
    while (l->tk=='<'           || l->tk=='>'           ||
           l->tk==LEX_EQUAL     || l->tk==LEX_NEQUAL    ||
           l->tk==LEX_LEQUAL    || l->tk==LEX_GEQUAL    ||
           l->tk==LEX_TYPEEQUAL || l->tk==LEX_NTYPEEQUAL) {
        int op = l->tk;
        l->match(l->tk);
        b = shift(execute);
        if (execute) {
            CLVar *res = a->var->mathsOp(b->var, op);
            CREATE_LINK(a,res);
        }
        CLEAN(b);
    }
    return a;
}

CLVarLink *CLOns::logic(bool &execute) {
    CLVarLink *a = condition(execute); // 条件符
    CLVarLink *b;
    while (l->tk=='&' || l->tk=='|' || l->tk=='^' || l->tk==LEX_ANDAND || l->tk==LEX_OROR) {
        bool noexecute = false;
        int op = l->tk;
        l->match(l->tk);
        bool shortCircuit = false;
        bool boolean = false;
        // if we have short-circuit ops, then if we know the outcome
        // we don't bother to execute the other op. Even if not
        // we need to tell mathsOp it's an & or |
        if (op==LEX_ANDAND) {
            op = '&';
            shortCircuit = !a->var->getBool();
            boolean = true;
        }
        else if (op==LEX_OROR) {
            op = '|';
            shortCircuit = a->var->getBool();
            boolean = true;
        }
        b = condition(shortCircuit ? noexecute : execute);
        if (execute && !shortCircuit) {
            if (boolean) {
                CLVar *newa = new CLVar(a->var->getBool());
                CLVar *newb = new CLVar(b->var->getBool());
                CREATE_LINK(a, newa);
                CREATE_LINK(b, newb);
            }
            CLVar *res = a->var->mathsOp(b->var, op);
            CREATE_LINK(a, res);
        }
        CLEAN(b);
    }
    return a;
}

CLVarLink *CLOns::ternary(bool &execute) {
    CLVarLink *lhs = logic(execute); // 位运算
    bool noexec = false;
    if (l->tk=='?') {
        l->match('?');
        if (!execute) {
            CLEAN(lhs);
            CLEAN(base(noexec));
            l->match(':');
            CLEAN(base(noexec));
        }
        else {
            bool first = lhs->var->getBool();
            CLEAN(lhs);
            if (first) {
                lhs = base(execute);
                l->match(':');
                CLEAN(base(noexec));
            }
            else {
                CLEAN(base(noexec));
                l->match(':');
                lhs = base(execute);
            }
        }
    }
    return lhs;
}

CLVarLink *CLOns::base(bool &execute) {
    CLVarLink *lhs = ternary(execute); // 三元运算符
    if (l->tk=='=' || l->tk==LEX_PLUSEQUAL || l->tk==LEX_MINUSEQUAL) {
        /* If we're assigning to this and we don't have a parent,
         * add it to the symbol table root as per JavaScript. */
        if (execute && !lhs->owned) {
            if (lhs->name.length()>0) {
                CLVarLink *realLhs = root->addChildNoDup(lhs->name, lhs->var);
                CLEAN(lhs);
                lhs = realLhs;
            }
            else {
                TRACE("Trying to assign to an un-named type\n");
            }
        }

        int op = l->tk;
        l->match(l->tk);
        CLVarLink *rhs = base(execute);
        if (execute) {
            if (op=='=') {
                // !TODO:如果是别名，会出问题的
                lhs->replaceWith(rhs);
            }
            else if (op==LEX_PLUSEQUAL) {
                CLVar *res = lhs->var->mathsOp(rhs->var, '+');
                lhs->replaceWith(res);
            }
            else if (op==LEX_MINUSEQUAL) {
                CLVar *res = lhs->var->mathsOp(rhs->var, '-');
                lhs->replaceWith(res);
            }
            else {
                ASSERT(0);
            }
        }
        CLEAN(rhs);
    }
    return lhs;
}

/** Handle a function call (assumes we've parsed the function name and we're
 * on the start bracket). 'parent' is the object that contains this method,
 * if there was one (otherwise it's just a normnal function).
 */
CLVarLink *CLOns::callFunction(bool &execute, CLVarLink* function) {
    CLVar* func = new CLVar(function->name, VAR_FUNCTION);
    int num = 0;
    while (l->tk && l->tk!=':' && l->tk!='\n') {
        string key = to_string(num++);
        CLVarLink* lns = NULL;
        if (l->tk==',') {
            lns = new CLVarLink(new CLVar());
        } else {
            lns = base(execute);
        }
        func->addChild(key, lns->var);
        CLEAN(lns);
        if (l->tk==',') {
            l->match(',');
            if (l->tk==0 || l->tk==':' || l->tk=='\n') {
                func->addChild(to_string(num++), new CLVar());
            }
        }
    }
    func->addChild(ONS_ARGC, new CLVar(num));

#ifdef TINYJS_CALL_STACK
    call_stack.push_back(function->name + " from " + l->getPosition());
#endif

    scopes.push_back(func);
    if (function->var->isUndefined()) {
        ons_undefined(func, this);
        scopes.pop_back();
        delete func;
#ifdef TINYJS_CALL_STACK
        if (!call_stack.empty()) { call_stack.pop_back(); }
#endif
    } else if (function->var->isNative()) {
        function->var->callback(func, function->var->userdata);
        scopes.pop_back();
        delete func;
#ifdef TINYJS_CALL_STACK
        if (!call_stack.empty()) { call_stack.pop_back(); }
#endif
    } else {
        push(function->var->getString());
    }
    return new CLVarLink(new CLVar());
}
