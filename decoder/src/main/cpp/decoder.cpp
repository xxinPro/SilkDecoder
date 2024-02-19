
#include "decoder.h"

extern "C" {
JNIEXPORT jboolean JNICALL
Java_xyz_xxin_silkdecoder_DecodeNative_silk2mp3(JNIEnv *env, jclass clazz,
                                                jstring _silkFilePath, jstring _pcmFilePath, jstring _mp3FilePath,
                                                jint _sampleRate) {

    String silkFilePath = env->GetStringUTFChars(_silkFilePath, 0);
    String pcmFilePath = env->GetStringUTFChars(_pcmFilePath, 0);
    String mp3FilePath = env->GetStringUTFChars(_mp3FilePath, 0);
    int sampleRate = (int)_sampleRate;

    int result = 0;

    // silk转pcm
    if (silk2pcm(silkFilePath, pcmFilePath, sampleRate)) {
        // pcm转mp3
        result = pcm2mp3(pcmFilePath, mp3FilePath, sampleRate);
    }

    env->ReleaseStringUTFChars(_silkFilePath, silkFilePath);
    env->ReleaseStringUTFChars(_pcmFilePath, pcmFilePath);

    return result;
}
}