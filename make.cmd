@echo off
set obj_folder=%~dp0obj
set bin_folder=%~dp0demo
where cl >NUL 2>&1 1>NUL 
if not %errorlevel% equ 0 (
    echo please run %~nx0 in develop prompt environment
    goto :EOF
)
:compile
echo compiling the downloader
if not exist %obj_folder% mkdir %obj_folder%
if not exist %bin_folder% mkdir %bin_folder%
if not exist src/get.c (
    echo please put %~nx0 at the project root folder
    goto :EOF
)
cl /nologo /W2 /Fo%obj_folder%\get.obj /Fe%bin_folder%/get.exe src/get.c
if %errorlevel% equ 0 (
    echo well done & echo.get.exe has compiled in %bin_folder:*demo=demo% folder
) else (
    echo bad news & echo.
)

