@echo off
set projectDir=%1
set buildInfoFile=%2

set svn_subwcrev="C:\Program Files\TortoiseSVN\bin\SubWCRev.exe"

rem Clean up
if exist %buildInfoFile% del %buildInfoFile%

echo #ifndef __BUILDINFO_H__> %buildInfoFile%
echo #define __BUILDINFO_H__>> %buildInfoFile%
echo.>> %buildInfoFile%

rem Get software repository revision
echo #define SVN_SOFTWARE_REV               $WCREV$>> %buildInfoFile%
echo #define SVN_SOFTWARE_MODIFIED          "$WCMODS?m:$">> %buildInfoFile%
echo.>> %buildInfoFile%
%svn_subwcrev% %projectDir% %buildInfoFile% %buildInfoFile%

echo #endif // __BUILDINFO_H__>> %buildInfoFile%
