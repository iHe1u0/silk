package cc.imorning.silk

import android.util.Log
import java.io.File

open class SilkJni {

    private external fun nativeGetSilkVersion(): String

    private external fun nativeEncode(input: String, output: String): Int

    private external fun nativeDecode(input: String, output: String): Int

    // pcm file suffix
    val pcmSuffix = "pcm"
    // silk file suffix
    val silkSuffix = "silk"

    init {
        System.loadLibrary("silk")
    }

    /**
     * get silk version
     *
     * @return string of version
     */
    fun getVersion(): String {
        return nativeGetSilkVersion()
    }

    /**
     * Encode a pcm file with silk
     *
     * @param pcmFilePath the path of pcm file
     *
     * @param silkFilePath the path of silk file
     *
     */
    protected fun encode(pcmFilePath: String, silkFilePath: String) {
        val inputFile = File(pcmFilePath)
        if (inputFile.exists()) {
            nativeEncode(pcmFilePath, silkFilePath)
        } else {
            Log.e(TAG, "encode failed,file $pcmFilePath not exist")
        }
    }

    /**
     * Decode a silk file with silk
     *
     * @param silkFilePath the path of silk file
     *
     * @param pcmFilePath the path of pcm file
     *
     */
    protected fun decode(silkFilePath: String, pcmFilePath: String) {
        val inputFile = File(silkFilePath)
        if (inputFile.exists()) {
            var output = pcmFilePath
            if (pcmFilePath.isEmpty()) {
                output = File(silkFilePath.replace(silkSuffix, pcmSuffix)).absolutePath
            }
            nativeDecode(silkFilePath, output)
        }
    }

    private object InnerClass {
        val silkJni = SilkJni()
    }

    companion object {
        private const val TAG = "SilkJni"

        fun getInstance(): SilkJni {
            return InnerClass.silkJni
        }
    }
}