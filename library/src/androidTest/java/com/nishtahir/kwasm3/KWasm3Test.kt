package com.nishtahir.kwasm3

import android.util.Log
import androidx.test.platform.app.InstrumentationRegistry
import org.junit.Assert.*

import org.junit.Test

class KWasm3Test {

    @Test
    fun testCallbackWithi32() {
        var i32callbackResult: Int? = null
        var i64callbackResult: Long? = null
        var f32callbackResult: Float? = null
        var f64callbackResult: Double? = null
        var function2Result: Pair<Int, Int>? = null
        var function3Result: Triple<Int, Int, Int>? = null

        val context = InstrumentationRegistry.getInstrumentation().targetContext
        val firmware = context.assets.open("callback.wasm")
        KWasm3.builder()
            .withBinaryModule("module", firmware)
            .withStackSize(8192)
            .withHostFunction<Int, Unit>("env", "i32callback") { p1 -> i32callbackResult = p1 }
            .withHostFunction<Long, Unit>("env", "i64callback") { p1 -> i64callbackResult = p1 }
            .withHostFunction<Float, Unit>("env", "f32callback") { p1 -> f32callbackResult = p1 }
            .withHostFunction<Double, Unit>("env", "f64callback") { p1 -> f64callbackResult = p1 }
            .withHostFunction<Int, Int, Unit>("env", "function2") { p1, p2 ->
                function2Result = Pair(p1, p2)
            }
            .withHostFunction<Int, Int, Int, Unit>("env", "function3") { p1, p2, p3 ->
                function3Result = Triple(p1, p2, p3)
            }
            .withHostFunction<Int, Int, Int, Int>("env", "i_iii") { p1, p2, p3 ->
                return@withHostFunction p1 + p2 + p3
            }
            .build()
            .execute("call")

        assertEquals(42, i32callbackResult)
        assertEquals(42L, i64callbackResult)
        assertEquals(42.0F, f32callbackResult)
        assertEquals(42.0, f64callbackResult)
        assertEquals(Pair(0, 1), function2Result)
        assertEquals(Triple(3, 2, 1), function3Result)
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