/**
 * @file silk.h
 * @author fkkt-55(admin@catcompany.cn)
 * @brief Silk encode and decode sample code
 * @version 0.1
 * @date 2023-07-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <string>

std::string getVersion();

int nativeEncode(std::string jInput, std::string jOutput, int sample_rate);

int nativeDecode( std::string jInput, std::string jOutput, int sample_rate);