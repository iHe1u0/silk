package cc.imorning.silk_demo

import android.Manifest
import android.content.Intent
import android.content.pm.PackageManager
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.os.Environment
import android.provider.Settings
import android.util.Log
import android.view.View
import android.view.View.OnClickListener
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import cc.imorning.silk.AudioConfig
import cc.imorning.silk.SilkDecoder
import cc.imorning.silk.SilkEncoder
import cc.imorning.silk.SilkJni
import cc.imorning.silk_demo.databinding.ActivityMainBinding
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.MainScope
import kotlinx.coroutines.launch
import java.io.*
import java.util.*

class MainActivity : AppCompatActivity(), OnClickListener {

    companion object {
        private const val TAG = "MainActivity"
        private const val REQUEST_PERMISSION_CODE = 1000
    }

    private lateinit var binding: ActivityMainBinding

    private val slkFile = File(Environment.getExternalStorageDirectory().path, "test.silk")
    private val pcmFile = File(Environment.getExternalStorageDirectory().path, "test.pcm")

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        // init ui
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        binding.record.setOnClickListener(this)
        binding.stop.setOnClickListener(this)
        binding.encode.setOnClickListener(this)
        binding.decode.setOnClickListener(this)
        // check permission
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            checkPermission()
        }
        // silk library version
        Log.d(TAG, "Silk library version:${SilkJni.getInstance().getVersion()}")
    }

    @RequiresApi(Build.VERSION_CODES.M)
    private fun checkPermission() {
        val permissions = packageManager.getPackageInfo(
            packageName,
            PackageManager.GET_PERMISSIONS
        ).requestedPermissions
        permissions.forEach {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                if (ContextCompat.checkSelfPermission(this, it)
                    == PackageManager.PERMISSION_DENIED
                ) {
                    requestPermissions(
                        arrayOf(it),
                        REQUEST_PERMISSION_CODE
                    )
                }
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
                    val uri = Uri.parse("package:${BuildConfig.APPLICATION_ID}")
                    try{
                        startActivity(
                            Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION,uri)
                        )
                    } catch(e: Exception) {
                    }
                }
            }
        }
    }

    override fun onClick(v: View?) {
        if (v == null) {
            return
        }
        val status = binding.status
        when (v.id) {
            binding.record.id -> {
                if (ActivityCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO)
                    != PackageManager.PERMISSION_GRANTED
                ) {
                    ActivityCompat.requestPermissions(
                        this,
                        arrayOf(Manifest.permission.RECORD_AUDIO),
                        REQUEST_PERMISSION_CODE
                    )
                    return
                }
                AudioRecordUtil.instance.start(pcmFile)
                binding.record.isEnabled = false
                binding.stop.isEnabled = true
                status.text = getString(R.string.recording)
            }
            binding.stop.id -> {
                AudioRecordUtil.instance.stop()
                binding.record.isEnabled = true
                binding.stop.isEnabled = false
                binding.encode.isEnabled = true
                status.text = getString(R.string.record_file)
                    .format(pcmFile, pcmFile.length() / 1024)
            }
            binding.encode.id -> {
                status.text = getString(R.string.encoding)
                MainScope().launch(Dispatchers.IO) {
                    val outputPath = SilkEncoder.doEncode(
                        pcmFilePath = pcmFile.absolutePath,
                        silkFilePath = slkFile.absolutePath,
                        sampleRate = AudioConfig.AudioSampleRate.SAMPLE_RATE_16K
                    ).orEmpty()
                    runOnUiThread {
                        status.text =
                            getString(R.string.encode_done)
                                .format(outputPath, slkFile.length() / 1024)
                        binding.decode.isEnabled = true
                    }
                }
                Log.d(TAG, "Saved in:$slkFile")
            }
            binding.decode.id -> {
                status.text = getString(R.string.decoding)
                MainScope().launch(Dispatchers.IO) {
                    val outputPath = SilkDecoder.doDecode(
                        silkFilePath = slkFile.absolutePath,
                        pcmFilePath = pcmFile.absolutePath,
                        sampleRate = AudioConfig.AudioSampleRate.SAMPLE_RATE_16K
                    ).orEmpty()
                    runOnUiThread {
                        status.text =
                            getString(R.string.decode_done)
                                .format(outputPath, pcmFile.length() / 1024)
                    }
                }
            }
        }
    }
}
