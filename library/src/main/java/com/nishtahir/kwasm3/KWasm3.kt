package com.nishtahir.kwasm3

import java.io.File
import java.io.FileInputStream
import java.io.InputStream

class KWasm3 internal constructor(
    val moduleName: String,
    val module: ByteArray,
    val stackSize: Int,
    val hostFunctions: List<HostFunction>
) {

    inline fun <reified P1, reified P2, reified P3, reified R> call(
        name: String,
        p1: P1,
        p2: P2,
        p3: P3
    ): R? {
        val target = object : ExportedFunction3<P1, P2, P3, R> {
            override fun getName(): String = name
            override fun getSignature(): String =
                serializeFunctionSignature(listOf(P1::class, P2::class, P3::class), R::class)

            override fun getArgs(): Array<*> = arrayOf(p1, p2, p3)
        }
        return executeModuleWithTarget(
            moduleName,
            module,
            stackSize,
            hostFunctions.toTypedArray(),
            target
        )
    }

    inline fun <reified P1, reified P2, reified R> call(name: String, p1: P1, p2: P2): R? {
        val target = object : ExportedFunction2<P1, P2, R> {
            override fun getName(): String = name
            override fun getSignature(): String =
                serializeFunctionSignature(listOf(P1::class, P2::class), R::class)

            override fun getArgs(): Array<*> = arrayOf(p1, p2)
        }
        return executeModuleWithTarget(
            moduleName,
            module,
            stackSize,
            hostFunctions.toTypedArray(),
            target
        )
    }

    inline fun <reified P1, reified R> call(name: String, param: P1): R? {
        val target = object : ExportedFunction1<P1, R> {
            override fun getName(): String = name
            override fun getSignature(): String =
                serializeFunctionSignature(listOf(P1::class), R::class)

            override fun getArgs(): Array<*> = arrayOf(param)
        }
        return executeModuleWithTarget(
            moduleName,
            module,
            stackSize,
            hostFunctions.toTypedArray(),
            target
        )
    }

    inline fun <reified R> call(name: String): R? {
        val target = object : ExportedFunction0<R> {
            override fun getName(): String = name
            override fun getSignature(): String =
                serializeFunctionSignature(emptyList(), R::class)

            override fun getArgs(): Array<*> = emptyArray<Any>()
        }
        return executeModuleWithTarget(
            moduleName,
            module,
            stackSize,
            hostFunctions.toTypedArray(),
            target
        )
    }

    external fun <R> executeModuleWithTarget(
        moduleName: String,
        module: ByteArray,
        stackSize: Int,
        hostFunctions: Array<HostFunction>,
        target: ExportedFunction,
    ): R

    class Builder {
        var moduleName: String? = null
        var module: ByteArray? = null
        var stackSize: Int = 0
        val hostFunctions: MutableMap<Pair<String, String>, HostFunction> = mutableMapOf()

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
            this.hostFunctions[namespace to name] = object : HostFunction0<R> {
                override fun getNamespace(): String = namespace
                override fun getName(): String = name
                override fun getSignature(): String =
                    serializeFunctionSignature(emptyList(), R::class)

                override fun invoke(): R = block()
            }
        }

        inline fun <reified P1, reified R> withHostFunction(
            namespace: String,
            name: String,
            crossinline block: (P1) -> R
        ) = apply {
            this.hostFunctions[namespace to name] = object : HostFunction1<P1, R> {
                override fun getNamespace(): String = namespace
                override fun getName(): String = name
                override fun getSignature(): String =
                    serializeFunctionSignature(listOf(P1::class), R::class)

                override fun invoke(p1: P1): R = block(p1)
            }
        }

        inline fun <reified P1, reified P2, reified R> withHostFunction(
            namespace: String,
            name: String,
            crossinline block: (P1, P2) -> R
        ) = apply {
            this.hostFunctions[namespace to name] = object : HostFunction2<P1, P2, R> {
                override fun getNamespace(): String = namespace
                override fun getName(): String = name
                override fun getSignature(): String =
                    serializeFunctionSignature(listOf(P1::class, P2::class), R::class)

                override fun invoke(p1: P1, p2: P2): R = block(p1, p2)
            }
        }

        inline fun <reified P1, reified P2, reified P3, reified R> withHostFunction(
            namespace: String,
            name: String,
            crossinline block: (P1, P2, P3) -> R
        ) = apply {
            this.hostFunctions[namespace to name] = object : HostFunction3<P1, P2, P3, R> {
                override fun getNamespace(): String = namespace
                override fun getName(): String = name
                override fun getSignature(): String =
                    serializeFunctionSignature(
                        listOf(P1::class, P2::class, P3::class),
                        R::class
                    )

                override fun invoke(p1: P1, p2: P2, p3: P3): R = block(p1, p2, p3)
            }
        }

        fun build(): KWasm3 {
            require(stackSize > 0) { "'stackSize' must be greater than 0" }
            return KWasm3(moduleName!!, module!!, stackSize, hostFunctions.values.toList())
        }
    }

    companion object {
        init {
            System.loadLibrary("kwasm3")
        }

        fun builder() = Builder()
    }
}
