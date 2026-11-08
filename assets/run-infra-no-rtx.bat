@echo off
setlocal
cd /d "%~dp0"

tasklist /fi "imagename eq infra.exe" | find /i "infra.exe" >nul
if not errorlevel 1 (
    echo INFRA is already running.
    pause
    exit /b 1
)

if exist bin\d3d9.dll ren bin\d3d9.dll d3d9.dll.rtx_off
if exist bin\winmm.dll ren bin\winmm.dll winmm.dll.rtx_off

if exist bin\d3d9.dll goto failed
if exist bin\winmm.dll goto failed

start "" /wait infra.exe -console -insecure -steam -novid %*

:restore
if exist bin\d3d9.dll.rtx_off ren bin\d3d9.dll.rtx_off d3d9.dll
if exist bin\winmm.dll.rtx_off ren bin\winmm.dll.rtx_off winmm.dll
exit /b 0

:failed
echo Could not disable Remix files in bin.
call :restore
pause
exit /b 1