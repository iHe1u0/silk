package cc.imorning.silk

import android.os.Build
import androidx.annotation.RequiresApi

/**
 * Class for common AudioRecord configure
 */
object AudioConfig {

    /**
     * Supported audio sample rate
     */
    enum class AudioSampleRate(val rate: Int) {
        SAMPLE_RATE_8K(8000),
        SAMPLE_RATE_12K(12000),
        SAMPLE_RATE_16K(16000),
        SAMPLE_RATE_24K(24000),
        SAMPLE_RATE_32K(32000),
        SAMPLE_RATE_44K(44100),
        SAMPLE_RATE_48K(48000)
    }

    /**
     * Supported audio max sample rate
     */
    enum class AudioMaxSampleRate(val rate: Int) {
        SAMPLE_RATE_8K(8000),
        SAMPLE_RATE_12K(12000),
        SAMPLE_RATE_16K(16000),
        SAMPLE_RATE_24K(24000)
    }

    /**
     * Supported audio format
     */
    object AudioFormat {

        /**
         * @see android.media.AudioFormat.ENCODING_PCM_FLOAT
         */
        const val PCM_FLOAT = android.media.AudioFormat.ENCODING_PCM_FLOAT

        /**
         * @see android.media.AudioFormat.ENCODING_PCM_8BIT
         */
        const val PCM_8 = android.media.AudioFormat.ENCODING_PCM_8BIT

        /**
         * @see android.media.AudioFormat.ENCODING_PCM_16BIT
         */
        const val PCM_16 = android.media.AudioFormat.ENCODING_PCM_16BIT

        /**
         * @see android.media.AudioFormat.ENCODING_PCM_24BIT_PACKED
         */
        @RequiresApi(Build.VERSION_CODES.S)
        const val PCM_24 = android.media.AudioFormat.ENCODING_PCM_24BIT_PACKED

        /**
         * @see android.media.AudioFormat.ENCODING_PCM_32BIT
         */
        @RequiresApi(Build.VERSION_CODES.S)
        const val PCM_32 = android.media.AudioFormat.ENCODING_PCM_32BIT
    }

    /**
     * Supported channel config
     */
    object AudioChannelConfig {

        /**
         * @see android.media.AudioFormat.CHANNEL_IN_STEREO
         */
        const val CHANNEL_IN_STEREO = android.media.AudioFormat.CHANNEL_IN_STEREO

        /**
         * @see android.media.AudioFormat.CHANNEL_IN_MONO
         */
        const val CHANNEL_IN_MONO = android.media.AudioFormat.CHANNEL_IN_MONO
    }
}