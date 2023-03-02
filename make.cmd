@echo off
@where cl 2>&1 1>NUL
if not %errorlevel% equ 0 (
    echo "please run this in develop prompt environment"
    goto :EOF
)
:all
if not exist obj mkdir obj
echo compiling the downloader
@cl /nologo /W2 /Foobj/get.obj /Fedemo/get.exe src/get.c
if %errorlevel% equ 0 (
    echo well done & echo.get.exe has compiled in demo folder
) else (
    echo bad news & echo.
)

