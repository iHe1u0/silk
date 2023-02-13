package cc.imorning.silk

import android.util.Log
import java.io.File

open class SilkJni {

    private external fun nativeGetSilkVersion(): String

    private external fun nativeEncode(input: String, output: String, sampleRate: Int): Int

    private external fun nativeDecode(input: String, output: String, sampleRate: Int): Int

    /**
     * pcm file suffix
     *
     * @since 1.0.0
     */
    val pcmSuffix = "pcm"

    /**
     * silk file suffix
     *
     * @since 1.0.0
     */
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
     * @param silkFilePath the path of silk file
     * @param audioSampleRate encode sample rate
     *
     * @since 1.0.0
     */
    protected fun encode(
        pcmFilePath: String,
        silkFilePath: String,
        audioSampleRate: AudioConfig.AudioSampleRate
    ) {
        val inputFile = File(pcmFilePath)
        if (inputFile.exists()) {
            nativeEncode(pcmFilePath, silkFilePath, audioSampleRate.rate)
        } else {
            Log.e(TAG, "encode failed,file $pcmFilePath not exist")
        }
    }

    /**
     * Decode a silk file with silk
     *
     * @param silkFilePath the path of silk file
     * @param pcmFilePath the path of pcm file
     * @param audioSampleRate decode sample rate
     *
     * @since 1.0.0
     */
    protected fun decode(
        silkFilePath: String,
        pcmFilePath: String,
        audioSampleRate: AudioConfig.AudioSampleRate
    ) {
        val inputFile = File(silkFilePath)
        if (inputFile.exists()) {
            var output = pcmFilePath
            if (pcmFilePath.isEmpty()) {
                output = File(silkFilePath.replace(silkSuffix, pcmSuffix)).absolutePath
            }
            nativeDecode(silkFilePath, output, audioSampleRate.rate)
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