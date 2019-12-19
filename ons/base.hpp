//
//  base.hpp
//  CLite
//
//  Created by 韩琼 on 2019/11/27.
//  Copyright © 2019 韩琼. All rights reserved.
//

#ifndef base_hpp
#define base_hpp

#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <map>
#include <chrono>
#include <future>
#include <string>
#include <thread>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>

#define ONS_ARGC    "argc"
#define ONS_DATA    ""
#define ONS_NAME    ""

// If defined, this keeps a note of all calls and where from in memory. This is slower, but good for debugging
#define TINYJS_CALL_STACK

#define ASSERT(X) assert(X)

/* Frees the given link IF it isn't owned by anything else */
#define CLEAN(x) { CLVarLink *__v = x; if (__v && !__v->owned) { delete __v; } }

/* Create a LINK to point to VAR and free the old link.
 * BUT this is more clever - it tries to keep the old link if it's not owned to save allocations */
#define CREATE_LINK(LINK, VAR) { if (!LINK || LINK->owned) LINK = new CLVarLink(VAR); else LINK->replaceWith(VAR); }

#ifdef _WIN32
    #ifdef _DEBUG
        #define _CRTDBG_MAP_ALLOC
        #include <stdlib.h>
        #include <crtdbg.h>
    #endif
#endif

#ifndef TRACE
    #define TRACE printf
#endif


#ifdef _WIN32
    #ifdef _DEBUG
       #ifndef DBG_NEW
          #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
          #define new DBG_NEW
       #endif
    #endif
#endif

#ifdef __GNUC__
    #define _strdup     strdup
    #define sprintf_s   snprintf
    #define vsprintf_s  vsnprintf
#endif

#define DEBUG_MEMORY 0

#if DEBUG_MEMORY

vector<CLVar*>      allocatedVars;
vector<CLVarLink*>  allocatedLinks;

void mark_allocated(CLVar *v) {
    allocatedVars.push_back(v);
}

void mark_deallocated(CLVar *v) {
    for (size_t i=0;i<allocatedVars.size();i++) {
        if (allocatedVars[i] == v) {
            allocatedVars.erase(allocatedVars.begin()+i);
            break;
        }
    }
}

void mark_allocated(CLVarLink *v) {
    allocatedLinks.push_back(v);
}

void mark_deallocated(CLVarLink *v) {
    for (size_t i=0;i<allocatedLinks.size();i++) {
        if (allocatedLinks[i] == v) {
            allocatedLinks.erase(allocatedLinks.begin()+i);
            break;
        }
    }
}

void show_allocated() {
    for (size_t i=0;i<allocatedVars.size();i++) {
        printf("ALLOCATED, %d refs\n", allocatedVars[i]->getRefs());
        allocatedVars[i]->trace("  ");
    }
    for (size_t i=0;i<allocatedLinks.size();i++) {
        printf("ALLOCATED LINK %s, allocated[%d] to \n", allocatedLinks[i]->name.c_str(), allocatedLinks[i]->var->getRefs());
        allocatedLinks[i]->var->trace("  ");
    }
    allocatedVars.clear();
    allocatedLinks.clear();
}
#endif

enum LEX_TYPES {
    LEX_EOF = 0,
    LEX_ID = 256,
    LEX_INT,
    LEX_STR,
    LEX_TAG,
    LEX_TXT,
    LEX_FLOAT,

    LEX_EQUAL,
    LEX_TYPEEQUAL,
    LEX_NEQUAL,
    LEX_NTYPEEQUAL,
    LEX_LEQUAL,
    LEX_LSHIFT,
    LEX_LSHIFTEQUAL,
    LEX_GEQUAL,
    LEX_RSHIFT,
    LEX_RSHIFTUNSIGNED,
    LEX_RSHIFTEQUAL,
    LEX_PLUSEQUAL,
    LEX_MINUSEQUAL,
    LEX_PLUSPLUS,
    LEX_MINUSMINUS,
    LEX_ANDEQUAL,
    LEX_ANDAND,
    LEX_OREQUAL,
    LEX_OROR,
    LEX_XOREQUAL,
    // reserved words
#define LEX_R_LIST_START LEX_R_IF
    LEX_R_IF,
    LEX_R_TO,
    LEX_R_FOR,
    LEX_R_NEXT,
    LEX_R_BREAK,
    LEX_R_GOTO,
    LEX_R_GOSUB,
    LEX_R_RETURN,
    LEX_R_DEFSUB,
    LEX_R_GETPARAM,
    LEX_R_NUMALIAS,
    LEX_R_CONTINUE,

    LEX_R_LIST_END /* always the last entry */
};

enum VAR_FLAGS {
    VAR_UNDEFINED   = 0,
    VAR_FUNCTION    = 1,
    VAR_OBJECT      = 2,
    VAR_ARRAY       = 4,
    VAR_DOUBLE      = 8,   // floating point double
    VAR_INTEGER     = 16,  // integer number
    VAR_STRING      = 32,  // string
    VAR_NULL        = 64,  // it seems null is its own data type
    VAR_LABEL       = 128, // like c: goto label;
    VAR_NATIVE      = 256, // to specify this is a native function
    VAR_MASK_NUMERIC =  VAR_NULL    |
                        VAR_DOUBLE  |
                        VAR_INTEGER,
    VAR_MASK_VARTYPE =  VAR_NULL    |
                        VAR_DOUBLE  |
                        VAR_INTEGER |
                        VAR_ARRAY   |
                        VAR_LABEL   |
                        VAR_OBJECT  |
                        VAR_STRING  |
                        VAR_FUNCTION,
};

bool isAlpha(char ch);
bool isNumeric(char ch);
bool isIDString(char *s);
bool isWhitespace(char ch);
bool isHexadecimal(char ch);
bool isNumber(const std::string &str);
bool isAlphaNum(const std::string &str);

bool endWith(const std::string& str, const std::string& ext);
void replace(std::string &str, char textFrom, const char *textTo);
std::string getJSString(const std::string &str);

class CLVar;
class CLException {
public:
    std::string msg;
    CLException(const std::string &msg) { this->msg = msg; }
};

typedef struct _CLData {
    long  len;
    void* ptr;
} CLData;
typedef struct _CLPoint {
    int x, y;
} CLPoint;
typedef struct _CLSize {
    int width, height;
} CLSize;

typedef void (*CLCallback)(CLVar* func, void* data);


#endif /* base_hpp */
