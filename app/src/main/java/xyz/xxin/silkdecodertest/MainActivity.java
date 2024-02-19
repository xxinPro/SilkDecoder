package xyz.xxin.silkdecodertest;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.io.File;
import java.util.List;

import xyz.xxin.fileselector.FileSelector;
import xyz.xxin.fileselector.beans.FileBean;
import xyz.xxin.fileselector.interfaces.OnResultCallbackListener;
import xyz.xxin.fileselector.utils.SAFUtil;
import xyz.xxin.silkdecoder.DecodeNative;

public class MainActivity extends AppCompatActivity {
    private Button start;
    private Button select_file;
    private EditText edit_text;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        findView();
        addEvent();
    }

    private void addEvent() {
        // 开始转换
        start.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String silkFilePath = edit_text.getText().toString();
                if (TextUtils.isEmpty(silkFilePath))
                    return;

                File silkFile = new File(silkFilePath);

                String pcmFilePath = silkFile.getParentFile().getAbsolutePath() + "/" + getFileName(silkFile) + "_temp.pcm";
                String mp3FilePath = silkFile.getParentFile().getAbsolutePath() + "/" + getFileName(silkFile) + ".mp3";

                boolean result = DecodeNative.silk2mp3(silkFilePath, pcmFilePath, mp3FilePath, 24000);
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
                                    edit_text.setText(fileBean.getFile().getAbsolutePath());
                                } else {
                                    edit_text.setText(SAFUtil.uriToPath(fileBean.getDocumentFile()));
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
        start = findViewById(R.id.start);
        select_file = findViewById(R.id.select_file);
        edit_text = findViewById(R.id.edit_text);
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

