cmake_minimum_required(VERSION 3.10.2) #最低支持的cmake版本

project("study_jni_fmod")

#批量导入C\C++文件
file(GLOB allCPP *.c *.cpp)

# todo 第一步：导入fmod的相关头文件
include_directories(inc) #相对路径，因为inc文件夹与CMakeLists.txt平级
# todo 第二步：导入fmod的相关库文件
#CMAKE_CXX_FLAGS: 设置C++的环境变量
# -L: 后面可以拼接别的环境变量
#CMAKE_SOURCE_DIR: 表示CMakeLists.txt文件的全路径
#CMAKE_ANDROID_ARCH_ABI: 表示可以去自动寻找jniLibs下各个平台的so库文件
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L ${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}")


add_library(
        study_jni_fmod # 最终目标生成libstudy_jni_fmod.so动态库
        SHARED #动态库
        #[[native-lib.cpp T1.cpp T20.cpp]]
        #简化写法，批量导入
        ${allCPP}
)

# 查找日志打印的库 D:\Develop\sdk\ndk\21.0.6113669\platforms\android-18\archarm\usr\lib\liblog.so
find_library(
        log-lib #起的变量名
        log)

target_link_libraries(
        study_jni_fmod
        ${log-lib} #使用上边定义的变量名，达到寻找liblog.so库的目的
        fmod  # 系统会自动拼接为libfmod.so，即jniLibs文件夹下的so库
        fmodL # todo 第三步：把具体的库，链接打包到 libstudy_jni_fmod.so 里面去
)