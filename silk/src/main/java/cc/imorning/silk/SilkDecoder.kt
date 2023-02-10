package cc.imorning.silk

import java.io.File

object SilkDecoder : SilkJni() {

    /**
     * decode silk file
     *
     * @param silkFilePath the silk file path
     *
     * @param pcmFilePath the pcm file path will output
     *
     * @return if [pcmFilePath] is not null,then will return [pcmFilePath],
     * or will replace [silkFilePath] suffix with .pcm and return it.
     * return null if [pcmFilePath] is incorrect.
     */
    fun doDecode(silkFilePath: String, pcmFilePath: String? = null): String? {
        assert(silkFilePath.isNotEmpty())
        assert(File(silkFilePath).exists())
        val inputFilePath = File(silkFilePath).absolutePath
        var outputFilePath = pcmFilePath
        if (outputFilePath.isNullOrEmpty()) {
            outputFilePath = File(silkFilePath.replace(pcmSuffix, silkSuffix)).absolutePath
        }
        if (outputFilePath != null) {
            decode(inputFilePath, outputFilePath)
            return outputFilePath
        }
        return null
    }

}