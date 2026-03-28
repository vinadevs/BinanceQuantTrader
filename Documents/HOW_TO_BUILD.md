# Building Binance Quant Trader

Quick start guide using CMake presets for Windows, and Docker Dev Container.

---

## Prerequisites

### Windows
- Visual Studio 2022
- CMake 3.20+
- vcpkg

### Docker Dev Container
- Docker Desktop / Docker Engine
- VS Code with Dev Containers extension

---

## Windows - CMake Preset

### 1. Setup vcpkg
```powershell
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
```

### 2. Set Environment
```powershell
$env:VCPKG_ROOT = "C:\path\to\vcpkg"
$env:PATH = "$env:VCPKG_ROOT;$env:PATH"
```

### 3. List Available Presets
```powershell
cmake --list-presets
```

### 4. Build

**Debug Build:**
```powershell
cmake --build --preset Debug-Windows
```

**Release Build:**
```powershell
cmake --build --preset Release-Windows
```

**Clean Build:**
```powershell
cmake --build --preset Debug-Windows --target clean
cmake --build --preset Debug-Windows
```

### 5. Output
- **DLLs:** `Build-Windows/Debug/Bin/Debug/`
- **Libraries:** `Build-Windows/Debug/Lib/Debug/`
- **Executables:** `Build-Windows/Debug/Bin/Debug/`
---

## Docker Dev Container - CMake Preset

### 1. Prerequisites
- Install Docker Desktop
- Install VS Code Dev Containers extension

### 2. Open in Dev Container
```
Ctrl+Shift+P → Dev Containers: Reopen in Container
```

### 3. Inside Container - First Setup
```bash
export VCPKG_ROOT="/vcpkg"
export PATH="$VCPKG_ROOT:$PATH"
vcpkg integrate install
```

### 4. List Available Presets
```bash
cmake --list-presets
```

### 5. Build

**Debug Build:**
```bash
cmake --build --preset Debug-Linux
```

**Release Build:**
```bash
cmake --build --preset Release-Linux
```

**Clean Build:**
```bash
cmake --build --preset Debug-Linux --target clean
cmake --build --preset Debug-Linux
```

### 6. Output
Output located at: `/workspaces/BinanceQuantTrader/build/Debug/`

---

## CMake Preset Reference

View all available presets:
```bash
# Windows
cmake --list-presets

# Linux / Docker
cmake --list-presets
```

Common preset commands:
```bash
# Build specific preset
cmake --build --preset <preset-name>

# Clean specific preset
cmake --build --preset <preset-name> --target clean

# Rebuild (clean + build)
cmake --build --preset <preset-name> --target clean
cmake --build --preset <preset-name>

# Verbose output
cmake --build --preset <preset-name> --verbose
```

---

## Troubleshooting

### "VCPKG_ROOT not found"
Set the environment variable:
- **Windows:** `$env:VCPKG_ROOT = "C:\path\to\vcpkg"`
- **Linux:** `export VCPKG_ROOT="/path/to/vcpkg"`

### "Preset not found"
Run `cmake --list-presets` to see available presets. Update CMakePresets.json if needed.

### Build fails with linker errors
```bash
# Clean and rebuild
cmake --build --preset <preset-name> --target clean
cmake --build --preset <preset-name>
```

### Dev Container won't start
```
Ctrl+Shift+P → Dev Containers: Rebuild Container
```

---

## Quick Commands Summary

| Task | Command |
|------|---------|
| List presets | `cmake --list-presets` |
| Debug build | `cmake --build --preset Debug-Windows` (Windows) or `Debug-Linux` (Linux) |
| Release build | `cmake --build --preset Release-Windows` (Windows) or `Release-Linux` (Linux) |
| Clean | `cmake --build --preset <preset> --target clean` |
| Rebuild | Clean + Debug build |
| Verbose output | Add `--verbose` flag |

