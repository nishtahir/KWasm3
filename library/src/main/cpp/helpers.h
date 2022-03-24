#ifndef KWASM3_HELPERS_H
#define KWASM3_HELPERS_H

#include <jni.h>
#include <string>
#include "m3_env.h"

jobject boxInteger(JNIEnv *env, jint value);

jobject boxLong(JNIEnv *env, jlong value);

jobject boxFloat(JNIEnv *env, jfloat value);

jobject boxDouble(JNIEnv *env, jdouble value);

std::string toString(JNIEnv *env, jstring jStr);

/**
 * Generate a new Wasm3Exception to be thrown on the JVM side
 * @param env JNIEnvironment to throw the exception
 * @param runtime Optional Wasm3 runtime to extract error information
 * @param format printf formatted string to generate the message
 * @param ... arguments to supply the formatted string
 * @return
 */
jint throwWasm3Exception(JNIEnv *env, IM3Runtime runtime, const char *format, ...);

#endif //KWASM3_HELPERS_H
