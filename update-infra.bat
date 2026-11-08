@echo off
setlocal
cd /d "%~dp0"

rem usage: update-infra.bat [Release|Debug|Dev] [/configs] [/runtime]
rem   /configs  also overwrite rtx.conf and map_settings.toml (old ones saved as .bak)
rem   /runtime  also sync bin\.trex (Remix runtime, large)
rem   INFRA_ROOT env var overrides the game folder

set "CONFIG=Release"
set "WITH_CONFIGS=0"
set "WITH_RUNTIME=0"
if not defined INFRA_ROOT set "INFRA_ROOT=O:\SteamLibrary\steamapps\common\infra"
if "%INFRA_ROOT:~-1%"=="\" set "INFRA_ROOT=%INFRA_ROOT:~0,-1%"

:args
if "%~1"=="" goto args_done
if /i "%~1"=="/configs" set "WITH_CONFIGS=1"& goto args_next
if /i "%~1"=="/runtime" set "WITH_RUNTIME=1"& goto args_next
set "CONFIG=%~1"
:args_next
shift
goto args
:args_done

if not exist "%INFRA_ROOT%\infra.exe" goto no_game

tasklist /fi "imagename eq infra.exe" | find /i "infra.exe" >nul
if not errorlevel 1 goto running

echo Target: %INFRA_ROOT%
echo Config: %CONFIG%

set "DLL=build\bin\%CONFIG%\infra-rtx.dll"
if exist "%DLL%" goto copy_dll
echo [warn] %DLL% not found - dll not updated
if not exist "%INFRA_ROOT%\infra-rtx.dll" goto no_dll
goto after_dll

:copy_dll
if exist "%INFRA_ROOT%\infra-rtx.dll" copy /y "%INFRA_ROOT%\infra-rtx.dll" "%INFRA_ROOT%\infra-rtx.dll.bak" >nul
copy /y "%DLL%" "%INFRA_ROOT%\infra-rtx.dll" >nul
if errorlevel 1 goto failed
echo Copied %DLL%

:after_dll
if exist "%INFRA_ROOT%\p2-rtx.dll" move /y "%INFRA_ROOT%\p2-rtx.dll" "%INFRA_ROOT%\p2-rtx.dll.old" >nul

call :sync "assets\bin" "%INFRA_ROOT%\bin" /E /XD .trex
call :sync "assets" "%INFRA_ROOT%" *.bat dxvk.conf
call :sync "assets\infra-rtx" "%INFRA_ROOT%\infra-rtx" /E /XF map_settings.toml

if "%WITH_CONFIGS%"=="1" call :update_configs
if "%WITH_RUNTIME%"=="1" call :sync "assets\bin\.trex" "%INFRA_ROOT%\bin\.trex" /E /XF d3d9_runtime.zip

echo Done.
exit /b 0

:sync
robocopy %* /NJH /NJS /NP /NDL /XX /R:1 /W:1
if errorlevel 8 goto failed
exit /b 0

:update_configs
if exist "%INFRA_ROOT%\rtx.conf" copy /y "%INFRA_ROOT%\rtx.conf" "%INFRA_ROOT%\rtx.conf.bak" >nul
copy /y "assets\rtx.conf" "%INFRA_ROOT%\rtx.conf" >nul
if errorlevel 1 goto failed
if exist "%INFRA_ROOT%\infra-rtx\map_settings.toml" copy /y "%INFRA_ROOT%\infra-rtx\map_settings.toml" "%INFRA_ROOT%\infra-rtx\map_settings.toml.bak" >nul
copy /y "assets\infra-rtx\map_settings.toml" "%INFRA_ROOT%\infra-rtx\map_settings.toml" >nul
if errorlevel 1 goto failed
echo Updated rtx.conf and map_settings.toml (backups: .bak)
exit /b 0

:no_game
echo infra.exe not found in "%INFRA_ROOT%". Set INFRA_ROOT to the game folder.
goto failed

:running
echo INFRA is running. Close the game first.
goto failed

:no_dll
echo No dll to install: build the project first or pick another config.
goto failed

:failed
echo Update failed.
pause
exit /b 1
