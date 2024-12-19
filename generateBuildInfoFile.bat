@echo off
set projectDir=%1
set buildInfoFile=%2

rem Clean up
if exist %buildInfoFile% del %buildInfoFile%
setlocal ENABLEDELAYEDEXPANSION
echo #ifndef __BUILDINFO_H__> %buildInfoFile%
echo #define __BUILDINFO_H__>> %buildInfoFile%
echo.>> %buildInfoFile%

call git log -n 1 %projectDir% > tmp.txt
set /p full_hash= <  tmp.txt 
SET hash=!full_hash:~7,8!

call git diff %projectDir% > tmp.txt
set /p modified=<  tmp.txt
del  tmp.txt
echo.%modified% | findstr /C:"diff" 1>nul
if errorlevel 1 (
   set value=""
) ELSE (
   set value="m"
)

rem Get software repository revision
echo #define SVN_SOFTWARE_REV               0x%hash% >> %buildInfoFile%
echo #define SVN_SOFTWARE_MODIFIED          %value% >> %buildInfoFile%
echo.>> %buildInfoFile%
rem %svn_subwcrev% %projectDir% %buildInfoFile% %buildInfoFile%

echo #endif // __BUILDINFO_H__>> %buildInfoFile%
