#include <jni.h>
#include <string>
#include <fmod.hpp>  //.hpp可以理解为.h的升级版，具备兼容性
#include <unistd.h>

using namespace FMOD; //导入FMOD的命名空间

// Java层六个常量所对应的六个宏
// #undef/#define: 如果没有这个宏，就定义这个宏
#undef com_sawyer_study_jni_fmod_MainActivity_MODE_NORMAL
#define com_sawyer_study_jni_fmod_MainActivity_MODE_NORMAL 0L
#undef com_sawyer_study_jni_fmod_MainActivity_MODE_LUOLI
#define com_sawyer_study_jni_fmod_MainActivity_MODE_LUOLI 1L
#undef com_sawyer_study_jni_fmod_MainActivity_MODE_DASHU
#define com_sawyer_study_jni_fmod_MainActivity_MODE_DASHU 2L
#undef com_sawyer_study_jni_fmod_MainActivity_MODE_JINGSONG
#define com_sawyer_study_jni_fmod_MainActivity_MODE_JINGSONG 3L
#undef com_sawyer_study_jni_fmod_MainActivity_MODE_GAOGUAI
#define com_sawyer_study_jni_fmod_MainActivity_MODE_GAOGUAI 4L
#undef com_sawyer_study_jni_fmod_MainActivity_MODE_KONGLING
#define com_sawyer_study_jni_fmod_MainActivity_MODE_KONGLING 5L

extern "C"
JNIEXPORT void JNICALL
Java_com_sawyer_study_1jni_1fmod_MainActivity_voiceChangeNative(JNIEnv *env, jobject thiz,jint mode, jstring path) {
    char * _content = "默认 播放完毕";
    const char * path_ = env->GetStringUTFChars(path, nullptr);

    // 音效引擎系统指针
    System * system = nullptr;
    // 声音指针
    Sound * sound = nullptr;
    // 声音在音轨上面跑：音轨指针
    Channel * channel = nullptr;
    // DSP：digital signal process == 数字信号处理指针
    DSP * dsp = nullptr;

    //Java初始化：System system = new System();
    //C初始化:xxxx(&system);
    //todo 第一步 创建系统
    System_Create(&system);
    //todo 第二步 系统的初始化 参数1：最大音轨数， 参数2：FMOD系统初始化标记， 参数3：额外数据
    system->init(32, FMOD_INIT_NORMAL, nullptr);
    //todo 第三步 创建声音 参数1：路径， 参数2：声音初始化标记， 参数3：额外数据， 参数4：声音指针
    system->createSound(path_, FMOD_DEFAULT, nullptr, &sound);
    //todo 第四步：播放声音 音轨 声音
    //参数1：声音， 参数2：分组音轨， 参数3：控制， 参数4：通道
    system->playSound(sound, nullptr, false, &channel);

    //todo 第五步：改变音轨，创建不同的音色
    switch (mode) {
        case com_sawyer_study_jni_fmod_MainActivity_MODE_NORMAL: //原生
            _content = "原生 播放完毕";
            break;
        case com_sawyer_study_jni_fmod_MainActivity_MODE_LUOLI: //萝莉：音调高
            _content = "萝莉 播放完毕";
            //1.创建DSP类型的Pitch音调条件
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            //2.设置Pitch音调调节2.0
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 2.0f);
            //3.添加音效至音轨中
            channel->addDSP(0, dsp);
            break;
        case com_sawyer_study_jni_fmod_MainActivity_MODE_DASHU: //大叔：音调低
            _content = "大叔 播放完毕";
            //1.创建DSP类型的Pitch音效调节
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            //2.设置Pitch音效调节
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.7f);
            //3.添加音效至音轨中
            channel->addDSP(0, dsp);
            break;
        case com_sawyer_study_jni_fmod_MainActivity_MODE_JINGSONG: //惊悚：由多个音效调节构成
            _content = "惊悚 播放完毕";
            //todo 1.低音调效果_pitch
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.7f);
            channel->addDSP(0, dsp);
            //todo 2.回音效果_echo
            system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 200);
            dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 10);
            channel->addDSP(1,dsp);
            //todo 3.颤抖效果_tremolo
            system->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_TREMOLO_FREQUENCY, 20);
            dsp->setParameterFloat(FMOD_DSP_TREMOLO_SKEW, 0.8f);
            channel->addDSP(2, dsp);
            break;
        case com_sawyer_study_jni_fmod_MainActivity_MODE_GAOGUAI: //搞怪：频率快
            _content = "搞怪 播放完毕";
            //1.从音轨中可以获得当前频率
            float mFrequency;
            channel->getFrequency(&mFrequency);
            //2.修改频率
            channel->setFrequency(mFrequency * 1.8f);
            break;
        case com_sawyer_study_jni_fmod_MainActivity_MODE_KONGLING: //空灵
             _content = "空灵 播放完毕";
            //1.创建DSP类型的回音ECHO音效调节
            system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
            //2.回音延时调节，默认Default = 500
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 300);
            //3.回音 衰减度Default = 50 , 0 == 完全衰减
            dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 10);
            //4.添加音效至音轨中
            channel->addDSP(0,dsp);
            break;
    }

    //todo 等待播放完毕再回收
    bool isEnd = true;
    while (isEnd){
        //播放是否完毕的信息从音轨中可以获取。在播放完毕后，会将isEnd置为false
        channel->isPlaying(&isEnd);
        usleep(1000 * 1000);
    }

    //todo 告知Java播放完毕
    jclass mainClass = env->GetObjectClass(thiz);
    jmethodID playEndMid = env->GetMethodID(mainClass, "playerEnd", "(Ljava/lang/String;)V");
    jstring playResult = env->NewStringUTF(_content);
    env->CallVoidMethod(thiz, playEndMid, playResult);
    env->DeleteLocalRef(playResult);

    //todo 专业点：时时刻刻记得回收
    sound->release();
    system->release();
    system->close();
    env->ReleaseStringUTFChars(path, path_);
}