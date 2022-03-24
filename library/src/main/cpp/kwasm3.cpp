#include <jni.h>
#include <string>
#include <vector>
#include "wasm3.h"
#include "m3_env.h"
#include "m3_api_libc.h"
#include "host.h"
#include "bindings.h"
#include "helpers.h"

int countParams(const std::string &signature) {
    int params = 0;
    bool parsingParams = false;
    for (char c : signature) {
        if (c == ')') { parsingParams = false; }
        if (parsingParams) { params++; }
        if (c == '(') { parsingParams = true; }
    }
    return params;
}

void linkRawFunction(
        IM3Module module,
        const std::string &hostFuncNsChars,
        const std::string &hostFuncNameChars,
        const std::string &hostFuncSigChars,
        const HostFunctionContext *context
) {
    int paramCount = countParams(hostFuncSigChars);
    switch (paramCount) {
        case 0:
            m3_LinkRawFunctionEx(
                    module,
                    hostFuncNsChars.c_str(),
                    hostFuncNameChars.c_str(),
                    hostFuncSigChars.c_str(),
                    &m3_host_function0,
                    context
            );
            break;
        case 1:
            m3_LinkRawFunctionEx(
                    module,
                    hostFuncNsChars.c_str(),
                    hostFuncNameChars.c_str(),
                    hostFuncSigChars.c_str(),
                    &m3_host_function1,
                    context
            );
            break;
        case 2:
            m3_LinkRawFunctionEx(
                    module,
                    hostFuncNsChars.c_str(),
                    hostFuncNameChars.c_str(),
                    hostFuncSigChars.c_str(),
                    &m3_host_function2,
                    context
            );
            break;
        case 3:
            m3_LinkRawFunctionEx(
                    module,
                    hostFuncNsChars.c_str(),
                    hostFuncNameChars.c_str(),
                    hostFuncSigChars.c_str(),
                    &m3_host_function3,
                    context
            );
            break;
        default:
            // Throw exception
            break;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nishtahir_kwasm3_KWasm3_executeModule(
        JNIEnv *jniEnv,
        jobject thiz,
        jstring jmodule_name,
        jbyteArray jmodule_arr,
        jint stack_size,
        jobjectArray host_functions,
        jstring jexec_functionName
) {
    M3Result result = m3Err_none;
    auto wasm = jniEnv->GetByteArrayElements(jmodule_arr, nullptr);
    auto wasm_len = jniEnv->GetArrayLength(jmodule_arr);

    IM3Environment env = m3_NewEnvironment();
    if (!env) {
        throwWasm3Exception(jniEnv, nullptr, "m3_NewEnvironment failed");
        return;
    }

    IM3Runtime runtime = m3_NewRuntime(env, stack_size, nullptr);
    if (!runtime) {
        throwWasm3Exception(jniEnv, nullptr, "m3_NewRuntime failed");
        return;
    }

    IM3Module module;
    result = m3_ParseModule(env, &module, (uint8_t *) wasm, wasm_len);
    if (result) {
        throwWasm3Exception(jniEnv, runtime, "m3_ParseModule: %s", result);
        return;
    }

    result = m3_LoadModule(runtime, module);
    if (result) {
        throwWasm3Exception(jniEnv, runtime, "m3_LoadModule: %s", result);
        return;
    }

    result = m3_LinkLibC(module);
    if (result) {
        throwWasm3Exception(jniEnv, runtime, "m3_LinkLibC: %s", result);
        return;
    }

    // Bind host functions m3RawFunctionEx passing in the reference to
    // the jobject as user data to invoke later
    auto host_functions_len = jniEnv->GetArrayLength(host_functions);
    std::vector<HostFunctionContext> hostFunctionsContexts(10);

    for (int i = 0; i < host_functions_len; i++) {
        jclass iHostFunction = jniEnv->FindClass("com/nishtahir/kwasm3/HostFunction");
        jmethodID getNsMethodId = jniEnv->GetMethodID(iHostFunction, "getNamespace",
                                                      "()Ljava/lang/String;");
        jmethodID getNameMethodId = jniEnv->GetMethodID(iHostFunction, "getName",
                                                        "()Ljava/lang/String;");
        jmethodID getSignatureMethodId = jniEnv->GetMethodID(iHostFunction, "getSignature",
                                                             "()Ljava/lang/String;");

        jobject func = jniEnv->GetObjectArrayElement(host_functions, i);

        auto hostFuncNsChars = toString(jniEnv, (jstring) jniEnv->
                CallObjectMethod(func, getNsMethodId));

        auto hostFuncNameChars = toString(jniEnv, (jstring) jniEnv->
                CallObjectMethod(func, getNameMethodId));

        auto hostFuncSigChars = toString(jniEnv, (jstring) jniEnv->
                CallObjectMethod(func, getSignatureMethodId));

        hostFunctionsContexts.push_back(HostFunctionContext{jniEnv, func, hostFuncSigChars});

        auto context = &hostFunctionsContexts[hostFunctionsContexts.size() - 1];
        linkRawFunction(module, hostFuncNsChars, hostFuncNameChars, hostFuncSigChars, context);
    }

    IM3Function f;
    const char *exec_functionName = jniEnv->GetStringUTFChars(jexec_functionName, nullptr);
    result = m3_FindFunction(&f, runtime, exec_functionName);

    if (result) {
        throwWasm3Exception(jniEnv, runtime, "m3_FindFunction: %s", result);
        return;
    }

    result = m3_CallV(f);
    if (result) throwWasm3Exception(jniEnv, runtime, "m3_Call: %s", result);

    // https://docs.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html
    // 0 - copy back the content and free the elems buffer
    jniEnv->ReleaseByteArrayElements(jmodule_arr, wasm, 0);
    jniEnv->ReleaseStringUTFChars(jexec_functionName, exec_functionName);
}

