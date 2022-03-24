package com.nishtahir.kwasm3

import java.io.File
import java.io.FileInputStream
import java.io.InputStream

class KWasm3 internal constructor(
    private val moduleName: String,
    private val module: ByteArray,
    private val stackSize: Int,
    private val hostFunctions: List<HostFunction>
) {

    fun execute(function: String) {
        return executeModule(
            moduleName,
            module,
            stackSize,
            hostFunctions.toTypedArray(),
            function
        )
    }

    private external fun executeModule(
        moduleName: String,
        module: ByteArray,
        stackSize: Int,
        hostFunctions: Array<HostFunction>,
        function: String,
    )

    class Builder {
        var moduleName: String? = null
        var module: ByteArray? = null
        var stackSize: Int = 0
        val hostFunctions: MutableList<HostFunction> = mutableListOf()

        fun withBinaryModule(moduleName: String, binaryStream: InputStream) = apply {
            this.moduleName = moduleName
            this.module = binaryStream.readBytes()
            return this
        }

        fun withBinaryModule(moduleName: String, file: File) = apply {
            withBinaryModule(moduleName, FileInputStream(file))
        }

        fun withStackSize(stackSize: Int) = apply {
            this.stackSize = stackSize
        }

        inline fun <reified R> withHostFunction(
            namespace: String,
            name: String,
            crossinline block: () -> R
        ) = apply {
            this.hostFunctions.add(
                object : HostFunction0<R> {
                    override fun getNamespace(): String = namespace
                    override fun getName(): String = name
                    override fun getSignature(): String =
                        serializeFunctionSignature(emptyList(), R::class)

                    override fun invoke(): R = block()
                }
            )
        }

        inline fun <reified P1, reified R> withHostFunction(
            namespace: String,
            name: String,
            crossinline block: (P1) -> R
        ) = apply {
            this.hostFunctions.add(
                object : HostFunction1<P1, R> {
                    override fun getNamespace(): String = namespace
                    override fun getName(): String = name
                    override fun getSignature(): String =
                        serializeFunctionSignature(listOf(P1::class), R::class)

                    override fun invoke(p1: P1): R = block(p1)
                }
            )
        }

        inline fun <reified P1, reified P2, reified R> withHostFunction(
            namespace: String,
            name: String,
            crossinline block: (P1, P2) -> R
        ) = apply {
            this.hostFunctions.add(
                object : HostFunction2<P1, P2, R> {
                    override fun getNamespace(): String = namespace
                    override fun getName(): String = name
                    override fun getSignature(): String =
                        serializeFunctionSignature(listOf(P1::class, P2::class), R::class)

                    override fun invoke(p1: P1, p2: P2): R = block(p1, p2)
                }
            )
        }

        inline fun <reified P1, reified P2, reified P3, reified R> withHostFunction(
            namespace: String,
            name: String,
            crossinline block: (P1, P2, P3) -> R
        ) = apply {
            this.hostFunctions.add(
                object : HostFunction3<P1, P2, P3, R> {
                    override fun getNamespace(): String = namespace
                    override fun getName(): String = name
                    override fun getSignature(): String =
                        serializeFunctionSignature(
                            listOf(P1::class, P2::class, P3::class),
                            R::class
                        )

                    override fun invoke(p1: P1, p2: P2, p3: P3): R = block(p1, p2, p3)
                }
            )
        }

        fun build(): KWasm3 {
            require(stackSize > 0) { "'stackSize' must be greater than 0" }
            return KWasm3(moduleName!!, module!!, stackSize, hostFunctions)
        }
    }

    companion object {
        init {
            System.loadLibrary("kwasm3")
        }

        fun builder() = Builder()
    }
}
