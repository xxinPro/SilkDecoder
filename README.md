# SilkDecoder 1.0.0

- [引用方式](#引用方式)<br>
- [使用方式](#使用方式)<br>
- [体验demo](#体验demo)<br>


## 引用方式

`Gradle 7.0`以下，需要在项目级`build.gradle`文件中加入

```sh
allprojects {
    repositories {
        ...
        maven { url 'https://jitpack.io' }
    }
}
```

`Gradle 7.0`以上，需要在`setting.gradle`文件中加入

```sh
dependencyResolutionManagement {
	repositories {
		...
		maven { url 'https://jitpack.io' }
	}
}
```

远程仓库配置之后，即可在模块的`build.gradle`中引入`FileSelector`

```sh
dependencies {
    implementation 'com.github.xxinPro:SilkDecoder:1.0'
}
```

## 使用方式

### 解码为mp3

将silk解码为mp3

```java
SilkDecoder.decodeToMp3(silkFilePath, mp3FilePath);
```

将silk解码为mp3并自定义`采样率`和`比特率`

- 采样率：有效值：`8000`、`12000`、`16000`、`24000`、`32000`、`44100`、`48000`
- 比特率：通常取：`64`、`128`、`192`

```java
SilkDecoder.decodeToMp3(silkFilePath, mp3FilePath, sampleRate, bitrate);
```

### 解码为wav

将silk解码为wav

```java
SilkDecoder.decodeToWav(silkFilePath, wavFilePath);
```

将silk解码为wav并自定义`采样率`

- 采样率：有效值：`8000`、`12000`、`16000`、`24000`、`32000`、`44100`、`48000`

```java
SilkDecoder.decodeToWav(silkFilePath, wavFilePath, sampleRate);
```

### 解码为pcm

将silk解码为pcm

```java
SilkDecoder.decodeToPcm(silkFilePath, pcmFilePath);
```

将silk解码为pcm并自定义`采样率`

- 采样率：有效值：`8000`、`12000`、`16000`、`24000`、`32000`、`44100`、`48000`

```java
SilkDecoder.decodeToPcm(silkFilePath, pcmFilePath, sampleRate);
```

## 体验demo

[点击下载](./app/release/app-release.apk)
