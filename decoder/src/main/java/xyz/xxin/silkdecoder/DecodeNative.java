package xyz.xxin.silkdecoder;

public class DecodeNative {
    static {
        System.loadLibrary("decoder");
    }

    private DecodeNative() {}

    /**
     * silk转换pcm，支持微信、QQ
     * 如果转换成功，将会生成对应路径的pcm文件
     * @param silkFilePath  silk文件路径
     * @param pcmFilePath   pcm文件路径
     * @param sampleRate    采样率，有效值：8000、12000、16000、24000、32000、44100、48000
     * @return  返回转换结果；true:成功；false:失败
     */
    protected static native boolean silk2pcm(String silkFilePath, String pcmFilePath, int sampleRate);

    /**
     * silk转换wav，支持微信、QQ
     * 如果转换成功，将会生成对应路径的pcm和wav文件
     * @param silkFilePath  silk文件路径
     * @param pcmFilePath   pcm文件路径
     * @param wavFilePath   wav文件路径
     * @param sampleRate    采样率，有效值：8000、12000、16000、24000、32000、44100、48000
     * @return  返回转换结果；true:成功；false:失败
     */
    protected static native boolean silk2wav(String silkFilePath, String pcmFilePath, String wavFilePath, int sampleRate);

    /**
     * silk转换mp3，支持微信、QQ
     * 如果转换成功，将会生成对应路径的pcm和mp3文件
     * @param silkFilePath  silk文件路径
     * @param pcmFilePath   pcm文件路径
     * @param mp3FilePath   mp3文件路径
     * @param sampleRate    采样率，有效值：8000、12000、16000、24000、32000、44100、48000
     * @param bitrate       比特率，通常取：64、128、192
     * @return  返回转换结果；true:成功；false:失败
     */
    protected static native boolean silk2mp3(String silkFilePath, String pcmFilePath, String mp3FilePath, int sampleRate, int bitrate);
}
