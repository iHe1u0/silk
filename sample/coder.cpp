#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>
#include <SKP_Silk_SDK_API.h>
#include <SKP_Silk_main_FIX.h>
#include <chrono>
#include "coder.h"

unsigned long Coder::resolutionTime() {
  // 使用 std::chrono::high_resolution_clock 获取当前时间
  std::chrono::high_resolution_clock::time_point _now = std::chrono::high_resolution_clock::now();

  // 将时间转换为微秒
  auto duration = _now.time_since_epoch();

  // 返回时间
  return duration.count();
}


int
Coder::decode(const std::string &inputFile, const std::string &outputFile, SKP_int32 sampleRate) {
    unsigned long totalTime, startTime;
    double fileTime;
    size_t counter;
    SKP_int32 totPackets, i, k;
    SKP_int16 ret, len, tot_len;
    SKP_int16 nBytes;
    SKP_uint8 payload[MAX_BYTES_PER_FRAME * MAX_INPUT_FRAMES * (MAX_LBRR_DELAY + 1)];
    SKP_uint8 *payloadEnd, *payloadToDec = nullptr;
    SKP_uint8 FECpayload[MAX_BYTES_PER_FRAME * MAX_INPUT_FRAMES], *payloadPtr;
    SKP_int16 nBytesFEC;
    SKP_int16 nBytesPerPacket[MAX_LBRR_DELAY + 1], totBytes;
    SKP_int16 out[((FRAME_LENGTH_MS * MAX_API_FS_KHZ) << 1) * MAX_INPUT_FRAMES], *outPtr;
    char speechOutFileName[150], bitInFileName[150];
    //define input and output file
    FILE *bitInFile, *speechOutFile;
    SKP_int32 packetSize_ms = 0;
    SKP_int32 decSizeBytes;
    void *psDec;
    SKP_float loss_prob;
    SKP_int32 frames, lost, quiet;
    SKP_SILK_SDK_DecControlStruct DecControl;
    /* default settings */
    quiet = 0;
    loss_prob = 0.0f;

    /* get arguments */
    strcpy(bitInFileName, inputFile.c_str());
    strcpy(speechOutFileName, outputFile.c_str());

#ifdef DEBUG
    LOG_D("********** Silk Decoder (Fixed Point) v %s ********************",
          SKP_Silk_SDK_get_version());
    LOG_D("********** Compiled for %d bit cpu *******************************",
          (int) sizeof(void *) * 8);
    LOG_D("Input:                       %s", bitInFileName);
    LOG_D("Output:                      %s", speechOutFileName);
#endif

    /* Open files */
    bitInFile = fopen(bitInFileName, "rb");
    if (bitInFile == nullptr) {
        LOG_E("Error: could not open input file %s", bitInFileName);
        return -1;
    }

    /* Check Silk header */
    {
        char header_buf[50];
        counter = fread(header_buf, sizeof(char), strlen("#!SILK_V3"), bitInFile);
        header_buf[strlen("#!SILK_V3")] = '\0'; /* Terminate with a null character */
        if (strcmp(header_buf, "#!SILK_V3") != 0) {
            /* Non-equal strings */
            LOG_E("Error: Wrong Header %s", header_buf);
            return -1;
        }
    }

    speechOutFile = fopen(speechOutFileName, "wb");
    if (speechOutFile == nullptr) {
        LOG_E("Error: could not open output file %s", speechOutFileName);
        return -1;
    }

    /* Set the samplingrate that is requested for the output */
    if (sampleRate == 0) {
        DecControl.API_sampleRate = 24000;
    } else {
        DecControl.API_sampleRate = sampleRate;
    }

    /* Initialize to one frame per packet, for proper concealment before first packet arrives */
    DecControl.framesPerPacket = 1;

    /* Create Coder */
    ret = SKP_Silk_SDK_Get_Decoder_Size(&decSizeBytes);
    if (ret) {
        LOG_D("SKP_Silk_SDK_Get_Decoder_Size returned %d", ret);
    }
    psDec = malloc(decSizeBytes);

    /* Reset Coder */
    ret = SKP_Silk_SDK_InitDecoder(psDec);
    if (ret) {
        LOG_D("SKP_Silk_InitDecoder returned %d", ret);
    }

    totPackets = 0;
    totalTime = 0;
    payloadEnd = payload;

    /* Simulate the jitter buffer holding MAX_FEC_DELAY packets */
    for (i = 0; i < MAX_LBRR_DELAY; i++) {
        /* Read payload size */
        fread(&nBytes, sizeof(SKP_int16), 1, bitInFile);
        /* Read payload */
        counter = fread(payloadEnd, sizeof(SKP_uint8), nBytes, bitInFile);

        if ((SKP_int16) counter < nBytes) {
            break;
        }
        nBytesPerPacket[i] = nBytes;
        payloadEnd += nBytes;
        totPackets++;
    }

    while (true) {
        /* Read payload size */
        counter = fread(&nBytes, sizeof(SKP_int16), 1, bitInFile);
        if (nBytes < 0 || counter < 1) {
            break;
        }

        /* Read payload */
        counter = fread(payloadEnd, sizeof(SKP_uint8), nBytes, bitInFile);
        if ((SKP_int16) counter < nBytes) {
            break;
        }

        /* Simulate losses */
        rand_seed = SKP_RAND(rand_seed);
        if ((((float) ((rand_seed >> 16) + (1 << 15))) / 65535.0f >= (loss_prob / 100.0f)) &&
            (counter > 0)) {
            nBytesPerPacket[MAX_LBRR_DELAY] = nBytes;
            payloadEnd += nBytes;
        } else {
            nBytesPerPacket[MAX_LBRR_DELAY] = 0;
        }

        if (nBytesPerPacket[0] == 0) {
            /* Indicate lost packet */
            lost = 1;
            /* Packet loss. Search after FEC in next packets. Should be done in the jitter buffer */
            payloadPtr = payload;
            for (i = 0; i < MAX_LBRR_DELAY; i++) {
                if (nBytesPerPacket[i + 1] > 0) {
                    startTime = resolutionTime();
                    SKP_Silk_SDK_search_for_LBRR(payloadPtr,
                                                 nBytesPerPacket[i + 1],
                                                 (i + 1),
                                                 FECpayload,
                                                 &nBytesFEC);
                    totalTime += resolutionTime() - startTime;
                    if (nBytesFEC > 0) {
                        payloadToDec = FECpayload;
                        nBytes = nBytesFEC;
                        lost = 0;
                        break;
                    }
                }
                payloadPtr += nBytesPerPacket[i + 1];
            }
        } else {
            lost = 0;
            nBytes = nBytesPerPacket[0];
            payloadToDec = payload;
        }

        /* Silk Coder */
        outPtr = out;
        tot_len = 0;
        startTime = resolutionTime();

        if (lost == 0) {
            /* No Loss: Decode all frames in the packet */
            frames = 0;
            do {
                /* Decode 20 ms */
                ret = SKP_Silk_SDK_Decode(psDec,
                                          &DecControl,
                                          0, payloadToDec,
                                          nBytes,
                                          outPtr,
                                          &len);
                if (ret) {
                    LOG_D("SKP_Silk_SDK_Decode returned %d", ret);
                }

                frames++;
                outPtr += len;
                tot_len += len;
                if (frames > MAX_INPUT_FRAMES) {
                    /* Hack for corrupt stream that could generate too many frames */
                    outPtr = out;
                    tot_len = 0;
                    frames = 0;
                }
                /* Until last 20 ms frame of packet has been decoded */
            } while (DecControl.moreInternalDecoderFrames);
        } else {
            /* Loss: Decode enough frames to cover one packet duration */
            for (i = 0; i < DecControl.framesPerPacket; i++) {
                /* Generate 20 ms */
                ret = SKP_Silk_SDK_Decode(psDec,
                                          &DecControl,
                                          1,
                                          payloadToDec,
                                          nBytes,
                                          outPtr,
                                          &len);
                if (ret) {
                    LOG_D("SKP_Silk_Decode returned %d", ret);
                }
                outPtr += len;
                tot_len += len;
            }
        }
        packetSize_ms = tot_len / (DecControl.API_sampleRate / 1000);
        totalTime += resolutionTime() - startTime;
        totPackets++;
        /* Write output to file */
        fwrite(out, sizeof(SKP_int16), tot_len, speechOutFile);
        /* Update buffer */
        totBytes = 0;
        for (i = 0; i < MAX_LBRR_DELAY; i++) {
            totBytes += nBytesPerPacket[i + 1];
        }
        SKP_memmove(payload, &payload[nBytesPerPacket[0]], totBytes * sizeof(SKP_uint8));
        payloadEnd -= nBytesPerPacket[0];
        SKP_memmove(nBytesPerPacket, &nBytesPerPacket[1], MAX_LBRR_DELAY * sizeof(SKP_int16));
    }

    /* Empty the receive buffer */
    for (k = 0; k < MAX_LBRR_DELAY; k++) {
        if (nBytesPerPacket[0] == 0) {
            /* Indicate lost packet */
            lost = 1;

            /* Packet loss. Search after FEC in next packets. Should be done in the jitter buffer */
            payloadPtr = payload;
            for (i = 0; i < MAX_LBRR_DELAY; i++) {
                if (nBytesPerPacket[i + 1] > 0) {
                    startTime = resolutionTime();
                    SKP_Silk_SDK_search_for_LBRR(payloadPtr, nBytesPerPacket[i + 1], (i + 1),
                                                 FECpayload, &nBytesFEC);
                    totalTime += resolutionTime() - startTime;
                    if (nBytesFEC > 0) {
                        payloadToDec = FECpayload;
                        nBytes = nBytesFEC;
                        lost = 0;
                        break;
                    }
                }
                payloadPtr += nBytesPerPacket[i + 1];
            }
        } else {
            lost = 0;
            nBytes = nBytesPerPacket[0];
            payloadToDec = payload;
        }

        /* Silk Coder */
        outPtr = out;
        tot_len = 0;
        startTime = resolutionTime();

        if (lost == 0) {
            /* No loss: Decode all frames in the packet */
            frames = 0;
            do {
                /* Decode 20 ms */
                ret = SKP_Silk_SDK_Decode(psDec,
                                          &DecControl,
                                          0,
                                          payloadToDec,
                                          nBytes,
                                          outPtr,
                                          &len);
                if (ret) {
                    LOG_D("SKP_Silk_SDK_Decode returned %d", ret);
                }

                frames++;
                outPtr += len;
                tot_len += len;
                if (frames > MAX_INPUT_FRAMES) {
                    /* Hack for corrupt stream that could generate too many frames */
                    outPtr = out;
                    tot_len = 0;
                    frames = 0;
                }
                /* Until last 20 ms frame of packet has been decoded */
            } while (DecControl.moreInternalDecoderFrames);
        } else {
            /* Loss: Decode enough frames to cover one packet duration */
            /* Generate 20 ms */
            for (i = 0; i < DecControl.framesPerPacket; i++) {
                ret = SKP_Silk_SDK_Decode(
                        psDec,
                        &DecControl,
                        1,
                        payloadToDec,
                        nBytes,
                        outPtr,
                        &len);
                if (ret) {
                    LOG_D("SKP_Silk_Decode returned %d", ret);
                }
                outPtr += len;
                tot_len += len;
            }
        }

        packetSize_ms = tot_len / (DecControl.API_sampleRate / 1000);
        totalTime += resolutionTime() - startTime;
        totPackets++;

        /* Write output to file */
        fwrite(out, sizeof(SKP_int16), tot_len, speechOutFile);

        /* Update Buffer */
        totBytes = 0;
        for (i = 0; i < MAX_LBRR_DELAY; i++) {
            totBytes += nBytesPerPacket[i + 1];
        }
        SKP_memmove(payload, &payload[nBytesPerPacket[0]], totBytes * sizeof(SKP_uint8));
        payloadEnd -= nBytesPerPacket[0];
        SKP_memmove(nBytesPerPacket, &nBytesPerPacket[1], MAX_LBRR_DELAY * sizeof(SKP_int16));

        if (!quiet) {
            LOG_W("Packets decoded: %d", totPackets);
        }
    }

    if (!quiet) {
        LOG_D("Decoding Finished ");
    }

    /* Free Coder */
    free(psDec);

    /* Close files */
    fclose(speechOutFile);
    fclose(bitInFile);

    fileTime = totPackets * 1e-3 * packetSize_ms;
    if (!quiet) {
        LOG_D("File length:                 %.3f s", fileTime);
        LOG_D("Time for decoding:           %.3f s (%.3f%% of realtime)", 1e-6 * totalTime,
              1e-4 * totalTime / fileTime);
    }
    return 0;
}

int Coder::encode(const std::string &inputFile, const std::string &outputFile,
                  const SKP_int32 apiSampleRate) {
    unsigned long totalTime, startTime;
    double fileTime;
    size_t counter;
    SKP_int32 k, totPackets, totActPackets, ret;
    SKP_int16 nBytes;
    double sumBytes, sumActBytes, avg_rate, act_rate, nrg;
    SKP_uint8 payload[MAX_BYTES_PER_FRAME * MAX_INPUT_FRAMES];
    SKP_int16 in[FRAME_LENGTH_MS * MAX_API_FS_KHZ * MAX_INPUT_FRAMES];
    char inputFilePath[150], outputFilePath[150];
    FILE *bitOutFile, *speechInFile;
    SKP_int32 encSizeBytes;
    void *psEnc;
    /* default settings */
    SKP_int32 max_internal_fs_Hz = 0;
    SKP_int32 targetRate_bps = 25000;
    SKP_int32 smplsSinceLastPacket, packetSize_ms = 20;
    SKP_int32 frameSizeReadFromFile_ms = 20;
    SKP_int32 packetLoss_perc = 0;
    SKP_int32 compldoExity_mode = 2;
    SKP_int32 DTX_enabled = 0, INBandFEC_enabled = 0, quiet = 0;
    SKP_SILK_SDK_EncControlStruct encControl; // Struct for input to encoder
    SKP_SILK_SDK_EncControlStruct encStatus;  // Struct for status of encoder
    strcpy(inputFilePath, inputFile.c_str());
    strcpy(outputFilePath, outputFile.c_str());
    /* If no max internal is specified, set to minimum of API fs and 24 kHz */
    if (max_internal_fs_Hz == 0) {
        max_internal_fs_Hz = 24000;
        if (apiSampleRate < max_internal_fs_Hz) {
            max_internal_fs_Hz = apiSampleRate;
        }
    }

#ifdef DEBUG
    LOG_D("********** Silk Encoder (Fixed Point) v %s ********************",
          SKP_Silk_SDK_get_version());
    LOG_D("********** Compiled for %d bit cpu ******************************* ",
          (int) sizeof(void *) * 8);
    LOG_D("Input:                          %s", inputFilePath);
    LOG_D("Output:                         %s", outputFilePath);
    LOG_D("API sampling rate:              %d Hz", apiSampleRate);
    LOG_D("Maximum internal sampling rate: %d Hz", max_internal_fs_Hz);
    LOG_D("Packet interval:                %d ms", packetSize_ms);
    LOG_D("Inband FEC used:                %d", INBandFEC_enabled);
    LOG_D("DTX used:                       %d", DTX_enabled);
    LOG_D("CompldoExity:                     %d", compldoExity_mode);
    LOG_D("Target bitrate:                 %d bps", targetRate_bps);
#endif

    /* Open files */
    speechInFile = fopen(inputFilePath, "rb");
    if (speechInFile == nullptr) {
        LOG_E("Error: could not open input file %s", inputFilePath);
        return -1;
    }
    bitOutFile = fopen(outputFilePath, "wb");
    if (bitOutFile == nullptr) {
        LOG_E("Error: could not open output file %s", outputFilePath);
        return -1;
    }

    /* Add Silk header to stream */
    {
        static const char Silk_header[] = "#!SILK_V3";
        fwrite(Silk_header, sizeof(char), strlen(Silk_header), bitOutFile);
    }

    /* Create Encoder */
    ret = SKP_Silk_SDK_Get_Encoder_Size(&encSizeBytes);
    if (ret) {
        LOG_E("Error: SKP_Silk_create_encoder returned %d", ret);
        return ret;
    }

    psEnc = malloc(encSizeBytes);

    /* Reset Encoder */
    ret = SKP_Silk_SDK_InitEncoder(psEnc, &encStatus);
    if (ret) {
        LOG_E("Error: SKP_Silk_reset_encoder returned %d", ret);
        return ret;
    }

    /* Set Encoder parameters */
    encControl.API_sampleRate = apiSampleRate;
    encControl.maxInternalSampleRate = max_internal_fs_Hz;
    encControl.packetSize = (packetSize_ms * apiSampleRate) / 1000;
    encControl.packetLossPercentage = packetLoss_perc;
    encControl.useInBandFEC = INBandFEC_enabled;
    encControl.useDTX = DTX_enabled;
    encControl.complexity = compldoExity_mode;
    encControl.bitRate = (targetRate_bps > 0 ? targetRate_bps : 0);

    if (apiSampleRate > MAX_API_FS_KHZ * 1000 || apiSampleRate < 0) {
        LOG_D("Error: API sampling rate = %d out of range, valid range 8000 - 48000",
              apiSampleRate);
        return -1;
    }

    totalTime = 0;
    totPackets = 0;
    totActPackets = 0;
    smplsSinceLastPacket = 0;
    sumBytes = 0.0;
    sumActBytes = 0.0;
    smplsSinceLastPacket = 0;

    while (true) {
        /* Read input from file */
        counter = fread(in, sizeof(SKP_int16), (frameSizeReadFromFile_ms * apiSampleRate) / 1000,
                        speechInFile);
#ifdef _SYSTEM_IS_BIG_ENDIAN
        swap_endian( in, counter );
#endif
        if ((SKP_int) counter < ((frameSizeReadFromFile_ms * apiSampleRate) / 1000)) {
            break;
        }

        /* max payload size */
        nBytes = MAX_BYTES_PER_FRAME * MAX_INPUT_FRAMES;

        startTime = resolutionTime();

        /* Silk Encoder */
        ret = SKP_Silk_SDK_Encode(psEnc, &encControl, in, (SKP_int16) counter, payload, &nBytes);
        if (ret) {
            LOG_D("SKP_Silk_Encode returned %d", ret);
        }

        totalTime += resolutionTime() - startTime;

        /* Get packet size */
        packetSize_ms = (SKP_int) ((1000 * (SKP_int32) encControl.packetSize) /
                                   encControl.API_sampleRate);

        smplsSinceLastPacket += (SKP_int) counter;

        if (((1000 * smplsSinceLastPacket) / apiSampleRate) == packetSize_ms) {
            /* Sends a dummy zero size packet in case of DTX period  */
            /* to make it work with the decoder test program.        */
            /* In practice should be handled by RTP sequence numbers */
            totPackets++;
            sumBytes += nBytes;
            nrg = 0.0;
            for (k = 0; k < (SKP_int) counter; k++) {
                nrg += in[k] * (double) in[k];
            }
            if ((nrg / (SKP_int) counter) > 1e3) {
                sumActBytes += nBytes;
                totActPackets++;
            }

            /* Write payload size */
            fwrite(&nBytes, sizeof(SKP_int16), 1, bitOutFile);
            /* Write payload */
            fwrite(payload, sizeof(SKP_uint8), nBytes, bitOutFile);
            smplsSinceLastPacket = 0;
        }
    }

    /* Write dummy because it can not end with 0 bytes */
    nBytes = -1;

    /* Write payload size */
    fwrite(&nBytes, sizeof(SKP_int16), 1, bitOutFile);

    /* Free Encoder */
    free(psEnc);

    fclose(speechInFile);
    fclose(bitOutFile);

    fileTime = totPackets * 1e-3 * packetSize_ms;
    avg_rate = 8.0 / packetSize_ms * sumBytes / totPackets;
    act_rate = 8.0 / packetSize_ms * sumActBytes / totActPackets;
    if (!quiet) {
        LOG_D("File length:                    %.3f s", fileTime);
        LOG_D("Time for encoding:              %.3f s (%.3f%% of realtime)", 1e-6 * totalTime,
              1e-4 * totalTime / fileTime);
        LOG_D("Average bitrate:                %.3f kbps", avg_rate);
        LOG_D("Active bitrate:                 %.3f kbps", act_rate);
        LOG_D("");
    } else {
        /* print time and % of realtime */
        LOG_D("%.3f %.3f %d ", 1e-6 * totalTime, 1e-4 * totalTime / fileTime, totPackets);
        /* print average and active bitrates */
        LOG_D("%.3f %.3f ", avg_rate, act_rate);
    }

    return 0;
}