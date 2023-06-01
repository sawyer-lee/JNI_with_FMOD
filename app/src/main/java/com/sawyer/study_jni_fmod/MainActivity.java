package com.sawyer.study_jni_fmod;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.sawyer.study_jni_fmod.databinding.ActivityMainBinding;

import org.fmod.FMOD;

public class MainActivity extends AppCompatActivity {


    private String path;    // 音频路径
    private static final int MODE_NORMAL = 0;       // 正常
    private static final int MODE_LUOLI = 1;        // 萝莉
    private static final int MODE_DASHU = 2;        // 大叔
    private static final int MODE_JINGSONG = 3;     // 惊悚
    private static final int MODE_GAOGUAI = 4;      // 搞怪
    private static final int MODE_KONGLING = 5;     // 空灵

    //最终目标为生成：libstudy_jni_fmod.so 动态库
    //libstudy_jni_fmod.a 表示静态库
    static {
        System.loadLibrary("study_jni_fmod");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        path = "file:///android_asset/derry.mp3";
        FMOD.init(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        FMOD.close();
    }

    // 六个 点击事件
    @SuppressLint("NonConstantResourceId")
    public void onFix(View view) {
        switch (view.getId()) {
            case R.id.btn_normal:
                voiceChangeNative(MODE_NORMAL, path);
                break;
            case R.id.btn_luoli:
                voiceChangeNative(MODE_LUOLI, path);
                break;
            case R.id.btn_dashu:
                voiceChangeNative(MODE_DASHU, path);
                break;
            case R.id.btn_jingsong:
                voiceChangeNative(MODE_JINGSONG, path);
                break;
            case R.id.btn_gaoguai:
                voiceChangeNative(MODE_GAOGUAI, path);
                break;
            case R.id.btn_kongling:
                voiceChangeNative(MODE_KONGLING, path);
                break;
        }
    }

    private native void voiceChangeNative(int mode, String path);

    // 给C++调用的函数
    // JNI 调用 Java函数的时候，忽略掉 私有、公开 等
    private void playerEnd(String msg) {
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
    }

}