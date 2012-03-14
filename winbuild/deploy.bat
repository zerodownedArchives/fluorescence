
chcp 65001
@echo off
if "%~1"=="/?" if "%~2"=="" (
	echo   ENU: Deploying and updating project envirement
	echo   RUS: Развертывание и обновление окружения проекта
	echo ∙
	exit /b 0
)
setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION
color F1
call :folder "%~dp0"
title "!$folder!" - Fluorescence

echo Fluorescence project deploying structer:
call :fpath "%~dp0" 3
echo %$fpath%^>
call :folder "%~dp0" 2
echo     !$folder!
echo     ├ build
echo     │ ├ Fluorescence
echo     │ │ ├ bin32-Debug
echo     │ │ ├ bin32-Release
echo     │ │ ├ data
echo     │ │ ├ effects
echo     │ │ ├ fonts
echo     │ │ ├ gumps
echo     │ │ ├ shader
echo     │ │ ├ shards
echo     │ │ └ themes
echo     │ └ ...
call :folder "%~dp0" 1
echo     ├ !$folder!
call :folder "%~dp0" 0
echo     │ ├ !$folder! 
echo     │ │ ├ data
echo     │ │ ├ ...
echo     │ │ ├ src
echo     │ │ ├ winbuild
echo     │ │ │ ├ _vc10
echo     │ │ │ ├ bin32
echo     │ │ │ ├ lib32
echo     │ │ │ ├ include
echo     │ │ │ ├ build.bat
echo     │ │ │ ├ deploy.bat
echo     │ │ │ └ ...
echo     │ │ ├ credits.txt
echo     │ │ └ LICENSE
echo     │ └ ...
echo     └ ...
	
set argY=
if "!argY!"=="" (
	:Prompt1
	set /P prompt=Do you accept deploying structure? [Y:N]:
	if "!prompt!"=="n" (exit /b) else (if "!prompt!"=="N" exit /b) 
	if not "!prompt!"=="y" if not "!prompt!"=="Y" goto :Prompt1
)

set locl=
if "!locl!"=="" (
	:Prompt2
	echo  Aviliable Languages:
	echo  ├[1] ENU - English (default)
	echo  └[2] RUS - Русский
	set /P prompt=Choose language for data files:	
	if "!prompt!"=="2" set locl=rus
	if not "!prompt!"=="1" if "!locl!"=="" goto :Prompt2 
)

set fls_count=0	
set dir=%~dp0..\..\..\_build\Fluorescence
echo Copieng files to ".\..\..\..\_build\fluorescence\":
call :dcopy "bin32\"		"\"
call :dcopy "..\data\"		"\data\"
call :dcopy "..\effects\"	"\effects\"
call :dcopy "..\fonts\"		"\fonts\"
call :dcopy "..\gumps\"		"\gumps\"
call :dcopy "..\shards\"	"\shards\"
call :dcopy "..\shader\"	"\shader\"
call :dcopy "..\themes\"	"\themes\"
echo There wera !fls_count! files copied.


echo                                               ╔════════════════════════╗
echo ══════════════════════════════════════════════╣ Project deploying done ╠══════
echo                                               ╚════════════════════════╝
pause
exit /b 0

:dcopy
	if not exist "!dir!%~2" mkdir "!dir!%~2"
	for /F "usebackq  delims=" %%d in (`dir "%~dp0%~1\" /A:D /B^|findstr /P /I /V /R /C:"_..."`) do (call :dcopy "%~1%%d\" "%~2%%d\")
	for /F "usebackq  delims=" %%f in (`dir "%~dp0%~1\" /A:A /B^|findstr /P /I /V /C:"File Not Found"`) do (call :fcopy "%~1%%f" "%~2")
	if not "!locl!"=="" if exist "%~dp0%~1_!locl!\" call :dcopy "%~1_!locl!\" "%~2"
	goto :EOF

:fcopy
	if not exist "!dir!%~2" mkdir "!dir!%~2"
	echo   "%~nx1" -^> _build\fluorescence%~2
	set /a fls_count=fls_count+1
	copy /Y "%~dp0%~1" "!dir!%~2" > nul	
	goto :EOF

:fpath
	pushd %1
	for /L %%I in (1,1,%2) do cd ..
	set $fpath=%cd%
	popd
	goto :EOF
	
:folder
	set $folder=%~dp1
	if not ""=="%~2" if not "0"=="%~2" (
		set folderbuf=%~dp1
		for /L %%f in (1,1,%~2) do (
			call :folder "!folderbuf!"
			call :strlen "!$folder!"
			for /L %%i in (0,1,!$strlen!) do set folderbuf=!folderbuf:~0,-1!
		)
		set $folder=!folderbuf!
	)	
	for /D %%a in ("!$folder:~0,-1!.ext") do set $folder=%%~na
	goto :EOF
	
:strlen
	set $strlen=0&set $strbuf=%~1
	if ""=="%~1" goto :EOF
	:StrLenLoop
	set /A $strlen+=1
	call set $strchr=%%$strbuf:~!$strlen!%%
	if ""=="!$strchr!" goto :EOF
	goto :StrLenLoop
