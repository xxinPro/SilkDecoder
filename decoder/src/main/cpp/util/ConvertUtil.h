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

// WAV文件头结构
typedef struct WavHeader {
    char            chunkId[4];     // "RIFF"标识
    unsigned int    chunkSize;      // 文件总大小（包括文件头）
    char            format[4];      // "WAVE"标识
    char            subchunk1Id[4]; // "fmt "标识
    unsigned int    subchunk1Size;  // fmt块的大小
    unsigned short  audioFormat;    // 音频格式（PCM为1）
    unsigned short  numChannels;    // 设置声道数，单声道(Mono)为1，双声道(Stereo)为2
    unsigned int    sampleRate;     // 采样率
    unsigned int    byteRate;       // 每秒字节数
    unsigned short  blockAlign;     // 每个样本的字节数
    unsigned short  bitsPerSample;  // 每个样本的位数
    char            subchunk2Id[4]; // "data"标识
    unsigned int    subchunk2Size;  // 数据块的大小
} WavHeader;


/**
 * 将silk文件转换成pcm文件
 * @param silkFilePath  silk文件路径
 * @param pcmFilePath   pcm文件路径
 * @param sampleRate    采样率
 * @return 0:转换失败；1:转换成功
 */
extern int silk2pcm(String silkFilePath, String pcmFilePath, int sampleRate);

/**
 * 将pcm文件转换成wav文件
 * @param pcmFilePath   pcm文件路径
 * @param wavFilePath   wav文件路径
 * @param sampleRate    采样率
 * @return 0:转换失败；1:转换成功
 */
extern int pcm2wav(String pcmFilePath, String wavFilePath, int sampleRate);

/**
 * 将pcm文件转换成mp3文件
 * @param pcmFilePath   pcm文件路径
 * @param mp3FilePath   mp3文件路径
 * @param sampleRate    采样率
 * @param bitrate       比特率
 * @return 0:转换失败；1:转换成功
 */
extern int pcm2mp3(String pcmFilePath, String mp3FilePath, int sampleRate, int bitrate);

#endif //SILKDECODER_CONVERTUTIL_H
