package com.nishtahir.kwasm3

import android.util.Log
import androidx.test.platform.app.InstrumentationRegistry
import org.junit.Assert.*
import org.junit.Before

import org.junit.Test

class KWasm3Test {

    private val context = InstrumentationRegistry.getInstrumentation().targetContext
    private val firmware = context.assets.open("callback.wasm")

    private lateinit var kwasm3: KWasm3.Builder

    @Before
    fun setup() {
        kwasm3 = KWasm3.builder()
            .withBinaryModule("module", firmware)
            .withStackSize(8192)
            .withHostFunction<Int, Int>("env", "i_i"){p1 -> p1}
            .withHostFunction<Int>("env", "i_"){0}
            .withHostFunction<Int, Unit>("env", "v_i") {}
            .withHostFunction<Long, Long>("env", "I_I"){p1 -> p1}
            .withHostFunction<Long>("env", "I_"){0L}
            .withHostFunction<Long, Unit>("env", "v_I"){}
            .withHostFunction<Float, Unit>("env", "v_f"){}
            .withHostFunction<Double, Unit>("env", "v_F"){}
            .withHostFunction<Int, Int, Unit>("env", "v_ii"){_,_->}
            .withHostFunction<Int, Int, Int, Unit>("env", "i_iii"){_,_,_->}
    }

    @Test
    fun testi32Bindings() {
        var result: Int? = null

        kwasm3
            .withHostFunction<Int, Unit>("env", "v_i") { p1 -> result = p1 }
            .withHostFunction<Int, Int>("env", "i_i"){p1 -> p1 * p1}
            .build()
            .execute("call")

        assertEquals(1764, result)
    }

    @Test
    fun execute() {
        val context = InstrumentationRegistry.getInstrumentation().targetContext
        val firmware = context.assets.open("firmware.wasm")
        KWasm3.builder()
            .withBinaryModule("module", firmware)
            .withStackSize(8192)
            .withHostFunction<Int, Int, Unit>(
                namespace = "env",
                name = "pinMode"
            ) { p1, p2 ->
                Log.d("Hello", "pinMode")
            }
            .withHostFunction<Int, Int, Unit>(
                namespace = "env",
                name = "digitalWrite"
            ) { p1: Int, p2: Int ->
                Log.d("Hello", "digitalWrite")
            }
            .withHostFunction<Int, Unit>(
                namespace = "env",
                name = "delay"
            ) { p1: Int ->
                Log.d("Hello", "delay")
            }
            .build()
            .execute("_start")
    }
}