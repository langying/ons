//
//  CLLex.cpp
//  CLite
//
//  Created by 韩琼 on 2019/11/27.
//  Copyright © 2019 韩琼. All rights reserved.
//

#include "CLLex.hpp"

using namespace std;

string CLLex::getTokenStr(int token) {
    if (token>32 && token<128) {
        char buf[4] = "' '";
        buf[1] = (char)token;
        return buf;
    }
    switch (token) {
        case LEX_EOF            : return "EOF";
        case LEX_ID             : return "ID";
        case LEX_INT            : return "INT";
        case LEX_FLOAT          : return "FLOAT";
        case LEX_STR            : return "STRING";
        case LEX_EQUAL          : return "==";
        case LEX_TYPEEQUAL      : return "===";
        case LEX_NEQUAL         : return "!=";
        case LEX_NTYPEEQUAL     : return "!==";
        case LEX_LEQUAL         : return "<=";
        case LEX_LSHIFT         : return "<<";
        case LEX_LSHIFTEQUAL    : return "<<=";
        case LEX_GEQUAL         : return ">=";
        case LEX_RSHIFT         : return ">>";
        case LEX_RSHIFTUNSIGNED : return ">>";
        case LEX_RSHIFTEQUAL    : return ">>=";
        case LEX_PLUSEQUAL      : return "+=";
        case LEX_MINUSEQUAL     : return "-=";
        case LEX_PLUSPLUS       : return "++";
        case LEX_MINUSMINUS     : return "--";
        case LEX_ANDEQUAL       : return "&=";
        case LEX_ANDAND         : return "&&";
        case LEX_OREQUAL        : return "|=";
        case LEX_OROR           : return "||";
        case LEX_XOREQUAL       : return "^=";
                // reserved words
        case LEX_R_IF           : return "if";
        case LEX_R_FOR          : return "for";
        case LEX_R_BREAK        : return "break";
        case LEX_R_CONTINUE     : return "continue";
        case LEX_R_RETURN       : return "return";
    }

    ostringstream msg;
    msg << "?[" << token << "]";
    return msg.str();
}

CLLex::~CLLex(void) {
    free((void*)data);
}

CLLex::CLLex(const string& code) {
    data = _strdup(code.c_str());
    dataLen = (int)strlen(data);
    reset(0);
}

void CLLex::reset(int pos) {
    dataPos = pos;
    tokenStart = 0;
    tokenEnd = 0;
    tokenLastEnd = 0;
    tk = 0;
    tkStr = "";
    getNextCh();
    getNextCh();
    getNextToken();
}

void CLLex::match(int expected_tk) {
    if (tk && tk!=expected_tk) {
        ostringstream msg;
        msg << "Got " << getTokenStr(tk) << " expected " << getTokenStr(expected_tk) << " at " << getPosition(tokenStart);
        throw new CLException(msg.str());
    }
    getNextToken();
}

void CLLex::getNextCh() {
    currCh = nextCh;
    if (dataPos < dataLen) {
        nextCh = data[dataPos];
    } else {
        nextCh = 0;
    }
    dataPos++;
}

void CLLex::getNextToken() {
    tk = LEX_EOF;
    tkStr.clear();
    while (currCh && isWhitespace(currCh)) getNextCh();
    if (currCh==';') {
        while (currCh && currCh!='\n') getNextCh();
    }
    // record beginning of this token
    tokenStart = dataPos-2;
    if (currCh<0 || currCh=='~') {
        tk=LEX_TXT;
        do {
            if (currCh!='\r') {
                tkStr+=currCh;
            }
            getNextCh();
        } while (currCh && currCh != '\n');
    }
    else if (currCh=='*') {
        if (isNumeric(nextCh)) {
            tk=currCh;
            getNextCh();
        } else {
            tk = LEX_TAG;
            do {
                tkStr+=currCh; getNextCh();
            } while (isAlpha(currCh) || isNumeric(currCh));
            transform(tkStr.begin(), tkStr.end(), tkStr.begin(), ::tolower);
        }
    }
    else if (isAlpha(currCh)) { //  IDs
        while (isAlpha(currCh) || isNumeric(currCh)) {
            tkStr += currCh; getNextCh();
        }
        tk = LEX_ID;
        transform(tkStr.begin(), tkStr.end(), tkStr.begin(), ::tolower);
             if (tkStr=="if")       tk = LEX_R_IF;
        else if (tkStr=="for")      tk = LEX_R_FOR;
        else if (tkStr=="to")       tk = LEX_R_TO;
        else if (tkStr=="next")     tk = LEX_R_NEXT;
        else if (tkStr=="break")    tk = LEX_R_BREAK;
        else if (tkStr=="goto")     tk = LEX_R_GOTO;
        else if (tkStr=="gosub")    tk = LEX_R_GOSUB;
        else if (tkStr=="return")   tk = LEX_R_RETURN;
        else if (tkStr=="defsub")   tk = LEX_R_DEFSUB;
        else if (tkStr=="getparam") tk = LEX_R_GETPARAM;
        else if (tkStr=="numalias") tk = LEX_R_NUMALIAS;
        else if (tkStr=="continue") tk = LEX_R_CONTINUE;
    }
    else if (currCh=='#' || isNumeric(currCh)) { // Numbers
        bool isHex = false;
        if (currCh=='#') {
            isHex = true;
            tkStr += currCh; getNextCh();
        } else {
            if (currCh=='0') {
                tkStr += currCh; getNextCh();
            }
            if (currCh=='x') {
                isHex = true;
                tkStr += currCh; getNextCh();
            }
        }
        tk = LEX_INT;
        while (isNumeric(currCh) || (isHex && isHexadecimal(currCh))) {
            tkStr += currCh; getNextCh();
        }
        if (!isHex && currCh=='.') {
            tk = LEX_FLOAT;
            tkStr += '.';
            getNextCh();
            while (isNumeric(currCh)) {
                tkStr += currCh; getNextCh();
            }
        }
        // do fancy e-style floating point
        if (!isHex && (currCh=='e'||currCh=='E')) {
            tk = LEX_FLOAT;
            tkStr += currCh; getNextCh();
            if (currCh=='-') { tkStr += currCh; getNextCh(); }
            while (isNumeric(currCh)) {
                tkStr += currCh; getNextCh();
            }
        }
    }
    else if (currCh=='"') {
        getNextCh();
        while (currCh && currCh!='"') {
            if (currCh == '\\') {
                getNextCh();
                switch (currCh) {
                case 'n'  : tkStr += '\n'; break;
                case '"'  : tkStr += '"' ; break;
                case '\\' : tkStr += '\\'; break;
                default   : tkStr += '\\'; tkStr += currCh;
                }
            } else {
                tkStr += currCh;
            }
            getNextCh();
        }
        getNextCh();
        tk = LEX_STR;
    }
    else if (currCh=='\'') {
        getNextCh();
        while (currCh && currCh!='\'') {
            if (currCh == '\\') {
                getNextCh();
                switch (currCh) {
                case 'n'  : tkStr += '\n'; break;
                case '"'  : tkStr += '"' ; break;
                case '\\' : tkStr += '\\'; break;
                default   : tkStr += '\\'; tkStr += currCh;
                }
            } else {
                tkStr += currCh;
            }
            getNextCh();
        }
        getNextCh();
        tk = LEX_STR;
    }
    else {
        // single chars
        tk = currCh;
        if (currCh) getNextCh();
        if (tk=='=' && currCh=='=') { // ==
            tk = LEX_EQUAL;
            getNextCh();
            if (currCh=='=') { // ===
                tk = LEX_TYPEEQUAL;
                getNextCh();
            }
        } else if (tk=='<' && currCh=='>') {
            tk = LEX_NEQUAL;
            getNextCh();
        } else if (tk=='!' && currCh=='=') { // !=
            tk = LEX_NEQUAL;
            getNextCh();
            if (currCh=='=') { // !==
                tk = LEX_NTYPEEQUAL;
                getNextCh();
            }
        } else if (tk=='<' && currCh=='=') {
            tk = LEX_LEQUAL;
            getNextCh();
        } else if (tk=='<' && currCh=='<') {
            tk = LEX_LSHIFT;
            getNextCh();
            if (currCh=='=') { // <<=
                tk = LEX_LSHIFTEQUAL;
                getNextCh();
            }
        } else if (tk=='>' && currCh=='=') {
            tk = LEX_GEQUAL;
            getNextCh();
        } else if (tk=='>' && currCh=='>') {
            tk = LEX_RSHIFT;
            getNextCh();
            if (currCh=='=') { // >>=
                tk = LEX_RSHIFTEQUAL;
                getNextCh();
            } else if (currCh=='>') { // >>>
                tk = LEX_RSHIFTUNSIGNED;
                getNextCh();
            }
        } else if (tk=='+' && currCh=='=') {
            tk = LEX_PLUSEQUAL;
            getNextCh();
        } else if (tk=='-' && currCh=='=') {
            tk = LEX_MINUSEQUAL;
            getNextCh();
        } else if (tk=='+' && currCh=='+') {
            tk = LEX_PLUSPLUS;
            getNextCh();
        } else if (tk=='-' && currCh=='-') {
            tk = LEX_MINUSMINUS;
            getNextCh();
        } else if (tk=='&' && currCh=='=') {
            tk = LEX_ANDEQUAL;
            getNextCh();
        } else if (tk=='&' && currCh=='&') {
            tk = LEX_ANDAND;
            getNextCh();
        } else if (tk=='|' && currCh=='=') {
            tk = LEX_OREQUAL;
            getNextCh();
        } else if (tk=='|' && currCh=='|') {
            tk = LEX_OROR;
            getNextCh();
        } else if (tk=='^' && currCh=='=') {
            tk = LEX_XOREQUAL;
            getNextCh();
        }
        else if (tk=='!') { // 这行放在最后判断，兼容 if %F11!0
            tk= LEX_NEQUAL;
        }
    }
    /* This isn't quite right yet */
    tokenLastEnd = tokenEnd;
    tokenEnd = dataPos-3;
}

string CLLex::getSubString(int lastPosition) {
    int lastCharIdx = tokenLastEnd+1;
    if (lastCharIdx < dataLen) {
        /* save a memory alloc by using our data array to create the substring */
        char old = data[lastCharIdx];
        data[lastCharIdx] = 0;
        std::string value = &data[lastPosition];
        data[lastCharIdx] = old;
        return value;
    } else {
        return std::string(&data[lastPosition]);
    }
}

string CLLex::getPosition(int pos) {
    if (pos<0) {
        pos=tokenLastEnd;
    }
    int line = 1, col = 1;
    for (int i=0; i<pos; i++) {
        char ch;
        if (i < dataLen) {
            ch = data[i];
        } else {
            ch = 0;
        }
        col++;
        if (ch=='\n') {
            line++;
            col = 0;
        }
    }
    char buf[256];
    sprintf_s(buf, 256, "(line: %d, col: %d)", line, col);
    return buf;
}

