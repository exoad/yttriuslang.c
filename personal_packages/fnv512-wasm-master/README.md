# fnv512

fnv512 is an implementation for the FNV-0/FNV-1/FNV-1A 512 bits hashing algorithms
 which has been rebuilt for WebAssembly (WASM).
 
 Live demo: https://crashdemons.github.io/fnv512-wasm/

All credit goes to tweqx for this library

## Build Requirements
 - emcc (Emscripten) or clang++
 - updated C++11 libraries/includes

## Building the project yourself

 - run make to generate the tests, or ./make-wasm.sh to generate the JS loader and WASM
 - note: fnv512-wasm.js is a barebones emscripten loader for the WASM Module.  fnv512-wrapped.js automatically loads and sets up a JS Object with more useful methods to you.


## Usage

coming soon

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License
[GPLv3](https://www.gnu.org/licenses/gpl-3.0.html)

[uint512 stub](https://github.com/crashdemons/uint512_llvm) is available separately under LGPLv3 to allow reuse as a library.
