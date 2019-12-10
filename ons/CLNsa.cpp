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
    if (kTable) {
        free((void*)kTable);
    }
}

CLNsa::CLNsa(const string& path) {
    this->fp = NULL;
    this->path = path;
    this->kTable = NULL;
    codec["GIF"] = NO_COMPRESSION;
    codec["JPG"] = NO_COMPRESSION;
    codec["NBZ"] = NBZ_COMPRESSION;
    codec["SPB"] = SPB_COMPRESSION;
}


void CLNsa::load(ONS_ARC offset) {
    fp = fopen(path.c_str(), "rb");
    for (int i=0; i<offset; i++) {
        readChar();
    }
    if (offset == ONS_NS2) {
        long sttIdx = swapLong(readLong()) + offset;
        while(true) {
            unsigned char ch = kTable[fgetc(fp)];
            if (ch != '"') {
                break;
            }
            std::string name = "";
            while ((ch=kTable[fgetc(fp)]) != '"') {
                if ('a' <= ch && ch <= 'z') {
                    ch += 'A' - 'a';
                }
                name+=ch;
            }
            long length = swapLong(readLong());
            files[name] = FileInfo{sttIdx, length};
            sttIdx += length;
        }
    } else {
        long count  = readShort();
        long sttIdx = readLong() + offset;
        for (long i=0 ; i<count; i++) {
            unsigned char ch;
            std::string name;
            while((ch = kTable[fgetc(fp)])) {
                if ('a' <= ch && ch <= 'z') {
                    ch += 'A' - 'a';
                }
                name+=ch;
            }

            FileInfo info;
            if (offset == ONS_NSA) {
                info.type = readChar();
                info.idx= readLong() + sttIdx;
                info.len =readLong();
                info.size = readLong();
            } else {
                info.type = NO_COMPRESSION;
                info.idx= readLong() + sttIdx;
                info.len = readLong();
                info.size = info.len;
            }
         
            if (info.type == NO_COMPRESSION) {
                info.type = 0;
            }
            if (info.type == NBZ_COMPRESSION ||  info.type == SPB_COMPRESSION ){
                info.size = 0;
            }
            files[name] = info;
        }
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

void CLNsa::makeKeyTable(const char* pathfile) {
    if (!pathfile) {
        return;
    }
    FILE* fp = fopen(pathfile, "rb");
    if (!fp){
        fprintf(stderr, "createKeyTable: can't open EXE file %s\n", pathfile);
        return;
    }
    
    kTable = new unsigned char[256];
    for (int i=0; i<256; i++) {
        kTable[i] = i;
    }
    
    unsigned char ring_buffer[256];
    
    int ring_start = 0;
    int ring_last = 0;
    int i, ch, count;
    while((ch = fgetc(fp)) != EOF){
        i = ring_start;
        count = 0;
        while (i != ring_last && ring_buffer[i] != ch ){
            count++;
            i = (i+1)%256;
        }
        if (i == ring_last && count == 255) {
            break;
        }
        if (i != ring_last) {
            ring_start = (i+1)%256;
        }
        ring_buffer[ring_last] = ch;
        ring_last = (ring_last+1)%256;
    }
    fclose(fp);

    if (ch == EOF) {
        printf("createKeyTable: can't find a key table.");
        exit(-1);
    }

    ring_buffer[ring_last] = ch;
    for (i=0 ; i<256 ; i++) {
        kTable[ring_buffer[(ring_start+i)%256]] = i;
    }
}
