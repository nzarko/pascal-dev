@echo off
REM run script used to execute win32 application

CHCP 1253 >nul 2>nul

cd @progpath@
@progname@
pause
exit
