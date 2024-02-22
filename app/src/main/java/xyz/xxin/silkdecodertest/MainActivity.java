package xyz.xxin.silkdecodertest;

import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import java.io.File;
import java.util.List;

import xyz.xxin.fileselector.FileSelector;
import xyz.xxin.fileselector.beans.FileBean;
import xyz.xxin.fileselector.interfaces.OnResultCallbackListener;
import xyz.xxin.fileselector.utils.SAFUtil;
import xyz.xxin.silkdecoder.SilkDecoder;

public class MainActivity extends AppCompatActivity {
    private Button start_decode;
    private Button select_file;
    private TextView file_path;
    private RadioGroup target_format;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        findView();
        addEvent();
    }

    private void addEvent() {
        // 开始解码转换
        start_decode.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String silkFilePath = file_path.getText().toString();
                if (TextUtils.isEmpty(silkFilePath)) {
                    Toast.makeText(MainActivity.this, "请先选择文件", Toast.LENGTH_SHORT).show();
                    return;
                }

                File silkFile = new File(silkFilePath);

                String pcmFilePath = silkFile.getParentFile().getAbsolutePath() + "/" + getFileName(silkFile) + ".pcm";
                String mp3FilePath = silkFile.getParentFile().getAbsolutePath() + "/" + getFileName(silkFile) + ".mp3";
                String wavFilePath = silkFile.getParentFile().getAbsolutePath() + "/" + getFileName(silkFile) + ".wav";

                boolean result;

                int checkedRadioButtonId = target_format.getCheckedRadioButtonId();
                if (checkedRadioButtonId == R.id.mp3) {
                    result = SilkDecoder.decodeToMp3(silkFilePath, mp3FilePath, 24000, 128);
                } else if (checkedRadioButtonId == R.id.wav){
                    result = SilkDecoder.decodeToWav(silkFilePath, wavFilePath, 24000);
                } else {
                    result = SilkDecoder.decodeToPcm(silkFilePath, pcmFilePath, 24000);
                }

                if (result) {
                    Toast.makeText(MainActivity.this, "转换成功", Toast.LENGTH_SHORT).show();
                } else {
                    Toast.makeText(MainActivity.this, "转换失败", Toast.LENGTH_SHORT).show();
                }
            }
        });

        // 选择文件
        select_file.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                FileSelector.create(MainActivity.this)
                        .isSingle(true)
                        .isOnlySelectFile(true)
                        .forResult(new OnResultCallbackListener() {
                            @Override
                            public void onResult(List<FileBean> result) {
                                FileBean fileBean = result.get(0);
                                if (fileBean.isFileType()) {
                                    file_path.setText(fileBean.getFile().getAbsolutePath());
                                } else {
                                    file_path.setText(SAFUtil.uriToPath(fileBean.getDocumentFile()));
                                }
                            }

                            @Override
                            public void onCancel() {

                            }
                        });
            }
        });
    }

    private void findView() {
        start_decode = findViewById(R.id.start_decode);
        select_file = findViewById(R.id.select_file);
        file_path = findViewById(R.id.file_path);
        target_format = findViewById(R.id.target_format);
    }

    private String getFileName(File file) {
        String name = file.getName();
        int pointIndex = name.lastIndexOf(".");
        if (pointIndex >= 0) {
            return name.substring(0, pointIndex);
        }
        return name;
    }
}

