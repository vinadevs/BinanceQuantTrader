## Visual studio (Window only)

1. Make sure you have vcpkg installed in your PC
2. Go to Source/BinanceQuantTrader
3. Open file BinanceQuantTrader.sln by Visual Studio 2022, right click on QuantLibrary project then build
4. Copy 2 files QuantLib-x64-mt-gd.lib and QuantLib-x64-mt.lib from BinanceQuantTrader\Source\QuantLibrary\lib
   to BinanceQuantTrader\Source\Lib64\
5. Right click on BinanceQuantTrader then build or ctrl + B to build the whole solution

## CMake (Window/Linux)

### Window
```powershell
$env:VCPKG_ROOT = "</path/to/vcpkg>"
$env:PATH = "$env:VCPKG_ROOT;$env:PATH"
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake"
cmake --build build
```

### Linux

```bash
# Set your vcpkg root path
export VCPKG_ROOT="</path/to/vcpkg>"
export PATH="$VCPKG_ROOT:$PATH"
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
cmake --build build
```

**Debug Build**

```bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

**Release Build**
```bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" -DCMAKE_BUILD_TYPE=Release
cmake --build build
```