//
//  CLNsa.cpp
//  CLite
//
//  Created by 韩琼 on 2019/12/8.
//  Copyright © 2019 韩琼. All rights reserved.
//

#include "CLNsa.hpp"

using namespace std;

CLNsa::~CLNsa() {
    if (fp) {
        fclose(fp);
    }
}

CLNsa::CLNsa(const uint8_t* kTable, const string& path) {
    this->fp = NULL;
    this->path = path;
    this->kTable = kTable;
}

void CLNsa::loadNSA() {
    fp = fopen(path.c_str(), "rb");
    long len = readShort();
    long stt = readLong();
    for (long i=0 ; i<len; i++) {
        uint8_t ch;
        string name;
        while((ch = kTable[fgetc(fp)])) {
            name+=ch;
        }
        transform(name.begin(), name.end(), name.begin(), ::tolower);
        NsaItem item;
        item.type = readChar();
        item.stt  = readLong() + stt;
        item.len  = readLong();
        item.size = readLong();
        if (item.type == NO_COMPRESSION) {
            item.type = getType(name);
        }
        if (item.type == NBZ_COMPRESSION ||  item.type == SPB_COMPRESSION) {
            item.size = 0;
        }
        files[name] = item;
    }
}
void CLNsa::loadNS2() {
    fp = fopen(path.c_str(), "rb");
    readChar();
    long stt = swapLong(readLong()) + 1;
    while(true) {
        uint8_t ch = kTable[fgetc(fp)];
        if (ch != '"') {
            break;
        }
        string name;
        while ((ch=kTable[fgetc(fp)]) != '"') {
            name += ch;
        }
        transform(name.begin(), name.end(), name.begin(), ::tolower);
        
        NsaItem item;
        item.stt = stt;
        item.len = swapLong(readLong());
        item.size = item.len;
        item.type = getType(name);
        files[name] = item;
        stt += item.len;
    }
}
void CLNsa::loadNS3() {
    fp = fopen(path.c_str(), "rb");
    readChar();
    readChar();
    long stt = swapLong(readLong()) + 2;
    while(true) {
        uint8_t ch = kTable[fgetc(fp)];
        if (ch != '"') {
            break;
        }
        string name;
        while ((ch=kTable[fgetc(fp)]) != '"') {
            name += ch;
        }
        transform(name.begin(), name.end(), name.begin(), ::tolower);
        NsaItem item;
        item.stt = stt;
        item.len = swapLong(readLong());
        item.size = item.len;
        item.type = getType(name);
        files[name] = item;
        stt += item.len;
    }
}
void CLNsa::savePath(const string& name) {
    for (auto one : files) {
        size_t idx = one.first.find('\\');
        string pathfile = name + "/" + one.first.substr(0, idx) + "-" + one.first.substr(idx+1);
        int   size = 0;
        void* data = 0;
        getFile(one.first, data, size);
        FILE* f = fopen(pathfile.c_str(), "w+");
        fwrite(data, 1, size, f);
        fclose(f);
        free(data);
    }
}
unsigned char CLNsa::readChar() {
    static unsigned char ret;
    fread(&ret, 1, 1, fp);
    return kTable[ret];
}

unsigned long CLNsa::readLong() {
    unsigned long ret;
    unsigned char buf[4];
    
    fread(&buf, 1, 4, fp);
    ret = kTable[buf[0]];
    ret = ret << 8 | kTable[buf[1]];
    ret = ret << 8 | kTable[buf[2]];
    ret = ret << 8 | kTable[buf[3]];
    return ret;
}

unsigned short CLNsa::readShort() {
    static unsigned short ret;
    static unsigned char  buf[2];
    fread(&buf, 1, 2, fp);
    ret = kTable[buf[0]] << 8 | kTable[buf[1]];
    return ret;
}

unsigned long CLNsa::swapLong(unsigned long ch) {
    return ((ch & 0xff000000) >> 24) | ((ch & 0x00ff0000) >> 8 ) |
           ((ch & 0x0000ff00) <<  8) | ((ch & 0x000000ff) << 24);
}

unsigned short CLNsa::swapShort(unsigned short ch) {
    return ((ch & 0xff00) >> 8) | ((ch & 0x00ff) << 8);
}

void CLNsa::getFile(string name, void* &data, int& size) {
    transform(name.begin(), name.end(), name.begin(), ::tolower);
    auto file = files.find(name);
    if (file == files.end()) {
        size = 0;
        data = 0;
        return;
    }
    auto info = file->second;
    fseek(fp, info.stt, SEEK_SET);
    size = (int)info.len;
    data = malloc(size);
    fread(data, 1, info.len, fp);
}

int CLNsa::getType(const std::string& name) {
    auto idx = name.find('.');
    string ext = (idx == string::npos) ? name : name.substr(idx+1);
    if (ext == "nbz") {
        return NBZ_COMPRESSION;
    } else if (ext == "spb") {
        return SPB_COMPRESSION;
    } else if (ext == "jpg") {
        return NO_COMPRESSION;
    } else if (ext == "jpg") {
        return NO_COMPRESSION;
    }
    return NO_COMPRESSION;
}
