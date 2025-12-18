@echo off
REM Build C IDE using TCC

set TCC_PATH=.\tcc
set INCLUDE_PATH=include
set SRC_PATH=src

echo Compiling...

%TCC_PATH%\tcc.exe -I%INCLUDE_PATH% ^
  -o c_ide.exe ^
  %SRC_PATH%\*.c ^
  -L%TCC_PATH%\lib -I%TCC_PATH%\libtcc -I%TCC_PATH%\include\winapi ^
  -ltcc -luser32 -lkernel32 -lgdi32 -lcomctl32

if %errorlevel% equ 0 (
    echo Build successful: c_ide.exe
) else (
    echo Build failed!
    exit /b 1
)