//
//  CLNsa.hpp
//  CLite
//
//  Created by 韩琼 on 2019/12/8.
//  Copyright © 2019 韩琼. All rights reserved.
//

#ifndef CLNsa_hpp
#define CLNsa_hpp

#include "base.hpp"

enum ONS_ARC {
    ONS_NSA = 0, // xxx.nsa or ***.___
    ONS_NS2 = 1, // xxx.ns2
    ONS_NS3 = 2,
};

enum ONS_CODEC {
    NO_COMPRESSION   = 0,
    SPB_COMPRESSION  = 1,
    LZSS_COMPRESSION = 2,
    NBZ_COMPRESSION  = 4
};

typedef struct _FileInfo {
    long idx;  // 索引位置
    long len;  // 实际文件长度
    long size; // 原始文件长度
    char type; // 文件压缩类型
} FileInfo;

class CLNsa {
public:
    ~CLNsa();
    CLNsa(const std::string& path);
    
    void load(ONS_ARC offset);
    void makeKeyTable(const char* pathfile);
    
private:
    unsigned char* kTable;
    
    FILE *fp;
    std::string path;
    std::map<std::string, int> codec;
    std::map<std::string, FileInfo> files;
    
    unsigned char  readChar();
    unsigned long  readLong();
    unsigned short readShort();
    
    unsigned long  swapLong(unsigned long ch);
    unsigned short swapShort(unsigned short ch);
};

#endif /* CLNsa_hpp */
