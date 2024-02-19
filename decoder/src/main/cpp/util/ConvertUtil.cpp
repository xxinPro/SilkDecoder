//
// Created by xxin on 2024/2/19.
//

#include "ConvertUtil.h"

/**
 * 将silk文件转换成pcm文件
 * @param silkFilePath  silk文件路径
 * @param pcmFilePath   pcm文件路径
 * @param sampleRate    采样率
 * @return 0:转换失败；1:转换成功
 */
int silk2pcm(String silkFilePath, String pcmFilePath, int sampleRate) {
    SKP_int32 result;           // silk方法返回值
    SKP_int32 decodeSizeBytes;  // 创建解码器需要的内存大小
    SKP_int32 counter;          // 从待解码文件中读取的单元个数
    SKP_int32 totalLen;         // 解码后文件的长度
    SKP_int32 frames;           // 不知道
    SKP_int16 validDataLen;     // 分块却待解码文件中有效数据的长度
    SKP_int16 outDataLen;       // 解码数据的长度

    void *decodeState;                              // 动态申请的解码器对象的内存地址
    SKP_SILK_SDK_DecControlStruct decodeControl;    // 解码器控制参数

    // 待解码文件中有效数据的缓冲区，缓冲区指针
    SKP_uint8 validDataBuffer[MAX_BYTES_PER_FRAME * MAX_INPUT_FRAMES], *validDataBufferPointer = NULL;
    // 解码后文件的数据缓冲区与缓冲区指针
    SKP_int16 outBuffer[((FRAME_LENGTH_MS * MAX_API_FS_KHZ) << 1) * MAX_INPUT_FRAMES], *outBufferPointer = NULL;

    // 打开输入文件
    FILE *silkFile = fopen(silkFilePath, "rb");
    if (silkFile == NULL) {
        LOG_E("Error: could not open file %s", silkFilePath);
        return 0;
    }

    // 验证文件头，文件头的字符串无法被解码器识别
    if (isSilk(silkFile)) {
        LOG_D("file belong silk");
    } else if (isWeChatSilk(silkFile)) {
        LOG_D("file belong wechat silk");
    } else {
        LOG_E("file not belong silk");
        return 0;
    }

    // 设置采样率
    if (sampleRate == 0) {
        sampleRate = 24000;
    }
    decodeControl.API_sampleRate = sampleRate;

    // 获取创建解码器需要的内存大小
    result = SKP_Silk_SDK_Get_Decoder_Size(&decodeSizeBytes);
    if (result) {
        LOG_E("Error: SKP_Silk_SDK_Get_Decoder_Size, code: %d", result);
    }
    // 动态申请解码器对象的内存
    decodeState = malloc(decodeSizeBytes);

    // 初始化解码器对象的内存地址
    result = SKP_Silk_SDK_InitDecoder(decodeState);
    if (result) {
        LOG_E("Error: SKP_Silk_SDK_InitDecoder, code: %d", result);
    }

    // 打开输出文件
    FILE *pcmFile = fopen(pcmFilePath, "wb");
    if (pcmFile == NULL) {
        LOG_E("Error: could not open file %s", pcmFilePath);
        return 0;
    }

    while (1) {
        // 读取有效数据大小
        counter = fread(&validDataLen, sizeof(SKP_int16), 1, silkFile);
        if (validDataLen < 0 || counter < 1) {
            break;
        }
        // 读取有效数据
        counter = fread(validDataBuffer, sizeof(unsigned char), validDataLen, silkFile);
        if ((SKP_int16) counter < validDataLen) {
            break;
        }

        validDataBufferPointer = validDataBuffer;
        outBufferPointer = outBuffer;
        totalLen = 0;
        frames = 0;

        do {
            result = SKP_Silk_SDK_Decode(decodeState, &decodeControl, 0,
                                         validDataBufferPointer, validDataLen, outBufferPointer, &outDataLen);
            if (result) {
                LOG_E("Error: SKP_Silk_SDK_Decode, code: %d", result);
                break;
            }
            frames++;
            outBufferPointer += outDataLen;
            totalLen += outDataLen;
            if (frames > MAX_INPUT_FRAMES) {
                outBufferPointer = outBuffer;
                totalLen = 0;
                frames = 0;
            }
        } while (decodeControl.moreInternalDecoderFrames);

        fwrite(outBuffer, sizeof(SKP_int16), totalLen, pcmFile);
    }

    // 释放解码器
    free(decodeState);

    // 关闭文件
    fclose(pcmFile);
    fclose(silkFile);

    // 解码完成
    LOG_D("silk decode over");

    return 1;
}


/**
 * 将pcm文件转换成wav文件
 * @param pcmFilePath   pcm文件路径
 * @param wavFilePath   wav文件路径
 * @param sampleRate    采样率
 * @return 0:转换失败；1:转换成功
 */
int pcm2wav(String pcmFilePath, String wavFilePath, int sampleRate) {
    FILE *pcmFile = fopen(pcmFilePath, "rb");
    if (pcmFile == NULL) {
        LOG_E("Error: could not open file %s", pcmFilePath);
        return 0;
    }

    fseek(pcmFile, 0, SEEK_END);
    long pcmFileSize = ftell(pcmFile);
    rewind(pcmFile);

    FILE *wavFile = fopen(wavFilePath, "wb");
    if (wavFile == NULL) {
        LOG_E("Error: could not open file %s", pcmFilePath);
        return 0;
    }

    WavHeader wavHeader;
    strncpy(wavHeader.chunkId, "RIFF", 4);
    wavHeader.chunkSize = 36 + pcmFileSize;
    strncpy(wavHeader.format, "WAVE", 4);
    strncpy(wavHeader.subchunk1Id, "fmt ", 4);
    wavHeader.subchunk1Size = 16;
    wavHeader.audioFormat = 1;  // PCM编码
    wavHeader.numChannels = 1;
    wavHeader.sampleRate = sampleRate;
    wavHeader.bitsPerSample = 16;
    wavHeader.byteRate = wavHeader.sampleRate * wavHeader.numChannels * wavHeader.bitsPerSample / 8;
    wavHeader.blockAlign = wavHeader.numChannels * wavHeader.bitsPerSample / 8;
    strncpy(wavHeader.subchunk2Id, "data", 4);
    wavHeader.subchunk2Size = pcmFileSize - 36;

    fwrite(&wavHeader, sizeof(struct WavHeader), 1, wavFile);

    // 写入PCM数据
    void *pcmData = malloc(pcmFileSize);
    fread(pcmData, pcmFileSize, 1, pcmFile);
    fwrite(pcmData, pcmFileSize, 1, wavFile);

    free(pcmData);
    fclose(pcmFile);
    fclose(wavFile);

    return 1;
}


/**
 * 将pcm文件转换成mp3文件
 * @param pcmFilePath   pcm文件路径
 * @param mp3FilePath   mp3文件路径
 * @param sampleRate    采样率
 * @param bitrate       比特率
 * @return 0:转换失败；1:转换成功
 */
int pcm2mp3(String pcmFilePath, String mp3FilePath, int sampleRate, int bitrate) {
    int readLen, writeLen;
    int PCM_SIZE = 8192;
    int MP3_SIZE = 8192;
    short int pcmBuffer[PCM_SIZE];
    unsigned char mp3Buffer[MP3_SIZE];

    // 打开文件
    FILE *pcmFile = fopen(pcmFilePath, "rb");
    if (pcmFile == NULL) {
        LOG_E("Error: could not open file %s", pcmFilePath);
        return 0;
    }

    FILE *mp3File = fopen(mp3FilePath, "wb");
    if (mp3File == NULL) {
        LOG_E("Error: could not open file %s", mp3FilePath);
        return 0;
    }

    lame_t lame = lame_init();
    // 设置输入音频的采样率
    lame_set_in_samplerate(lame, sampleRate);
    // 设置输出 MP3 文件的采样率
    lame_set_out_samplerate(lame, sampleRate);
    // 设置比特率，以千比特每秒（kbps）为单位
    lame_set_brate(lame, bitrate);
    // 设置声道数
    lame_set_num_channels(lame, 1);
    // 设置音频编码模式，此处为单声道模式
    lame_set_mode(lame, MONO);
    // 设置编码质量，有效值范围0.0到9.0, 取值越高音质越好，但是输出的mp3文件更大，编码速度也会下降
    // 建议取值：3:质量很好，速度还可以; 5:质量较好，速度快; 7:质量还可以，速度很快
    lame_set_quality(lame, 3);
    // 初始化参数
    lame_init_params(lame);

    do {
        readLen = fread(pcmBuffer, sizeof(short int), PCM_SIZE, pcmFile);
        if (readLen == 0) {
            writeLen = lame_encode_flush(lame, mp3Buffer, MP3_SIZE);
        } else {
            writeLen = lame_encode_buffer(lame, pcmBuffer, nullptr, readLen, mp3Buffer, MP3_SIZE);
        }
        fwrite(mp3Buffer, 1, writeLen, mp3File);
    } while (readLen != 0);

    // 释放lame
    lame_close(lame);

    // 转换完成
    LOG_D("mp3 convert over");

    return 1;
}
