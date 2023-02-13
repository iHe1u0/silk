//
// Created by iMorning on 2022/8/29.
//
#pragma once

#include <jni.h>
#include <android/log.h>
#include <string>
#include <SKP_Silk_SDK_API.h>

/* Seed for the random number generator, which is used for simulating packet loss */
static SKP_int32 rand_seed = 1;
/* Define codec specific settings */
#define MAX_BYTES_PER_FRAME     250 // Equals peak bitrate of 100 kbps
#define MAX_INPUT_FRAMES        5
#define FRAME_LENGTH_MS         20
#define MAX_API_FS_KHZ          48
#define MAX_LBRR_DELAY          2

/* For log */
#define DEBUG
#define TAG "NativeCode"
#ifdef DEBUG

#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR,    TAG, __VA_ARGS__)
#define LOG_W(...) __android_log_print(ANDROID_LOG_WARN,     TAG, __VA_ARGS__)
#define LOG_I(...) __android_log_print(ANDROID_LOG_INFO,     TAG, __VA_ARGS__)
#define LOG_D(...) __android_log_print(ANDROID_LOG_DEBUG,    TAG, __VA_ARGS__)
#else
#define LOG_E(...)
#define LOG_W(...)
#define LOG_I(...)
#define LOG_D(...)
#endif

enum SampleRate {
    SAMPLE_RATE_8K = 8000,
    SAMPLE_RATE_12K = 12000,
    SAMPLE_RATE_16K = 16000,
    SAMPLE_RATE_24K = 24000,
    SAMPLE_RATE_32K = 32000,
    SAMPLE_RATE_44K = 44100,
    SAMPLE_RATE_48K = 48000
};

enum MaxSample {
    MAX_SAMPLE_RATE_8K = 8000,
    MAX_SAMPLE_RATE_12K = 12000,
    MAX_SAMPLE_RATE_16K = 16000,
    MAX_SAMPLE_RATE_24K = 24000
};

class Coder {
public:
    static int decode(const std::string &inputFile,
                      const std::string &outputFile,
                      const SKP_int32 sampleRate = 24000);

    /**
     * Encode input file
     * @param inputFile input file path,must is a pcm file
     * @param outputFile output file path
     * @param sampleRate sample rate
     * @return 0 if success
     */
    static int encode(const std::string &inputFile,
                      const std::string &outputFile,
                      const SKP_int32 sampleRate = 0);

private:
    static unsigned long resolutionTime();
};
