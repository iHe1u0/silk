package cc.imorning.silk

import java.io.File

object SilkEncoder : SilkJni() {

    /**
     * encode pcm file
     *
     * @param pcmFilePath the pcm file path
     *
     * @param silkFilePath the silk file path will output
     *
     * @return if [silkFilePath] is not null,then will return [silkFilePath],
     * or will replace [pcmFilePath] suffix with .silk and return it.
     * return null if [pcmFilePath] is incorrect.
     */
    fun doEncode(pcmFilePath: String, silkFilePath: String? = null): String? {
        assert(pcmFilePath.isNotEmpty())
        val inputFilePath = File(pcmFilePath).absolutePath
        var outputFilePath = silkFilePath
        if (outputFilePath.isNullOrEmpty()) {
            outputFilePath = File(pcmFilePath.replace(pcmSuffix, silkSuffix)).absolutePath
        }
        if (outputFilePath != null) {
            encode(inputFilePath, outputFilePath)
            return outputFilePath
        }
        return null
    }

}