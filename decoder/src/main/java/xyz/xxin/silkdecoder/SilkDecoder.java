package xyz.xxin.silkdecoder;

import java.io.File;

public class SilkDecoder {


    /**
     * 将silk解码为mp3
     * @param silkFilePath  silk文件路径
     * @param mp3FilePath   mp3文件路径
     * @return  返回转换结果；true:成功；false:失败
     */
    public static boolean decodeToMp3(String silkFilePath, String mp3FilePath) {
        return decodeToMp3(silkFilePath, mp3FilePath, 24000, 128);
    }

    /**
     * 将silk解码为mp3
     * @param silkFilePath  silk文件路径
     * @param mp3FilePath   mp3文件路径
     * @param sampleRate    采样率，有效值：8000、12000、16000、24000、32000、44100、48000
     * @param bitrate       比特率，通常取：64、128、192
     * @return  返回转换结果；true:成功；false:失败
     */
    public static boolean decodeToMp3(String silkFilePath, String mp3FilePath, int sampleRate, int bitrate) {
        File silkFile = new File(silkFilePath);

        if (!silkFile.exists() || silkFile.isDirectory())
            return false;

        String tempPcmPath = getTempPcmPath(silkFilePath);
        boolean result  = DecodeNative.silk2mp3(silkFilePath, tempPcmPath, mp3FilePath, sampleRate, bitrate);

        new File(tempPcmPath).delete();

        return result;
    }

    /**
     * 将silk解码为wav
     * @param silkFilePath  silk文件路径
     * @param wavFilePath   wav文件路径
     * @return  返回转换结果；true:成功；false:失败
     */
    public static boolean decodeToWav(String silkFilePath, String wavFilePath) {
        return decodeToWav(silkFilePath, wavFilePath, 24000);
    }

    /**
     * 将silk解码为wav
     * @param silkFilePath  silk文件路径
     * @param wavFilePath   wav文件路径
     * @param sampleRate    采样率，有效值：8000、12000、16000、24000、32000、44100、48000
     * @return  返回转换结果；true:成功；false:失败
     */
    public static boolean decodeToWav(String silkFilePath, String wavFilePath, int sampleRate) {
        File silkFile = new File(silkFilePath);

        if (!silkFile.exists() || silkFile.isDirectory())
            return false;

        String tempPcmPath = getTempPcmPath(silkFilePath);
        boolean result  = DecodeNative.silk2wav(silkFilePath, tempPcmPath, wavFilePath, sampleRate);

        new File(tempPcmPath).delete();

        return result;
    }

    /**
     * 将silk解码为pcm
     * @param silkFilePath  silk文件路径
     * @param pcmFilePath   pcm文件路径
     * @return  返回转换结果；true:成功；false:失败
     */
    public static boolean decodeToPcm(String silkFilePath, String pcmFilePath) {
        return decodeToPcm(silkFilePath, pcmFilePath, 24000);
    }

    /**
     * 将silk解码为pcm
     * @param silkFilePath  silk文件路径
     * @param pcmFilePath   pcm文件路径
     * @param sampleRate    采样率，有效值：8000、12000、16000、24000、32000、44100、48000
     * @return  返回转换结果；true:成功；false:失败
     */
    public static boolean decodeToPcm(String silkFilePath, String pcmFilePath, int sampleRate) {
        File silkFile = new File(silkFilePath);

        if (!silkFile.exists() || silkFile.isDirectory())
            return false;

        return DecodeNative.silk2pcm(silkFilePath, pcmFilePath, sampleRate);
    }

    private static String getTempPcmPath(String filePath) {
        return filePath + "__decode_temp.pcm";
    }
}
