rm -rf fnv512-wasm-dist
mkdir fnv512-wasm-dist

W_FLAGS="-O3 -flto -DNDEBUG"
#W_FLAGS="-O0 -g -v -s ASSERTIONS=1 -s DISABLE_EXCEPTION_CATCHING=0 --memoryprofiler"
echo $W_FLAGS
emcc $W_FLAGS -s MODULARIZE=1 -s 'EXPORT_NAME="createFnv512Module"' -s WASM=1 -s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap"]' uint512_llvm.cpp fnv512.cpp -o fnv512-wasm-dist/fnv512-wasm.js


cat fnv512-wasm-dist/fnv512-wasm.js js-wrapper/wrapper.js > fnv512-wasm-dist/fnv512-wrapped.js

