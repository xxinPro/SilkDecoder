package xyz.xxin.silkdecoder;

public class DecodeNative {
    static {
        System.loadLibrary("decoder");
    }

    /**
     * silk转换mp3，支持微信、QQ
     * 如果转换成功，将会生成对应路径的pcm和mp3文件
     * @param silkFilePath  silk文件路径
     * @param pcmFilePath   pcm文件路径
     * @param mp3FilePath   mp3文件路径
     * @param sampleRate    采样率
     * @return  转换结果
     */
    public static native boolean silk2mp3(String silkFilePath, String pcmFilePath, String mp3FilePath, int sampleRate);
}
