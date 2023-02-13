#include <jni.h>
#include <string>
#include <SKP_Silk_SDK_API.h>
#include "silk.h"
#include "coder.h"

jstring getVersion(JNIEnv *env, jobject thiz) {
    return env->NewStringUTF(SKP_Silk_SDK_get_version());
}

std::string jstring2string(JNIEnv *env, jstring jStr) {
    if (!jStr) {
        return "";
    }
    const jclass stringClass = env->GetObjectClass(jStr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const auto stringJbytes = (jbyteArray) env->CallObjectMethod(jStr, getBytes,
                                                                 env->NewStringUTF("UTF-8"));

    auto length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte *pBytes = env->GetByteArrayElements(stringJbytes, nullptr);

    std::string ret = std::string((char *) pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;
}

jint nativeEncode(JNIEnv *env, jobject thiz, jstring jInput,
                                           jstring jOutput,
                                           jint sample_rate) {

    std::string input = jstring2string(env, jInput);
    std::string output = jstring2string(env, jOutput);
    int result = Coder::encode(input, output, sample_rate);
    LOG_I("encode_result: %d", result);
    return result;
}

jint  nativeDecode(JNIEnv *env, jobject obj, jstring jInput,
                                           jstring jOutput,
                                           jint sample_rate) {
    std::string input = jstring2string(env, jInput);
    std::string output = jstring2string(env, jOutput);
    int result = Coder::decode(input, output, sample_rate);
    LOG_I("decode_result: %d", result);
    return result;
}