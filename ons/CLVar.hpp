//
//  CLVar.hpp
//  CLite
//
//  Created by 韩琼 on 2019/11/28.
//  Copyright © 2019 韩琼. All rights reserved.
//

#ifndef CLVar_hpp
#define CLVar_hpp

#include "base.hpp"

class CLVarLink;

class CLVar {
public:
    ~CLVar(void);
    CLVar();
    CLVar(int val);
    CLVar(double val);
    CLVar(const std::string &str);
    CLVar(const std::string &data, int flags);
    
    CLVar *getParameter(const std::string &name); ///< If this is a function, get the parameter with the given name (for use by native functions)

    CLVarLink *addChild(const std::string &name, CLVar *child=NULL);
    CLVarLink *addChildNoDup(const std::string &name, CLVar *child=NULL); ///< add a child overwriting any with the same name
    
    CLVarLink *findChild(const std::string &name);
    CLVarLink *findChildOrCreate(const std::string &name);
    CLVarLink *findChildOrCreate(const std::string &name, int flags);
    CLVarLink *findChildOrCreateByPath(const std::string &path);
    
    void removeChild(CLVar *child);
    void removeLink(CLVarLink *link); ///< Remove a specific link (this is faster than finding via a child)
    void removeAllChildren();
    
    int                 getInt();
    void                setInt(int val);
    double              getDouble();
    void                setDouble(double val);
    const std::string&  getString();
    void                setString(const std::string &str);
    CLVar*              getArrayIndex(int idx);
    void                setArrayIndex(int idx, CLVar *value);
    
    void setArray();
    void setUndefined();
    void setCallback(CLCallback callback, void *userdata); ///< Set the callback for native functions

    int getChildren();                  ///< Get the number of children
    int getArrayLength();               ///< If this is an array, return the number of items in it (else 0)
    std::string getFlagsAsString();     ///< For debugging - just dump a string version of the flags
    std::string getParsableString();    ///< get Data as a parsable javascript string
    

    bool equals(CLVar *v);
    void copyValue(CLVar *val);         ///< copy the value from the value given
    CLVar *deepCopy();                  ///< deep copy this node and return the result
    CLVar *mathsOp(CLVar *b, int op);   ///< do a maths op with another script variable

    void trace(std::string tag = "", const std::string &name = "");
    void toJSON(std::ostringstream &txt, const std::string tag="");
    
    bool getBool()      { return getInt()   !=0; }
    bool isBasic()      { return firstChild ==0; }
    bool isInt()        { return (flags&VAR_INTEGER)        !=0; }
    bool isNull()       { return (flags&VAR_NULL)           !=0; }
    bool isArray()      { return (flags&VAR_ARRAY)          !=0; }
    bool isNative()     { return (flags&VAR_NATIVE)         !=0; }
    bool isDouble()     { return (flags&VAR_DOUBLE)         !=0; }
    bool isObject()     { return (flags&VAR_OBJECT)         !=0; }
    bool isString()     { return (flags&VAR_STRING)         !=0; }
    bool isNumeric()    { return (flags&VAR_MASK_NUMERIC)   !=0; }
    bool isFunction()   { return (flags&VAR_FUNCTION)       !=0; }
    bool isUndefined()  { return (flags&VAR_MASK_VARTYPE)   ==VAR_UNDEFINED; }
    
public:
    CLVar *ref();
    void unref();
    int getRefs();
    
public:
    CLVarLink *firstChild;
    CLVarLink *lastChild;

protected:
    int         refs;       ///< The number of references held to this - used for garbage collection
    int         flags;      ///< the flags determine the type of the variable - int/double/string/etc
    long        intData;    ///< The contents of this variable if it is an int
    double      doubleData; ///< The contents of this variable if it is a double
    std::string data;       ///< The contents of this variable if it is a string
    void*       userdata;   ///< user data passed as second argument to native functions
    CLCallback  callback;   ///< Callback for native functions
    

    void init(); ///< initialisation of data members
    void copySimpleData(CLVar *val);

    friend class CLOns;
};

#endif /* CLVar_hpp */
