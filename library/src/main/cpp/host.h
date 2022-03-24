#ifndef KWASM3_HOST_H
#define KWASM3_HOST_H

struct HostFunctionContext {
    JNIEnv *env;
    jobject context;
    std::string signature;
};

#endif //KWASM3_HOST_H
