@echo off
set TCC_PATH=.\tcc
set INCLUDE_PATH=include
set SRC_PATH=src

%TCC_PATH%\tcc.exe -I%INCLUDE_PATH% -Itcc\lib\libtcc^
  -o c_ide.exe ^
  %SRC_PATH%\main.c ^
  %SRC_PATH%\editor.c ^
  %SRC_PATH%\compiler.c ^
  %SRC_PATH%\file_io.c ^
  %SRC_PATH%\output.c ^
  %SRC_PATH%\editor.c ^
  -L%TCC_PATH%\lib ^
  -ltcc -luser32 -lkernel32 -lgdi32 -lcomctl32 -lshell32 -lcomdlg32

if %errorlevel% equ 0 (
    echo Build successful: c_ide.exe
) else (
    echo Build failed!
    exit /b 1
)