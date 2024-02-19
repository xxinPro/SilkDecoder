//
// Created by xxin on 2024/2/16.
//

#include "CodeUtil.h"


/**
 * 判断指定的文件是否属于silk文件
 * @param file
 * @return 0：false；1：true
 */
int isSilk(FILE* file) {
    fseek(file, 0, SEEK_SET);
    // silk文件的文件头
    char silkHead[9] = {0x23, 0x21, 0x53, 0x49, 0x4C, 0x4B, 0x5F, 0x56, 0x33};
    // 判断文件是否属于silk文件
    char fileHeadBuffer[9] = {0};
    // 取文件头
    fread(fileHeadBuffer, sizeof(char), sizeof(fileHeadBuffer), file);
    for (int i = 0; i < 9; ++i) {
        if (silkHead[i] != fileHeadBuffer[i]) {
            return 0;
        }
    }
    return 1;
}

/**
 * 是否是来自微信的silk文件
 * @param file 文件指针
 * @return 0：false；1：true
 */
int isWeChatSilk(FILE* file) {
    fseek(file, 0, SEEK_SET);
    // 微信silk文件的文件头
    char wechatSilkHead[10] = {0x02, 0x23, 0x21, 0x53, 0x49, 0x4C, 0x4B, 0x5F, 0x56, 0x33};
    // 判断文件是否属于微信silk文件
    char fileHeadBuffer[10] = {0};
    // 取文件头
    fread(fileHeadBuffer, sizeof(char), sizeof(fileHeadBuffer), file);
    for (int i = 0; i < 10; ++i) {
        if (wechatSilkHead[i] != fileHeadBuffer[i]) {
            return 0;
        }
    }
    return 1;
}
