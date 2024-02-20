
#include "decoder.h"

extern "C" {
JNIEXPORT jboolean JNICALL
Java_xyz_xxin_silkdecoder_DecodeNative_silk2mp3(JNIEnv *env, jclass clazz,
                                                jstring _silkFilePath, jstring _pcmFilePath, jstring _mp3FilePath,
                                                jint _sampleRate, jint _bitrate) {

    String silkFilePath = env->GetStringUTFChars(_silkFilePath, 0);
    String pcmFilePath = env->GetStringUTFChars(_pcmFilePath, 0);
    String mp3FilePath = env->GetStringUTFChars(_mp3FilePath, 0);
    int sampleRate = (int)_sampleRate, bitrate = (int)_bitrate; // 采样率、比特率

    int result = 0;

    // silk转pcm
    if (silk2pcm(silkFilePath, pcmFilePath, sampleRate)) {
        // pcm转mp3
        result = pcm2mp3(pcmFilePath, mp3FilePath, sampleRate, bitrate);
    }

    env->ReleaseStringUTFChars(_silkFilePath, silkFilePath);
    env->ReleaseStringUTFChars(_pcmFilePath, pcmFilePath);
    env->ReleaseStringUTFChars(_mp3FilePath, mp3FilePath);

    return result;
}

JNIEXPORT jboolean JNICALL
Java_xyz_xxin_silkdecoder_DecodeNative_silk2wav(JNIEnv *env, jclass clazz,
                                                jstring _silkFilePath,jstring _pcmFilePath, jstring _wavFilePath,
                                                jint _sampleRate) {

    String silkFilePath = env->GetStringUTFChars(_silkFilePath, 0);
    String pcmFilePath = env->GetStringUTFChars(_pcmFilePath, 0);
    String wavFilePath = env->GetStringUTFChars(_wavFilePath, 0);
    int sampleRate = (int)_sampleRate; // 采样率

    int result = 0;

    // silk转pcm
    if (silk2pcm(silkFilePath, pcmFilePath, sampleRate)) {
        // pcm转mp3
        result = pcm2wav(pcmFilePath, wavFilePath, sampleRate);
    }

    env->ReleaseStringUTFChars(_silkFilePath, silkFilePath);
    env->ReleaseStringUTFChars(_pcmFilePath, pcmFilePath);
    env->ReleaseStringUTFChars(_wavFilePath, wavFilePath);

    return result;
}

JNIEXPORT jboolean JNICALL
Java_xyz_xxin_silkdecoder_DecodeNative_silk2pcm(JNIEnv *env, jclass clazz, jstring _silkFilePath,
                                                jstring _pcmFilePath, jint _sampleRate) {

    String silkFilePath = env->GetStringUTFChars(_silkFilePath, 0);
    String pcmFilePath = env->GetStringUTFChars(_pcmFilePath, 0);
    int sampleRate = (int)_sampleRate; // 采样率

    // silk转pcm
    int result = silk2pcm(silkFilePath, pcmFilePath, sampleRate);

    env->ReleaseStringUTFChars(_silkFilePath, silkFilePath);
    env->ReleaseStringUTFChars(_pcmFilePath, pcmFilePath);

    return result;
}
}