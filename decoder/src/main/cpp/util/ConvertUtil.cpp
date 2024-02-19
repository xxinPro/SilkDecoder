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
 * 将pcm文件转换成mp3文件
 * @param pcmFilePath   pcm文件路径
 * @param mp3FilePath   mp3文件路径
 * @param sampleRate    采样率
 * @return 0:转换失败；1:转换成功
 */
int pcm2mp3(String pcmFilePath, String mp3FilePath, int sampleRate) {
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
    // 设置声道，跟实例化AudioRecord的参数的channelConfig声道配置保持一致
    lame_set_num_channels(lame, 1);
    // 设置采样率
    lame_set_in_samplerate(lame, sampleRate);
    // 设置比特率xxx kbps
    lame_set_brate(lame, 128);
    // 设置模式。立体声或者单声道
    lame_set_mode(lame, MONO);
    // 设置质量影响压缩算法，值范围为0-9, 0质量最好速度最慢，9质量最差速度最快。
    // 源码建议：3:near-best quality,not too slow; 5:good quality,fast; 7:ok quality,really fast
    lame_set_quality(lame, 5);
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
