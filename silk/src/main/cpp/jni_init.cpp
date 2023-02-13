#include "silk.h"
#include "coder.h"

const char *classPathName = "cc/imorning/silk/SilkJni";

static JNINativeMethod methods[] = {
        {"nativeGetSilkVersion", "()Ljava/lang/String;",                     (void *) getVersion},
        {"nativeEncode",         "(Ljava/lang/String;Ljava/lang/String;I)I", (void *) nativeEncode},
        {"nativeDecode",         "(Ljava/lang/String;Ljava/lang/String;I)I", (void *) nativeDecode},
};

static int methodSize = 3;

JNIEXPORT jint  JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = nullptr;
    int jniVersion;
#ifdef JNI_VERSION_1_6
    jniVersion = JNI_VERSION_1_6;
#else
    jniVersion = JNI_VERSION_1_4;
#endif
    int ret = vm->GetEnv((void **) &env, jniVersion);
    if (ret != JNI_OK) {
        LOG_E("Get env failed:%d", ret);
        return ret;
    }
    jclass dynamicClazz = env->FindClass(classPathName);
    ret = env->RegisterNatives(dynamicClazz, methods, methodSize);
    if (ret != JNI_OK) {
        LOG_E("Get env failed:%d", ret);
        return ret;
    }
    return jniVersion;
}