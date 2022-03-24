package com.nishtahir.kwasm3

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import com.nishtahir.kwasm3.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val file = assets.open("firmware.wasm")
        val kwasm3 = KWasm3.builder()
            .withBinaryModule("module", file)
            .build()
    }

}