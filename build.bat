@echo off
setlocal

REM Detecta MSVC (cl)
where cl >nul 2>nul
if not errorlevel 1 goto build_msvc

REM Detecta GCC (gcc)
where gcc >nul 2>nul
if not errorlevel 1 goto build_gcc

echo Nenhum compilador encontrado (MSVC/cl ou GCC/gcc). Instale Visual Studio Build Tools (C++) ou MinGW-w64.
exit /b 1

:build_msvc
echo Compilando com MSVC cl
cl /nologo /W3 /EHsc /Fe:tetris_stack.exe super_trunfo.c
if errorlevel 1 (
  echo Falha ao compilar com MSVC.
  exit /b 1
)
echo Build concluido com MSVC
exit /b 0

:build_gcc
echo Compilando com GCC gcc
gcc -std=c11 -O2 -Wall -Wextra -o tetris_stack.exe super_trunfo.c
if errorlevel 1 (
  echo Falha ao compilar com GCC.
  exit /b 1
)
echo Build concluido com GCC
exit /b 0
