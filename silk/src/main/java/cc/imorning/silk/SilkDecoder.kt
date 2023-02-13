package cc.imorning.silk

import java.io.File

object SilkDecoder : SilkJni() {

    /**
     * decode silk file
     *
     * @param silkFilePath the silk file path
     * @param pcmFilePath the pcm file path will output
     * @param sampleRate decode sample rate,must be [AudioConfig.AudioSampleRate]
     *
     * @return if [pcmFilePath] is not null,then will return [pcmFilePath],
     * or will replace [silkFilePath] suffix with .pcm and return it.
     * return null if [pcmFilePath] is incorrect.
     */
    fun doDecode(
        silkFilePath: String,
        pcmFilePath: String? = null,
        sampleRate: AudioConfig.AudioSampleRate = AudioConfig.AudioSampleRate.SAMPLE_RATE_16K
    ): String? {
        assert(silkFilePath.isNotEmpty())
        assert(File(silkFilePath).exists())
        val inputFilePath = File(silkFilePath).absolutePath
        var outputFilePath = pcmFilePath
        if (outputFilePath.isNullOrEmpty()) {
            outputFilePath = File(silkFilePath.replace(pcmSuffix, silkSuffix)).absolutePath
        }
        if (outputFilePath != null) {
            decode(inputFilePath, outputFilePath, sampleRate)
            return outputFilePath
        }
        return null
    }

}