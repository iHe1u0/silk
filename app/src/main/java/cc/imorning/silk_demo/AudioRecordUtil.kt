package cc.imorning.silk_demo

import android.annotation.SuppressLint
import android.media.AudioFormat
import android.media.AudioRecord
import android.media.MediaRecorder
import java.io.File
import java.io.FileOutputStream

class AudioRecordUtil private constructor() {

    private val sampleRateInHz = 44100
    private val channelConfig = AudioFormat.CHANNEL_IN_MONO
    private val audioFormat = AudioFormat.ENCODING_PCM_16BIT

    private var recorderState = true

    private lateinit var buffer: ByteArray
    private lateinit var audioRecord: AudioRecord
    private lateinit var fileOutputStream: FileOutputStream

    @SuppressLint("MissingPermission")
    private fun init() {
        val recordMinBufferSize =
            AudioRecord.getMinBufferSize(sampleRateInHz, channelConfig, audioFormat)
        buffer = ByteArray(recordMinBufferSize)
        audioRecord = AudioRecord(
            MediaRecorder.AudioSource.VOICE_COMMUNICATION,
            sampleRateInHz,
            channelConfig,
            audioFormat,
            recordMinBufferSize
        )
    }

    /**
     * start record audio
     */
    fun start(pcmFile: File) {
        init()
        fileOutputStream = FileOutputStream(pcmFile)
        if (audioRecord.state == AudioRecord.RECORDSTATE_STOPPED) {
            recorderState = true
            audioRecord.startRecording()
            RecordThread().start()
        }
    }

    /**
     * stop record
     */
    fun stop() {
        recorderState = false
        if (audioRecord.state == AudioRecord.RECORDSTATE_RECORDING) {
            audioRecord.stop()
            fileOutputStream.close()
        }
        audioRecord.release()
    }

    private inner class RecordThread : Thread() {
        override fun run() {
            while (recorderState) {
                val read = audioRecord.read(buffer, 0, buffer.size)
                if (AudioRecord.ERROR_INVALID_OPERATION != read) {
                    fileOutputStream.write(buffer)
                }
            }
        }
    }

    companion object {
        private const val TAG = "AudioRecordUtil"
        val instance = AudioRecordUtil()
    }
}