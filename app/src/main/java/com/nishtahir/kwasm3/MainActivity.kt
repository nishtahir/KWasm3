package com.nishtahir.kwasm3

import android.graphics.Color
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.Button
import android.widget.ImageView
import android.widget.TextView
import com.nishtahir.kwasm3.databinding.ActivityMainBinding
import kotlinx.coroutines.*
import java.io.InputStream

class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding
    lateinit var scope: CoroutineScope
    lateinit var button: Button
    lateinit var lightBulb: ImageView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        scope = MainScope()
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        button = findViewById(R.id.button)
        lightBulb = findViewById(R.id.imageView)


        val file = assets.open("firmware.wasm")

        button.setOnClickListener {
            startWasmModule(file)
        }
    }

    private fun startWasmModule(file: InputStream) {
        scope.launch {
            withContext(Dispatchers.IO) {
                KWasm3.builder()
                    .withStackSize(8196)
                    .withBinaryModule("module", file)
                    .withHostFunction("env", "pinMode", ::pinMode)
                    .withHostFunction("env", "digitalWrite", ::digitalWrite)
                    .withHostFunction("env", "delay", ::delay)
                    .build()
                    .execute("_start")
            }
        }
    }

    private fun pinMode(pin: Int, mode: Int) {
        // Maybe wire up light bulb here?
    }

    private fun digitalWrite(pin: Int, state: Int) {
        runOnUiThread {
            if (state == 0) {
                lightBulb.setColorFilter(Color.parseColor("#ff0000"));
            } else {
                lightBulb.setColorFilter(Color.parseColor("#000000"));
            }
        }
    }

    private fun delay(value: Int) {
        Thread.sleep(value.toLong())
    }
}