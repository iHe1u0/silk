#pragma once

#include <jni.h>
#include <string>

jstring getVersion(JNIEnv *env, jobject thiz);

std::string jstring2string(JNIEnv *env, jstring jStr);

jint nativeEncode(JNIEnv *env, jobject thiz,
                  jstring jInput,
                  jstring jOutput,
                  jint sample_rate);

jint nativeDecode(JNIEnv *env, jobject obj,
                  jstring jInput,
                  jstring jOutput,
                  jint sample_rate);