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
            // TODO - Throw exception
            break;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_nishtahir_kwasm3_KWasm3_executeModuleWithTarget(
        JNIEnv *jniEnv, jobject thiz,
        jstring module_name,
        jbyteArray jmodule,
        jint stack_size,
        jobjectArray host_functions,
        jobject target
) {
    M3Result result = m3Err_none;
    auto wasm = jniEnv->GetByteArrayElements(jmodule, nullptr);
    auto wasm_len = jniEnv->GetArrayLength(jmodule);

    IM3Environment env = m3_NewEnvironment();
    if (!env) {
        throwWasm3Exception(jniEnv, nullptr, "m3_NewEnvironment failed");
        return nullptr;
    }

    IM3Runtime runtime = m3_NewRuntime(env, stack_size, nullptr);
    if (!runtime) {
        throwWasm3Exception(jniEnv, nullptr, "m3_NewRuntime failed");
        return nullptr;
    }

    IM3Module module;
    result = m3_ParseModule(env, &module, (uint8_t *) wasm, wasm_len);
    if (result) {
        throwWasm3Exception(jniEnv, runtime, "m3_ParseModule: %s", result);
        return nullptr;
    }

    result = m3_LoadModule(runtime, module);
    if (result) {
        throwWasm3Exception(jniEnv, runtime, "m3_LoadModule: %s", result);
        return nullptr;
    }

    result = m3_LinkLibC(module);
    if (result) {
        throwWasm3Exception(jniEnv, runtime, "m3_LinkLibC: %s", result);
        return nullptr;
    }

    // Bind host functions m3RawFunctionEx passing in the reference to
    // the jobject as user data to invoke later
    auto host_functions_len = jniEnv->GetArrayLength(host_functions);
    std::vector<HostFunctionContext> hostFunctionsContexts;

    for (int i = 0; i < host_functions_len; i++) {
        jclass iHostFunction = jniEnv->FindClass("com/nishtahir/kwasm3/HostFunction");
        jmethodID getNsMethodId = jniEnv->GetMethodID(iHostFunction, "getNamespace",
                                                      "()Ljava/lang/String;");
        jmethodID getHostNameMethodId = jniEnv->GetMethodID(iHostFunction, "getName",
                                                            "()Ljava/lang/String;");
        jmethodID getHostSignatureMethodId = jniEnv->GetMethodID(iHostFunction, "getSignature",
                                                                 "()Ljava/lang/String;");

        jobject func = jniEnv->GetObjectArrayElement(host_functions, i);

        auto hostFuncNsChars = toString(jniEnv, (jstring) jniEnv->
                CallObjectMethod(func, getNsMethodId));

        auto hostFuncNameChars = toString(jniEnv, (jstring) jniEnv->
                CallObjectMethod(func, getHostNameMethodId));

        auto hostFuncSigChars = toString(jniEnv, (jstring) jniEnv->
                CallObjectMethod(func, getHostSignatureMethodId));

        hostFunctionsContexts.push_back(HostFunctionContext{jniEnv, func, hostFuncSigChars});

        auto context = &hostFunctionsContexts[hostFunctionsContexts.size() - 1];
        linkRawFunction(module, hostFuncNsChars, hostFuncNameChars, hostFuncSigChars, context);
    }


    jclass iExportedFunctionClazz = jniEnv->FindClass("com/nishtahir/kwasm3/ExportedFunction");
    jmethodID getNameMethodId = jniEnv->GetMethodID(
            iExportedFunctionClazz, "getName", "()Ljava/lang/String;");
    jmethodID getSignatureMethodId = jniEnv->GetMethodID(
            iExportedFunctionClazz, "getSignature", "()Ljava/lang/String;");
    jmethodID getParamsMethodId = jniEnv->GetMethodID(
            iExportedFunctionClazz, "getArgs", "()[Ljava/lang/Object;");

    auto exportedFunctionName = toString(jniEnv, (jstring) jniEnv->
            CallObjectMethod(target, getNameMethodId));

    auto exportedFunctionSig = toString(jniEnv, (jstring) jniEnv->
            CallObjectMethod(target, getSignatureMethodId));

    auto exportedFunctionArgs = (jobjectArray) jniEnv->CallObjectMethod(target, getParamsMethodId);

    IM3Function f;
    result = m3_FindFunction(&f, runtime, exportedFunctionName.c_str());

    if (result) {
        throwWasm3Exception(jniEnv, runtime, "m3_FindFunction: %s", result);
        return nullptr;
    }

    auto argsCount = countParams(exportedFunctionSig);
    std::vector<void *> mappedArgs;

    for (int i = 0; i < argsCount; i++) {
        auto type = exportedFunctionSig.at(i + 2);
        auto arg = jniEnv->GetObjectArrayElement(exportedFunctionArgs, i);
        switch (type) {
            case 'i': {
                jclass iClazz = jniEnv->FindClass("java/lang/Integer");
                jmethodID methodID = jniEnv->GetMethodID(iClazz, "intValue", "()I");
                int32_t val = jniEnv->CallIntMethod(arg, methodID);
                mappedArgs.push_back((void *const) val);
                break;
            }
            case 'I': {
                jclass iClazz = jniEnv->FindClass("java/lang/Long");
                jmethodID methodID = jniEnv->GetMethodID(iClazz, "longValue", "()J");
                int64_t val = jniEnv->CallLongMethod(arg, methodID);
                mappedArgs.push_back((void *const) val);
                break;
            }
            case 'f': {
                jclass iClazz = jniEnv->FindClass("java/lang/Float");
                jmethodID methodID = jniEnv->GetMethodID(iClazz, "floatValue", "()F");
                float val = jniEnv->CallFloatMethod(arg, methodID);
                // Looks like you can't cast floats and doubles to void * without having some sort of
                // conversion scheme associated with this.
                // TODO - Needs a decent solution to pass this without loss
                // mappedArgs.push_back((void *const) val);
                break;
            }
            case 'F': {
                jclass iClazz = jniEnv->FindClass("java/lang/Double");
                jmethodID methodID = jniEnv->GetMethodID(iClazz, "doubleValue", "()D");
                double val = jniEnv->CallDoubleMethod(arg, methodID);
                // mappedArgs.push_back((void *const) val);
                break;
            }
            default: {
                // No-op
            }
        }
    }

    switch (argsCount) {
        case 1:
            result = m3_CallV(f, mappedArgs[0]);
            break;
        case 2:
            result = m3_CallV(f, mappedArgs[0], mappedArgs[1]);
            break;
        case 3:
            result = m3_CallV(f, mappedArgs[0], mappedArgs[1], mappedArgs[2]);
            break;
        default: {
            // TODO - throw exception
        }
    }
    if (result) {
        throwWasm3Exception(jniEnv, runtime, "m3_CalLV: %s", result);
        return nullptr;
    }

    // Extract and parse result
    auto rType = exportedFunctionSig.at(0);
    jobject rValue;
    switch (rType) {
        case 'i': {
            int value = 0;
            result = m3_GetResultsV(f, &value);
            rValue = boxInteger(jniEnv, value);
            break;
        }
        case 'I': {
            int64_t value = 0;
            result = m3_GetResultsV(f, &value);
            rValue = boxLong(jniEnv, value);
            break;
        }
        case 'f': {
            float value = 0;
            result = m3_GetResultsV(f, &value);
            rValue = boxFloat(jniEnv, value);
            break;
        }
        case 'F': {
            double value = 0;
            result = m3_GetResultsV(f, &value);
            rValue = boxDouble(jniEnv, value);
            break;
        }
        default: {
            // TODO - throw exception
        }
    }

    if (result) {
        throwWasm3Exception(jniEnv, runtime, "GetResults: %s", result);
        return nullptr;
    }

    // https://docs.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html
    // 0 - copy back the content and free the elems buffer
    jniEnv->ReleaseByteArrayElements(jmodule, wasm, 0);

    return rValue;
}