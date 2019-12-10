//
//  CLVar.cpp
//  CLite
//
//  Created by 韩琼 on 2019/11/28.
//  Copyright © 2019 韩琼. All rights reserved.
//

#include "CLLex.hpp"
#include "CLVar.hpp"
#include "CLVarLink.hpp"

using namespace std;

CLVar::~CLVar(void) {
#if DEBUG_MEMORY
    mark_deallocated(this);
#endif
    removeAllChildren();
}

CLVar::CLVar() {
    refs = 0;
#if DEBUG_MEMORY
    mark_allocated(this);
#endif
    init();
    flags = VAR_UNDEFINED;
}

CLVar::CLVar(int val) {
    refs = 0;
#if DEBUG_MEMORY
    mark_allocated(this);
#endif
    init();
    setInt(val);
}

CLVar::CLVar(double val) {
    refs = 0;
#if DEBUG_MEMORY
    mark_allocated(this);
#endif
    init();
    setDouble(val);
}

CLVar::CLVar(const string &str) {
    refs = 0;
#if DEBUG_MEMORY
    mark_allocated(this);
#endif
    init();
    flags = VAR_STRING;
    data = str;
}

CLVar::CLVar(const string &str, int flg) {
    refs = 0;
#if DEBUG_MEMORY
    mark_allocated(this);
#endif
    init();
    flags = flg;
    if (flg & VAR_INTEGER) {
        if (str[0]=='#') {
            string num = "0x" + str.substr(1);
            intData = strtol(num.c_str(), 0, 0);
        } else {
            intData = strtol(str.c_str(), 0, 0);
        }
    }
    else if (flg & VAR_DOUBLE) {
        doubleData = strtod(str.c_str(),0);
    }
    else {
        data = str;
    }
}

CLVar *CLVar::getParameter(const std::string &name) {
    return findChildOrCreate(name)->var;
}

CLVarLink *CLVar::addChild(const std::string &name, CLVar *child) {
    if (isUndefined()) {
        flags = VAR_OBJECT;
    }
    if (!child) {
        child = new CLVar();
    }
    CLVarLink *link = new CLVarLink(child, name);
    link->owned = true;
    if (lastChild) {
        lastChild->nextSibling = link;
        link->prevSibling = lastChild;
        lastChild = link;
    } else {
        firstChild = link;
        lastChild = link;
    }
    return link;
}

CLVarLink *CLVar::addChildNoDup(const std::string &name, CLVar *child) {
    if (!child) {
        child = new CLVar();
    }

    CLVarLink *lns = findChild(name);
    if (lns) {
        lns->replaceWith(child);
    } else {
        lns = addChild(name, child);
    }
    return lns;
}

CLVarLink *CLVar::findChild(const string &name) {
    CLVarLink *lns = firstChild;
    while (lns) {
        if (lns->name.compare(name)==0) {
            return lns;
        }
        lns = lns->nextSibling;
    }
    return 0;
}

CLVarLink *CLVar::findChildOrCreate(const string &name) {
    CLVarLink *lns = findChild(name);
    if (lns) {
        return lns;
    }
    int flag = VAR_UNDEFINED;
    if (name[1]=='$') {
        flag = VAR_STRING;
    } else if (name[0]=='%') {
        flag = VAR_INTEGER;
    } else if (name[0]=='?') {
        // !TODO: ONS的数组是?开头的ID
    }
    return addChild(name, new CLVar(name, flag));
}

CLVarLink *CLVar::findChildOrCreate(const string &name, int varFlags) {
    CLVarLink *lns = findChild(name);
    if (lns) {
        return lns;
    } else {
        return addChild(name, new CLVar(varFlags==VAR_UNDEFINED ? name : ONS_DATA, varFlags));
    }
}

CLVarLink *CLVar::findChildOrCreateByPath(const std::string &path) {
    size_t p = path.find('.');
    if (p == string::npos) {
        return findChildOrCreate(path);
    }
    auto lns = findChildOrCreate(path.substr(0,p), VAR_OBJECT);
    return lns->var->findChildOrCreateByPath(path.substr(p+1));
}

void CLVar::removeChild(CLVar *child) {
    CLVarLink *link = firstChild;
    while (link) {
        if (link->var == child) {
            break;
        }
        link = link->nextSibling;
    }
    ASSERT(link);
    removeLink(link);
}

void CLVar::removeLink(CLVarLink *link) {
    if (!link) {
        return;
    }
    if (link->nextSibling) {
        link->nextSibling->prevSibling = link->prevSibling;
    }
    if (link->prevSibling) {
        link->prevSibling->nextSibling = link->nextSibling;
    }
    if (lastChild == link) {
        lastChild = link->prevSibling;
    }
    if (firstChild == link) {
        firstChild = link->nextSibling;
    }
    delete link;
}

void CLVar::removeAllChildren() {
    CLVarLink *c = firstChild;
    while (c) {
        CLVarLink *t = c->nextSibling;
        delete c;
        c = t;
    }
    firstChild = 0;
    lastChild = 0;
}

int CLVar::getInt() {
    if (isInt())    return (int)intData;
    if (isDouble()) return (int)doubleData;
    return 0;
}

void CLVar::setInt(int val) {
    flags = (flags&~VAR_MASK_VARTYPE) | VAR_INTEGER;
    intData = val;
    doubleData = 0;
    data = ONS_DATA;
}

double CLVar::getDouble() {
    if (isDouble()) return doubleData;
    if (isInt())    return intData;
    return 0;
}

void CLVar::setDouble(double val) {
    flags = (flags&~VAR_MASK_VARTYPE) | VAR_DOUBLE;
    doubleData = val;
    intData = 0;
    data = ONS_DATA;
}

const string& CLVar::getString() {
    /* Because we can't return a string that is generated on demand.
     * I should really just use char* :) */
    static string s_null = "null";
    static string s_undefined = "undefined";
    if (isInt()) {
        char buffer[32];
        sprintf_s(buffer, sizeof(buffer), "%ld", intData);
        data = buffer;
        return data;
    }
    if (isDouble()) {
        char buffer[32];
        sprintf_s(buffer, sizeof(buffer), "%f", doubleData);
        data = buffer;
        return data;
    }
    if (isNull())      return s_null;
    if (isUndefined()) return data.length() ? data : s_undefined;
    return data;
}

void CLVar::setString(const string &str) {
    // name sure it's not still a number or integer
    flags = (flags&~VAR_MASK_VARTYPE) | VAR_STRING;
    data = str;
    intData = 0;
    doubleData = 0;
}

CLVar *CLVar::getArrayIndex(int idx) {
    char sIdx[64];
    sprintf_s(sIdx, sizeof(sIdx), "%d", idx);
    CLVarLink *link = findChildOrCreate(sIdx, VAR_NULL);
    return link->var;
}

void CLVar::setArrayIndex(int idx, CLVar *val) {
    char sIdx[64];
    sprintf_s(sIdx, sizeof(sIdx), "%d", idx);
    CLVarLink *lns = findChildOrCreate(sIdx);
    lns->replaceWith(val);
}

void CLVar::setArray() {
    // name sure it's not still a number or integer
    flags = (flags&~VAR_MASK_VARTYPE) | VAR_ARRAY;
    data = ONS_DATA;
    intData = 0;
    doubleData = 0;
    removeAllChildren();
}

void CLVar::setUndefined() {
    // name sure it's not still a number or integer
    flags = (flags&~VAR_MASK_VARTYPE) | VAR_UNDEFINED;
    data = ONS_DATA;
    intData = 0;
    doubleData = 0;
    removeAllChildren();
}

void CLVar::setCallback(CLCallback callback, void *userdata) {
    this->callback = callback;
    this->userdata = userdata;
}

int CLVar::getChildren() {
    int n = 0;
    CLVarLink *l = firstChild;
    while (l) {
        n++;
        l = l->nextSibling;
    }
    return n;
}

int CLVar::getArrayLength() {
    int max = -1;
    if (!isArray()) {
        return 0;
    }

    CLVarLink *link = firstChild;
    while (link) {
        if (isNumber(link->name)) {
            int num = atoi(link->name.c_str());
            if (num > max) {
                max = num;
            }
        }
        link = link->nextSibling;
    }
    return max+1;
}

string CLVar::getFlagsAsString() {
    string ret = "";
    if (flags&VAR_UNDEFINED ) ret += "FUNCTION ";
    if (flags&VAR_FUNCTION  ) ret += "FUNCTION ";
    if (flags&VAR_OBJECT    ) ret += "OBJECT ";
    if (flags&VAR_ARRAY     ) ret += "ARRAY ";
    if (flags&VAR_DOUBLE    ) ret += "DOUBLE ";
    if (flags&VAR_INTEGER   ) ret += "INTEGER ";
    if (flags&VAR_STRING    ) ret += "STRING ";
    if (flags&VAR_NULL      ) ret += "NULL ";
    if (flags&VAR_LABEL     ) ret += "LABEL ";
    if (flags&VAR_NATIVE    ) ret += "NATIVE ";
    return ret;
}

string CLVar::getParsableString() {
    // Numbers can just be put in directly
    if (isNull())       return "null";
    if (isNumeric())    return getString();
    if (isString())     return getJSString(getString());
    if (isFunction()) {
        ostringstream funcStr;
        funcStr << "function (";
        // get list of parameters
        CLVarLink *link = firstChild;
        while (link) {
            funcStr << link->name;
            if (link->nextSibling) funcStr << ",";
            link = link->nextSibling;
        }
        // add function body
        funcStr << ") " << getString();
        return funcStr.str();
    }
    return "undefined";
}

bool CLVar::equals(CLVar *v) {
    CLVar *resV = mathsOp(v, LEX_EQUAL);
    bool res = resV->getBool();
    delete resV;
    return res;
}

void CLVar::copyValue(CLVar *xin) {
    if (xin) {
        copySimpleData(xin);
        removeAllChildren();
        CLVarLink *child = xin->firstChild;
        while (child) {
            CLVar *val = child->var->deepCopy();
            addChild(child->name, val);
            child = child->nextSibling;
        }
    } else {
      setUndefined();
    }
}

CLVar *CLVar::deepCopy() {
    CLVar* ret = new CLVar();
    ret->copySimpleData(this);
    CLVarLink *child = firstChild;
    while (child) {
        CLVar* val = child->var->deepCopy();
        ret->addChild(child->name, val);
        child = child->nextSibling;
    }
    return ret;
}

CLVar *CLVar::mathsOp(CLVar *b, int op) {
    CLVar *a = this;
    if (op == LEX_TYPEEQUAL || op == LEX_NTYPEEQUAL) {
        bool eql = ((a->flags & VAR_MASK_VARTYPE) ==
                    (b->flags & VAR_MASK_VARTYPE));
        if (eql) {
            CLVar *contents = a->mathsOp(b, LEX_EQUAL);
            if (!contents->getBool()) eql = false;
            if (!contents->refs) delete contents;
        }
        if (op == LEX_TYPEEQUAL) {
            return new CLVar(eql);
        } else {
            return new CLVar(!eql);
        }
    }
    // do maths...
    if (a->isUndefined() && b->isUndefined()) {
             if (op == LEX_EQUAL) return new CLVar(true);
        else if (op == LEX_NEQUAL) return new CLVar(false);
        else return new CLVar(); // undefined
    }
    else if ((a->isNumeric() || a->isUndefined()) &&
             (b->isNumeric() || b->isUndefined())) {
        if (!a->isDouble() && !b->isDouble()) {
            // use ints
            int da = a->getInt();
            int db = b->getInt();
            switch (op) {
                case '+'        : return new CLVar(da+db);
                case '-'        : return new CLVar(da-db);
                case '*'        : return new CLVar(da*db);
                case '/'        : return new CLVar(da/db);
                case '&'        : return new CLVar(da&db);
                case '|'        : return new CLVar(da|db);
                case '^'        : return new CLVar(da^db);
                case '%'        : return new CLVar(da%db);
                case '<'        : return new CLVar(da<db);
                case '>'        : return new CLVar(da>db);
                case LEX_EQUAL  : return new CLVar(da==db);
                case LEX_NEQUAL : return new CLVar(da!=db);
                case LEX_LEQUAL : return new CLVar(da<=db);
                case LEX_GEQUAL : return new CLVar(da>=db);
                default: throw new CLException("Operation "+CLLex::getTokenStr(op)+" not supported on the Int datatype");
            }
        } else {
            // use doubles
            double da = a->getDouble();
            double db = b->getDouble();
            switch (op) {
                case '+'        : return new CLVar(da+db);
                case '-'        : return new CLVar(da-db);
                case '*'        : return new CLVar(da*db);
                case '/'        : return new CLVar(da/db);
                case '<'        : return new CLVar(da<db);
                case '>'        : return new CLVar(da>db);
                case LEX_EQUAL  : return new CLVar(da==db);
                case LEX_NEQUAL : return new CLVar(da!=db);
                case LEX_LEQUAL : return new CLVar(da<=db);
                case LEX_GEQUAL : return new CLVar(da>=db);
                default: throw new CLException("Operation "+CLLex::getTokenStr(op)+" not supported on the Double datatype");
            }
        }
    } else if (a->isArray()) {
      /* Just check pointers */
      switch (op) {
           case LEX_EQUAL   : return new CLVar(a==b);
           case LEX_NEQUAL  : return new CLVar(a!=b);
           default: throw new CLException("Operation "+CLLex::getTokenStr(op)+" not supported on the Array datatype");
      }
    } else if (a->isObject()) {
          /* Just check pointers */
          switch (op) {
               case LEX_EQUAL   : return new CLVar(a==b);
               case LEX_NEQUAL  : return new CLVar(a!=b);
               default: throw new CLException("Operation "+CLLex::getTokenStr(op)+" not supported on the Object datatype");
          }
    } else {
       string da = a->getString();
       string db = b->getString();
       // use strings
       switch (op) {
           case '+'         : return new CLVar(da+db, VAR_STRING);
           case '<'         : return new CLVar(da<db);
           case '>'         : return new CLVar(da>db);
           case LEX_EQUAL   : return new CLVar(da==db);
           case LEX_NEQUAL  : return new CLVar(da!=db);
           case LEX_LEQUAL  : return new CLVar(da<=db);
           case LEX_GEQUAL  : return new CLVar(da>=db);
           default: throw new CLException("Operation "+CLLex::getTokenStr(op)+" not supported on the string datatype");
       }
    }
    ASSERT(0);
    return 0;
}

void CLVar::trace(string tag, const string &name) {
    TRACE("%s'%s' = '%s' %s\n", tag.c_str(), name.c_str(), getString().c_str(), getFlagsAsString().c_str());
    string indent = tag+" ";
    CLVarLink *link = firstChild;
    while (link) {
        link->var->trace(indent, link->name);
        link = link->nextSibling;
    }
}

void CLVar::toJSON(ostringstream& txt, const string tag) {
   if (isObject()) {
       string indentedLinePrefix = tag+"  ";
       txt << "{ \n";
       CLVarLink *link = firstChild;
       while (link) {
           txt << indentedLinePrefix;
           txt  << getJSString(link->name);
           txt  << " : ";
           link->var->toJSON(txt, indentedLinePrefix);
           link = link->nextSibling;
           if (link) {
               txt  << ",\n";
           }
       }
       txt << "\n" << tag << "}";
   } else if (isArray()) {
       string indent = tag+"  ";
       txt << "[\n";
       int len = getArrayLength();
       if (len>10000) len=10000; // we don't want to get stuck here!

       for (int i=0;i<len;i++) {
           getArrayIndex(i)->toJSON(txt, indent);
           if (i<len-1) txt  << ",\n";
       }

       txt << "\n" << tag << "]";
   } else {
       // no children or a function... just write value directly
       txt << getParsableString();
   }
}

CLVar *CLVar::ref() {
    refs++;
    return this;
}

void CLVar::unref() {
    if (refs<=0) {
        printf("OMFG, we have unreffed too far!\n");
    }
    if ((--refs)==0) {
        delete this;
    }
}

int CLVar::getRefs() {
    return refs;
}

void CLVar::init() {
    firstChild = 0;
    lastChild = 0;
    flags = 0;
    callback = 0;
    userdata = 0;
    data = ONS_DATA;
    intData = 0;
    doubleData = 0;
}

void CLVar::copySimpleData(CLVar *val) {
    data = val->data;
    intData = val->intData;
    doubleData = val->doubleData;
    flags = (flags & ~VAR_MASK_VARTYPE) | (val->flags & VAR_MASK_VARTYPE);
}

