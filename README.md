# Prerequisites

- Clang
- CMake
- Conan

# iOS

```bash
scripts/build-ios.sh
```

# Web

```bash
scripts/build-emcc.sh
cd build-emcc
python3 ../servewasm.py
```