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

typedef struct _NsaInfo {
    long stt;  // 索引位置
    long len;  // 实际文件长度
    long size; // 原始文件长度
    char type; // 文件压缩类型
} NsaItem;

class CLNsa {
public:
    ~CLNsa();
    CLNsa(const uint8_t* kTable, const std::string& path);
    
    void loadNSA(const std::string& charset);
    void loadNS2(const std::string& charset);
    void loadNS3(const std::string& charset);
    void savePath(const std::string& name);
    void getFile(std::string name, void** data, long* size);
    
    int  getType(const std::string& name);
    
public:
    const uint8_t* kTable;
    
    FILE *fp;
    std::string path;
    std::map<std::string, NsaItem> files;
    
    unsigned char  readChar();
    unsigned long  readLong();
    unsigned short readShort();
    
    unsigned long  swapLong(unsigned long ch);
    unsigned short swapShort(unsigned short ch);
};

#endif /* CLNsa_hpp */
