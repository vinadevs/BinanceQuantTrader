@echo off

:buildVCPKG
if exist .\vcpkg.exe (goto :installDeps)

echo Please run this inside vcpkg\ directory to install dependencies
goto :end

:installDeps

set VCPKG_TARGET_TRIPLET "x64-windows"
.\vcpkg.exe install boost --triplet x64-windows
.\vcpkg.exe install boost-asio --triplet x64-windows
.\vcpkg.exe install boost-test --triplet x64-windows
.\vcpkg.exe install boost-multiprecision --triplet x64-windows
.\vcpkg.exe install boost-callable-traits --triplet x64-windows   
.\vcpkg.exe install boost-beast --triplet x64-windows 
.\vcpkg.exe install zeromq --triplet x64-windows
.\vcpkg.exe integrate install

echo vcpkg dependencies installation is done!

:end
