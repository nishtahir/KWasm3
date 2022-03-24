#include "bindings.h"

void *mapJvmParam(const uint64_t *_sp, JNIEnv *jniEnv, char tok) {
    void *p1;
    switch (tok) {
        case 'i': {
            int32_t i = *((int32_t *) _sp);
            p1 = boxInteger(jniEnv, (jint) i);
            break;
        }
        case 'I': {
            int64_t I = *((int64_t *) _sp);
            p1 = boxLong(jniEnv, (jlong) I);
            break;
        }
        case 'f': {
            float I = *((float *) _sp);
            p1 = boxFloat(jniEnv, (jfloat) I);
            break;
        }
        case 'F': {
            double I = *((double *) _sp);
            p1 = boxDouble(jniEnv, (jdouble) I);
            break;
        }
        default: {
            p1 = nullptr;
        }
    }
    return p1;
}

const void *
m3_host_function0(IM3Runtime _runtime, IM3ImportContext _ctx, uint64_t *_sp, void *_mem) {
    auto *obj = (HostFunctionContext *) _ctx->userdata;
    JNIEnv *jniEnv = obj->env;

    auto clazz = jniEnv->FindClass("com/nishtahir/kwasm3/HostFunction0");
    auto invokeId = jniEnv->GetMethodID(clazz, "invoke",
                                        "()Ljava/lang/Object;");

    jniEnv->CallObjectMethod(obj->context, invokeId);
    m3ApiSuccess()
}

const void *
m3_host_function1(IM3Runtime _runtime, IM3ImportContext ctx, uint64_t *_sp, void *_mem) {
    auto *obj = (HostFunctionContext *) ctx->userdata;
    JNIEnv *jniEnv = obj->env;

    auto clazz = jniEnv->FindClass("com/nishtahir/kwasm3/HostFunction1");
    auto invokeId = jniEnv->GetMethodID(clazz, "invoke",
                                        "(Ljava/lang/Object;)Ljava/lang/Object;");

    void *p1;
    p1 = mapJvmParam(_sp++, jniEnv, obj->signature.at(2));

    jniEnv->CallObjectMethod(obj->context, invokeId, p1);
    m3ApiSuccess()
}

const void *
m3_host_function2(IM3Runtime runtime, IM3ImportContext ctx, uint64_t *_sp, void *_mem) {
    auto *obj = (HostFunctionContext *) ctx->userdata;
    JNIEnv *jniEnv = obj->env;

    auto clazz = jniEnv->FindClass("com/nishtahir/kwasm3/HostFunction2");
    auto invokeId = jniEnv->GetMethodID(clazz, "invoke",
                                        "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    void *p1;
    void *p2;
    p1 = mapJvmParam(_sp++, jniEnv, obj->signature.at(2));
    p2 = mapJvmParam(_sp++, jniEnv, obj->signature.at(3));

    jniEnv->CallObjectMethod(obj->context, invokeId, p1, p2);
    m3ApiSuccess()
}

const void *
m3_host_function3(IM3Runtime runtime, IM3ImportContext ctx, uint64_t *_sp, void *_mem) {
    auto *obj = (HostFunctionContext *) ctx->userdata;
    JNIEnv *jniEnv = obj->env;

    auto clazz = jniEnv->FindClass("com/nishtahir/kwasm3/HostFunction3");
    auto invokeId = jniEnv->GetMethodID(clazz, "invoke",
                                        "(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    void *p1;
    void *p2;
    void *p3;
    p1 = mapJvmParam(_sp++, jniEnv, obj->signature.at(2));
    p2 = mapJvmParam(_sp++, jniEnv, obj->signature.at(3));
    p3 = mapJvmParam(_sp++, jniEnv, obj->signature.at(4));

    auto jvmRVal = jniEnv->CallObjectMethod(obj->context, invokeId, p1, p2, p3);
    // i(iii)
    auto rType = obj->signature.at(0);

    void *rVal;
    switch (rType) {
        case 'i': {
            rVal = (int32_t *) jvmRVal;
        }
    }

    // Push the return value to the WASM stack
    m3ApiSuccess()
}
