#include "helpers.h"

jobject boxInteger(JNIEnv *env, jint value) {
    jclass clazz = env->FindClass("java/lang/Integer");
    jmethodID methodID = env->GetMethodID(clazz, "<init>", "(I)V");
    return env->NewObject(clazz, methodID, value);
}

jobject boxLong(JNIEnv *env, jlong value) {
    jclass clazz = env->FindClass("java/lang/Long");
    jmethodID methodID = env->GetMethodID(clazz, "<init>", "(J)V");
    return env->NewObject(clazz, methodID, value);
}

jobject boxFloat(JNIEnv *env, jfloat value) {
    jclass clazz = env->FindClass("java/lang/Float");
    jmethodID methodID = env->GetMethodID(clazz, "<init>", "(F)V");
    return env->NewObject(clazz, methodID, value);
}

jobject boxDouble(JNIEnv *env, jdouble value) {
    jclass clazz = env->FindClass("java/lang/Double");
    jmethodID methodID = env->GetMethodID(clazz, "<init>", "(D)V");
    return env->NewObject(clazz, methodID, value);
}

std::string toString(JNIEnv *env, jstring jStr) {
    if (!jStr)
        return "";

    jclass clazz = env->GetObjectClass(jStr);
    jmethodID getBytes = env->GetMethodID(clazz, "getBytes", "(Ljava/lang/String;)[B");
    auto bytes = (jbyteArray) env->CallObjectMethod(jStr, getBytes, env->NewStringUTF("UTF-8"));

    auto length = (size_t) env->GetArrayLength(bytes);
    jbyte *pBytes = env->GetByteArrayElements(bytes, nullptr);

    std::string ret = std::string((char *) pBytes, length);
    env->ReleaseByteArrayElements(bytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(bytes);
    env->DeleteLocalRef(clazz);
    return ret;
}

jint throwWasm3Exception(JNIEnv *env, IM3Runtime runtime, const char *format, ...) {
    va_list arg;
    va_start (arg, format);
    char *message;
    vasprintf(&message, format, arg);
    va_end (arg);

    if (runtime) {
        // Extract runtime error and append it
        // to the formatted string
        M3ErrorInfo info;
        m3_GetErrorInfo(runtime, &info);
        sprintf(message, "%s: %s at %s:%d", message, info.message, info.file, info.line);
    }

    jclass exClass = env->FindClass("com/nishtahir/kwasm3/Wasm3Exception");
    return env->ThrowNew(exClass, message);
}
