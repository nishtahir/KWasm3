package com.nishtahir.kwasm3

import org.junit.Assert.*

import org.junit.Test

class HostFunctionTest {

    @Test
    fun testSerializeFunctionSignature() {
        val actual = serializeFunctionSignature(listOf(Int::class, Int::class), Unit::class)
        assertEquals("v(ii)", actual)
    }

    @Test
    fun testSerializeVoidFunctionSignature() {
        val actual = serializeFunctionSignature(listOf(), Unit::class)
        assertEquals("v()", actual)
    }
}