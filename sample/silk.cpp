#include <string>
#include <SKP_Silk_SDK_API.h>
#include "silk.h"
#include "coder.h"

std::string getVersion() {
    return SKP_Silk_SDK_get_version();
}

int nativeEncode(std::string input,std::string output,int sample_rate) {
    int result = Coder::encode(input, output, sample_rate);
    LOG_I("encode_result: %d", result);
    return result;
}

int nativeDecode(std::string input,std::string output,int sample_rate) {
    int result = Coder::decode(input, output, sample_rate);
    LOG_I("decode_result: %d", result);
    return result;
}