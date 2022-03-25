package com.nishtahir.kwasm3

interface ExportedFunction {
    fun getName(): String
    fun getSignature(): String
    fun getArgs(): Array<*>
}

interface ExportedFunction0<R> : ExportedFunction

interface ExportedFunction1<P1, R> : ExportedFunction

interface ExportedFunction2<P1, P2, R> : ExportedFunction

interface ExportedFunction3<P1, P2, P3, R> : ExportedFunction
