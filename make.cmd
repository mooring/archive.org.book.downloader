@echo off
set obj_folder=%~dp0obj
set bin_folder=%~dp0demo
where cl >NUL 2>&1 1>NUL 
if not %errorlevel% equ 0 (
    color 04
    cls
    title environment error
    echo please run %~nx0 in develop prompt environment
    pause & color & title=%comspec%
    goto :EOF
)
:compile
echo compiling the downloader
if not exist %obj_folder% mkdir %obj_folder%
if not exist %bin_folder% mkdir %bin_folder%
if not exist src/get.c (
    cls
    color 03
    title file location error
    echo please put %~nx0 at the project root folder
    pause & color & title=%comspec%
    goto :EOF
)
cl /nologo /W2 /Fo%obj_folder%\get.obj /Fe%bin_folder%/get.exe src/get.c
if %errorlevel% equ 0 (
    cls
    color 03
    title success
    echo well done & echo.get.exe has compiled in %bin_folder:*demo=demo% folder
    echo please use archiveOrgAssistant to generate config.conf file
    echo and then put it in %bin_folder%
    pause & color & title=%comspec%
) else (
    title compile error
    echo bad news & echo.
    pause & color & title=%comspec%
)

