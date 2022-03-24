package com.nishtahir.kwasm3

import java.lang.IllegalArgumentException
import kotlin.reflect.KClass

interface HostFunction {
    fun getNamespace(): String
    fun getName(): String
    fun getSignature(): String
}

interface HostFunction0<R> : HostFunction {
    operator fun invoke(): R
}

interface HostFunction1<P1, R> : HostFunction {
    operator fun invoke(p1: P1): R
}

interface HostFunction2<P1, P2, R> : HostFunction {
    operator fun invoke(p1: P1, p2: P2): R
}

interface HostFunction3<P1, P2, P3, R> : HostFunction {
    operator fun invoke(p1: P1, p2: P2, p3: P3): R
}

fun mapKClassToSignatureToken(clazz: KClass<*>): String {
    return when (clazz.simpleName) {
        "Int" -> "i"
        "Float" -> "f"
        "Long" -> "I"
        "Double" -> "F"
        "Unit" -> "v"
        else -> throw IllegalArgumentException("Invalid class")
    }
}

fun serializeFunctionSignature(params: List<KClass<*>>, returns: KClass<*>): String {
    val serializedParams =
        params.joinToString(separator = "", transform = ::mapKClassToSignatureToken)
    val serializedReturn = mapKClassToSignatureToken(returns);
    return "${serializedReturn}(${serializedParams})"
}