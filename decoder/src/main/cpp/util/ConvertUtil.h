//
// Created by xxin on 2024/2/19.
//

#ifndef SILKDECODER_CONVERTUTIL_H
#define SILKDECODER_CONVERTUTIL_H

#include "PUBLIC.h"
#include "CodeUtil.h"

#include "lame.h"
#include "SKP_Silk_SDK_API.h"

#define MAX_BYTES_PER_FRAME     1024
#define MAX_INPUT_FRAMES        5
#define FRAME_LENGTH_MS         20
#define MAX_API_FS_KHZ          48

/**
 * 将silk文件转换成pcm文件
 * @param silkFilePath  silk文件路径
 * @param pcmFilePath   pcm文件路径
 * @param sampleRate    采样率
 * @return 0:转换失败；1:转换成功
 */
extern int silk2pcm(String silkFilePath, String pcmFilePath, int sampleRate);

/**
 * 将pcm文件转换成mp3文件
 * @param pcmFilePath   pcm文件路径
 * @param mp3FilePath   mp3文件路径
 * @param sampleRate    采样率
 * @return 0:转换失败；1:转换成功
 */
extern int pcm2mp3(String pcmFilePath, String mp3FilePath, int sampleRate);

#endif //SILKDECODER_CONVERTUTIL_H
