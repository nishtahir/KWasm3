# KWasm3

A JNI Wasm3 wrapper for Kotlin/Android.

## Usage

```
let wasmModuleByteArray = context.assets.open("module.wasm")
let kwasm3 = KWasm3.builder()
    .withBinaryModule("module", wasmModuleByteArray)
    .withStackSize(8192)
    .withHostFunction<Int, Int>("env", "myImportedFunction"){ p1 -> p1 }
    .build()
    .execute("myExportedFunction")
```

## Roadmap
- [x] Bind host functions to wasm3
- [ ] Exported functions API return values
- [ ] Bind imported memory to WASM3
- [ ] API to access exported memory through host functions
- [ ] Higher order type conversions (Strings etc...)

## Resources

* [Java Native Interface (JNI) Example](https://www.protechtraining.com/blog/post/java-native-interface-jni-example-65)
* [WASM3](https://github.com/wasm3/wasm3) - The fastest WebAssembly Interpreter
* [KWasm](https://github.com/jasonwyatt/KWasm) - Inspiration for the API
