#ifndef KWASM3_BINDINGS_H
#define KWASM3_BINDINGS_H

#include <jni.h>
#include "m3_env.h"
#include "helpers.h"
#include "host.h"

const void *
m3_host_function0(IM3Runtime runtime, IM3ImportContext ctx, uint64_t *_sp, void *_mem);

const void *
m3_host_function1(IM3Runtime runtime, IM3ImportContext ctx, uint64_t *_sp, void *_mem);

const void *
m3_host_function2(IM3Runtime runtime, IM3ImportContext ctx, uint64_t *_sp, void *_mem);

const void *
m3_host_function3(IM3Runtime runtime, IM3ImportContext ctx, uint64_t *_sp, void *_mem);

#endif //KWASM3_BINDINGS_H
