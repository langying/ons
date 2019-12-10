//
//  CLLex.hpp
//  CLite
//
//  Created by 韩琼 on 2019/11/27.
//  Copyright © 2019 韩琼. All rights reserved.
//

#ifndef CLLex_hpp
#define CLLex_hpp

#include "base.hpp"

class CLLex {
public:
    static std::string getTokenStr(int token); ///< Get the string representation of the given token
    
    char currCh, nextCh;
    int tk;             ///< The type of the token that we have
    int tokenStart;     ///< Position in the data at the beginning of the token we have here
    int tokenEnd;       ///< Position in the data at the last character of the token we have here
    int tokenLastEnd;   ///< Position in the data at the last character of the last token
    std::string tkStr;  ///< Data contained in the token we have here
    
public:
    ~CLLex(void);
    CLLex(const std::string& code);

    void reset(int pos);
    void match(int expected_tk); ///< Lexical match wotsit

    std::string getSubString(int pos); ///< Return a sub-string from the given position up until right now
    std::string getPosition(int pos=-1); ///< Return a string representing the position in lines and columns of the character pos given

protected:
    char *data;
    int dataPos, dataLen;

    void getNextCh();
    void getNextToken(); ///< Get the text token from our text string
};

#endif /* CLLex_hpp */
