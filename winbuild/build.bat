@echo off
if "%~1"=="" call "%~dp0%~nx0" /b&goto :EOF
rem chcp 866
if "%~1"=="/?" if "%~2"=="" (
	echo   Building solution from console without running MVS2010
	echo   Usage: %~n0 [/D] [/C ^| [/C] /B ^| /R [/C]] [/X]
	echo   ·
	echo   /D		Using configuration "Debug", for default "Release".
	echo   /C		Clean solution temparary building files.  
	echo   /B		Building solution ^(for default^).
	echo   /R		Rebuilding solution.
	echo   /X		Deleting debuging files from output folder.
	echo     		^(all files, except *.exe and *.dll will be deleted^)
	echo   ·
	echo   ·	
	echo   Построение решения из консоли без запуска MVS2010
	echo   Использование: %~n0 [/D] [/C ^| [/C] /B ^| /R [/C]] [/X]
	echo   ·
	echo   /D		Использование конфигурации "Debug", по умолчанию 
	echo     		используется конфигурация "Release".
	echo   /C		Очистка окружения построения, при совместном  
	echo     		использовании с /B или /R в зависимости от порядка
	echo     		следования параметров определяется порядок действий.
	echo     		Например: %~n0 /C /B - сначала очистит окружение,
	echo     		а только потом соберет; в отличии от %~n0 /R /C
	echo     		которое выполнит очистку после перестроения.
	echo   /B		Построение решение ^(действие по умолчанию^).
	echo   /R		Перестроение всего решения.
	echo   /X		Удаление отладочных файлов из выходного католога.
	echo     		^(все файлы, кроме *.exe и *.dll будут удалены^)
	echo   ·
	exit /b 0
)
setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION
call :folder "%~dp0"
title "!$folder!" - Starting...
rem mode con: COLS=140 LINES=3600
color F1	
rem cls

set param2=
set param3=
set param4=
set config="Release"
set paramX=

set arg1=%~1
set arg2=%~2
set arg3=%~3
set arg4=%~4
set arg5=%~5
set arg6=%~6
set arg7=%~7
set arg8=%~8
set arg9=%~9


for /L %%i in (1,1,9) do for /F %%a in ("!arg%%i!") do (
	call :LowerCase "%%a" "arg"	
	if "!arg!"=="/d" set config="Debug"
	if "!arg!"=="/c" if "!param3!"=="" (set param3=/Clean) else (if not "!param3!"=="/Clean" (set param4=/Clean))
	if "!arg!"=="/b" if "!param3!"=="" (set param3=/Build) else (if "!param3!"=="/Clean" (set param2=/Clean&set param3=/Build))
	if "!arg!"=="/r" if "!param3!"=="" (set param3=/Rebuild) else (if "!param3!"=="/Clean" (set param2=/Clean&set param3=/Rebuild))	
	if "!arg!"=="/x" set paramX=1		
)
if "!param2!"=="" if "!param3!"=="" if "!param4!"=="" if "!paramX!"=="" set param3=/Build

set reg_xOS=\Wow6432Node
if not defined PROCESSOR_ARCHITEW6432 if not "%PROCESSOR_ARCHITECTURE%"=="AMD64" set reg_xOS=
set reg_path=HKLM\SOFTWARE%reg_xOS%\Microsoft\VisualStudio\10.0
set reg_param=InstallDir
set reg_value=
for /F "UseBackQ Tokens=2*" %%I in (`reg query "%reg_path%"^|Find /I "%reg_param%"`) do set reg_value=%%J
if "%reg_value%"=="" (
  echo Please install "Microsoft Visual Studio 2010" and than run ".\%~nx0" again. 
  exit /b
)

for /L %%i in (2,1,4) do for /F %%p in ("!param%%i!") do if not "%%p"=="" (
	title "!$folder!" - devenv %%p !config!...
	call "!reg_value!devenv" "%~dp0\_vc10\fluorescence.sln" %%p !config! | findstr /P /I /V /R "\<warning\> \<предупреждение\>"
)

if not "!paramX!"=="" (
	title "!$folder!" - Cleaning...
	if not exist "%~dp0_temp\" (mkdir "%~dp0_temp\"&attrib +H "%~dp0_temp" /S /D)
	
	if exist "%~dp0_temp\%~n0.lst" del "%~dp0_temp\%~n0.lst"
	dir "%~dp0..\..\..\_build\Fluorescence\bin-%config%\" /B /A:D | findstr /P /I /R /C:".*">>"%~dp0_temp\%~n0.lst"
	for /F "usebackq delims=" %%i in ("%~dp0_temp\%~n0.lst") do (rmdir /S /Q "%~dp0..\..\..\_build\Fluorescence\bin32-%config%\%%i\")

	if exist "%~dp0_temp\%~n0.lst" del "%~dp0_temp\%~n0.lst"
	dir "%~dp0..\..\..\_build\Fluorescence\bin-%config%\" /B /A:A | findstr /P /I /V /R ".*.exe .*.dll">>"%~dp0_temp\%~n0.lst"
	for /F "usebackq delims=" %%i in ("%~dp0_temp\%~n0.lst") do (del /F /Q "%~dp0..\..\..\_build\Fluorescence\bin32-%config%\%%i")
	
	if exist "%~dp0_temp\%~n0.lst" del "%~dp0_temp\%~n0.lst"
)

title "!$folder!" - Done
echo ·
echo                                        ╔═══════════════════════════════╗
echo ───────────────────────────────────────╢ Solution building finished    ╟──────
echo                                        ╚═══════════════════════════════╝
pause
exit /b 0


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
	set $strlen=0&Set $strbuf=%~1
	if ""=="%~1" goto :EOF
	:StrLenLoop
	set /A $strlen+=1
	call set $strchr=%%$strbuf:~!$strlen!%%
	If ""=="!$strchr!" goto :EOF
	goto :StrLenLoop
	
:UpperCase
    setlocal enableextensions enabledelayedexpansion   
    call :Translate "%~1" "U" "strTempString"   
    endlocal & set %~2=%strTempString%
    exit /b 0

:LowerCase
    setlocal enableextensions enabledelayedexpansion
    call :Translate "%~1" "L" "strTempString"
    endlocal & set %~2=%strTempString%
    exit /b 0

:ProperCase
    setlocal enableextensions enabledelayedexpansion
    set strTempString=%~1
    call :UpperCase "%strTempString:~0,1%" "strFirstChar"
    call :LowerCase "%strTempString:~1%"   "strLastChars"
    endlocal & set %~2=%strFirstChar%%strLastChars%
    exit /b 0
	
rem ==========================================================================
rem %1 : строка текста
rem %2 : направление трансляции (L|U)
rem %3 : имя переменной для возврата транслированного значения
:Translate
    setlocal enableextensions enabledelayedexpansion
    set strTempString=%~1
    for /f "tokens=2,3" %%i in ('findstr.exe /b /l "# " "%~f0"') do (
        if /i "%~2" equ "L" (
            set strTempString=!strTempString:%%~i=%%~j!
        ) else if /i "%~2" equ "U" (
            set strTempString=!strTempString:%%~j=%%~i!
        )
    )
    endlocal & set %~3=%strTempString%
    exit /b 0

rem ==========================================================================
rem Таблица трансляции символов
# A a
# B b
# C c
# D d
# E e
# F f
# G g
# H h
# I i
# J j
# K k
# L l
# M m
# N n
# O o
# P p
# Q q
# R r
# S s
# T t
# U u
# V v
# W w
# X x
# Y y
# Z z
# А а
# Б б
# В в
# Г г
# Д д
# Е е
# Ё ё
# Ж ж
# З з
# И и
# Й й
# К к
# Л л
# М м
# Н н
# О о
# П п
# Р р
# С с
# Т т
# У у
# Ф ф
# Х х
# Ц ц
# Ч ч
# Ш ш
# Щ щ
# Ъ ъ
# Ы ы
# Ь ь
# Э э
# Ю ю
# Я я
rem ==========================================================================