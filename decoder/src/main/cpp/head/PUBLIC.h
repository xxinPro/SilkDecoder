//
// Created by xxin on 2024/2/19.
//

#ifndef SILKDECODER_PUBLIC_H
#define SILKDECODER_PUBLIC_H

#include "../../../../../../../Sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/android/log.h"
#include "../../../../../../../Sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/malloc.h"
#include "../../../../../../../Sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/c++/v1/stdbool.h"

typedef const char* String;     // 字符串别名

#define DEBUG 1         // 宏定义，是否是DEBUG模式
#define LOG_TAG "silk"  // log的TAG

// DEBUG模式下输出日志
#if DEBUG
#define LOG_I(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOG_D(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOG_PRINTF(...) printf(__VA_ARGS__)
#else
#define LOGI(...)
    #define LOGD(...)
    #define LOGE(...)
    #define LOG_PRINTF(...)
#endif

#endif //SILKDECODER_PUBLIC_H
