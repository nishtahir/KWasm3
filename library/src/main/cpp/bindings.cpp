#include "bindings.h"

void *mapJvmParam(const uint64_t *_sp, JNIEnv *jniEnv, char tok) {
    void *param;
    switch (tok) {
        case 'i': {
            int32_t i = *((int32_t *) _sp);
            param = boxInteger(jniEnv, (jint) i);
            break;
        }
        case 'I': {
            int64_t I = *((int64_t *) _sp);
            param = boxLong(jniEnv, (jlong) I);
            break;
        }
        case 'f': {
            float I = *((float *) _sp);
            param = boxFloat(jniEnv, (jfloat) I);
            break;
        }
        case 'F': {
            double I = *((double *) _sp);
            param = boxDouble(jniEnv, (jdouble) I);
            break;
        }
        default: {
            param = nullptr;
        }
    }
    return param;
}


const char *
mapJvmReturnValue(JNIEnv *jniEnv, char rType, const void *raw_return, jobject jvmRVal) {
    switch (rType) {
        case 'i': {
            jclass iClazz = jniEnv->FindClass("java/lang/Integer");
            jmethodID methodID = jniEnv->GetMethodID(iClazz, "intValue", "()I");
            int32_t val = jniEnv->CallIntMethod(jvmRVal, methodID);
            *(int32_t *) raw_return = val;
            return m3Err_none;
        }
        case 'I': {
            jclass iClazz = jniEnv->FindClass("java/lang/Long");
            jmethodID methodID = jniEnv->GetMethodID(iClazz, "longValue", "()J");
            int64_t val = jniEnv->CallLongMethod(jvmRVal, methodID);
            *(int64_t *) raw_return = val;
            return m3Err_none;
        }
            // TODO add float and double
        default: {
            return m3Err_none;
        }
    }
}

void allocate_raw_return(char rType, uint64_t *&_sp, void *&raw_return) {
    switch (rType) {
        case 'i': {
            raw_return = ((int32_t *) (_sp++));
            break;
        }
        case 'I': {
            raw_return = ((int64_t *) (_sp++));
            break;
        }
        default: {
            // No-op
            // raw_return remains a null pointer do not increment
            // _sp because this is a void function
            break;
        }
    }
}


const void *
m3_host_function0(IM3Runtime _runtime, IM3ImportContext _ctx, uint64_t *_sp, void *_mem) {
    auto *obj = (HostFunctionContext *) _ctx->userdata;
    JNIEnv *jniEnv = obj->env;

    auto clazz = jniEnv->FindClass("com/nishtahir/kwasm3/HostFunction0");
    auto invokeId = jniEnv->GetMethodID(clazz, "invoke",
                                        "()Ljava/lang/Object;");

    auto rType = obj->signature.at(0);
    void *raw_return;
    allocate_raw_return(rType, _sp, raw_return);

    auto jvmRVal = jniEnv->CallObjectMethod(obj->context, invokeId);
    return mapJvmReturnValue(jniEnv, rType, raw_return, jvmRVal);
}

const void *
m3_host_function1(IM3Runtime _runtime, IM3ImportContext ctx, uint64_t *_sp, void *_mem) {
    auto *obj = (HostFunctionContext *) ctx->userdata;
    JNIEnv *jniEnv = obj->env;

    auto clazz = jniEnv->FindClass("com/nishtahir/kwasm3/HostFunction1");
    auto invokeId = jniEnv->GetMethodID(clazz, "invoke",
                                        "(Ljava/lang/Object;)Ljava/lang/Object;");

    auto rType = obj->signature.at(0);
    void *raw_return;
    allocate_raw_return(rType, _sp, raw_return);

    void *p1;
    p1 = mapJvmParam(_sp++, jniEnv, obj->signature.at(2));

    auto jvmRVal = jniEnv->CallObjectMethod(obj->context, invokeId, p1);
    return mapJvmReturnValue(jniEnv, rType, raw_return, jvmRVal);
}

const void *
m3_host_function2(IM3Runtime runtime, IM3ImportContext ctx, uint64_t *_sp, void *_mem) {
    auto *obj = (HostFunctionContext *) ctx->userdata;
    JNIEnv *jniEnv = obj->env;

    auto clazz = jniEnv->FindClass("com/nishtahir/kwasm3/HostFunction2");
    auto invokeId = jniEnv->GetMethodID(clazz, "invoke",
                                        "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    auto rType = obj->signature.at(0);
    void *raw_return;
    allocate_raw_return(rType, _sp, raw_return);

    void *p1;
    void *p2;
    p1 = mapJvmParam(_sp++, jniEnv, obj->signature.at(2));
    p2 = mapJvmParam(_sp++, jniEnv, obj->signature.at(3));

    auto jvmRVal = jniEnv->CallObjectMethod(obj->context, invokeId, p1, p2);
    return mapJvmReturnValue(jniEnv, rType, raw_return, jvmRVal);
}

const void *
m3_host_function3(IM3Runtime runtime, IM3ImportContext ctx, uint64_t *_sp, void *_mem) {
    auto *obj = (HostFunctionContext *) ctx->userdata;
    JNIEnv *jniEnv = obj->env;

    auto clazz = jniEnv->FindClass("com/nishtahir/kwasm3/HostFunction3");
    auto invokeId = jniEnv->GetMethodID(clazz, "invoke",
                                        "(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    auto rType = obj->signature.at(0);
    void *raw_return;
    allocate_raw_return(rType, _sp, raw_return);

    void *p1;
    void *p2;
    void *p3;
    p1 = mapJvmParam(_sp++, jniEnv, obj->signature.at(2));
    p2 = mapJvmParam(_sp++, jniEnv, obj->signature.at(3));
    p3 = mapJvmParam(_sp++, jniEnv, obj->signature.at(4));

    auto jvmRVal = jniEnv->CallObjectMethod(obj->context, invokeId, p1, p2, p3);
    return mapJvmReturnValue(jniEnv, rType, raw_return, jvmRVal);
}