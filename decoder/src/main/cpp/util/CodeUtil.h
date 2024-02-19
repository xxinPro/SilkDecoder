//
// Created by xxin on 2024/2/16.
//

#ifndef SILKDECODER_CODEUTIL_H
#define SILKDECODER_CODEUTIL_H

#include "PUBLIC.h"

/**
 * 判断指定的文件是否属于silk文件
 * @param file
 * @return 0：false；1：true
 */
int isSilk(FILE* file);

/**
 * 是否是来自微信的silk文件
 * @param file 文件指针
 * @return 0：false；1：true
 */
int isWeChatSilk(FILE* file);

#endif //SILKDECODER_CODEUTIL_H
