package cc.imorning.silk

import java.io.File

object SilkEncoder : SilkJni() {

    /**
     * encode pcm file
     *
     * @param pcmFilePath the pcm file path
     * @param silkFilePath the silk file path will output
     * @param sampleRate encode sample rate,must be [AudioConfig.AudioSampleRate]
     *
     * @return if [silkFilePath] is not null,then will return [silkFilePath],
     * or will replace [pcmFilePath] suffix with .silk and return it.
     * return null if [pcmFilePath] is incorrect.
     */
    fun doEncode(
        pcmFilePath: String,
        silkFilePath: String? = null,
        sampleRate: AudioConfig.AudioSampleRate = AudioConfig.AudioSampleRate.SAMPLE_RATE_16K
    ): String? {
        assert(pcmFilePath.isNotEmpty())
        val inputFilePath = File(pcmFilePath).absolutePath
        var outputFilePath = silkFilePath
        if (outputFilePath.isNullOrEmpty()) {
            outputFilePath = File(pcmFilePath.replace(pcmSuffix, silkSuffix)).absolutePath
        }
        if (outputFilePath != null) {
            encode(inputFilePath, outputFilePath, sampleRate)
            return outputFilePath
        }
        return null
    }

}