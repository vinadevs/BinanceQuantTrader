## Visual studio IDE (Windows only)

1. Make sure you have vcpkg installed in your PC
2. Go to Source/BinanceQuantTrader
3. Open file BinanceQuantTrader.sln by Visual Studio 2022, right click on QuantLibrary project then build
4. Copy 2 files QuantLib-x64-mt-gd.lib and QuantLib-x64-mt.lib from BinanceQuantTrader\Source\QuantLibrary\lib
   to BinanceQuantTrader\Source\Lib64\
5. Right click on BinanceQuantTrader then build or ctrl + B to build the whole solution

Note: If you can not build due to missing header errors from libraries like boost, grpc,...
It is possibility that your vcpkg manager is not configured now in your Visual Studio project solution
then you may want to close Visual Studio and then run this command then reopen Visual Studio again
where_you_install_vcpkg> ./vcpkg integrate install

## CMake (Windows/Linux)

### Windows
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

**Clean Build**
```bash
cmake --build .\build\ --target clean
```

**Dev Container (Vscode only)**

SSH Agent Forwarding must be enabled

On Windows Powershell:
```powershell
ssh-add $env:SSH_AUTH_SOCK
```

On Linux/Mac Terminal:
```bash
ssh-add $SSH_AUTH_SOCK
```

On VSCode Command Palette: Ctrl + Shift + P
- Dev Containers: Rebuild and reopen container...

Inside Dev Container Terminal to build project:
```bash
vcpkg integrate install
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
cmake --build build -j8
```