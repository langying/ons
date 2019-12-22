//
//  base.cpp
//  CLite
//
//  Created by 韩琼 on 2019/11/27.
//  Copyright © 2019 韩琼. All rights reserved.
//

#include <iconv.h>
#include "base.hpp"

using namespace std;

bool isAlpha(char ch) {
    return ((ch>='a') && (ch<='z')) || ((ch>='A') && (ch<='Z')) || ch=='_' || ch=='%' || ch=='$';
}

bool isNumeric(char ch) {
    return (ch>='0') && (ch<='9');
}

bool isIDString(char *s) {
    if (!isAlpha(*s)) {
        return false;
    }
    while (*s) {
        if (!(isAlpha(*s) || isNumeric(*s))) {
            return false;
        }
        s++;
    }
    return true;
}

bool isWhitespace(char ch) {
    return ch==' ' || ch=='\t' || ch=='\r';
}

bool isHexadecimal(char ch) {
    return ((ch>='0') && (ch<='9')) ||
           ((ch>='a') && (ch<='f')) ||
           ((ch>='A') && (ch<='F'));
}

bool isNumber(const string &str) {
    for (size_t i=0;i<str.size();i++) {
        if (!isNumeric(str[i])) {
            return false;
        }
    }
    return true;
}

bool isAlphaNum(const std::string &str) {
    if (str.size()==0)      return true;
    if (!isAlpha(str[0]))   return false;
    for (size_t i=0;i<str.size();i++) {
        if (!(isAlpha(str[i]) || isNumeric(str[i]))) {
            return false;
        }
    }
    return true;
}


bool endWith(const std::string& str, const std::string& ext) {
    return str.rfind(ext) == (str.length() - ext.length());
}

void replace(string &str, char textFrom, const char *textTo) {
    int sLen = (int)strlen(textTo);
    size_t p = str.find(textFrom);
    while (p != string::npos) {
        str = str.substr(0, p) + textTo + str.substr(p+1);
        p = str.find(textFrom, p+sLen);
    }
}

std::string getJSString(const std::string &str) {
    std::string nStr = str;
    for (size_t i=0;i<nStr.size();i++) {
        const char *replaceWith = "";
        bool replace = true;

        switch (nStr[i]) {
            case '\\': replaceWith = "\\\\"; break;
            case '\n': replaceWith = "\\n" ; break;
            case '\r': replaceWith = "\\r" ; break;
            case '\a': replaceWith = "\\a" ; break;
            case '"' : replaceWith = "\\\""; break;
            default: {
                int nCh = ((int)nStr[i]) &0xFF;
                if (nCh<32 || nCh>127) {
                    char buffer[5];
                    sprintf_s(buffer, 5, "\\x%02X", nCh);
                    replaceWith = buffer;
                } else {
                    replace=false;
                }
            }
      }

      if (replace) {
        nStr = nStr.substr(0, i) + replaceWith + nStr.substr(i+1);
        i += strlen(replaceWith)-1;
      }
    }
    return "\"" + nStr + "\"";
}
std::string codecString(const std::string& src, const std::string& srcCharset, const std::string& dstCharset) {
    if (srcCharset == dstCharset) {
        return src;
    }
    iconv_t cd = iconv_open(dstCharset.c_str(), srcCharset.c_str());
    if (cd) {
        size_t srcLen = src.length();
        char*  srcStr = (char*)src.c_str();
        
        size_t dstLen = src.length() * 3;
        char*  dstStr = (char*)calloc(dstLen, 1);
        char*  outStr = dstStr;
        
        std::string dst;
        if (iconv(cd, &srcStr, &srcLen, &outStr, &dstLen) != -1) {
            dst = dstStr;
        } else {
            dst = src;
        }
        free(dstStr);
        iconv_close(cd);
        return dst;
    } else {
        return src;
    }
}
