@echo off
set OutFileName=build.h
set BuildFile=build.txt
set inc=1
set bld=
For /F "EOL=; DELIMS=" %%i In (%BuildFile%) Do (set bld=%%i)	

set /a newbld=bld+inc
rem ������������ *.h ����� � ������� ������
echo // ���� ���� ����������� ������������� ����� ������ ������>%OutFileName%
echo #ifndef _BUILD_H_>>%OutFileName%
echo #define _BUILD_H_>>%OutFileName%
echo #define __BUILD_NUMBER "����� ������: %newbld%">>%OutFileName%
echo #endif>>%OutFileName%

rem ��������� �����, ��������� ����� ������
echo ; ���� ���� �������� ����� ������� ������ > %BuildFile%
echo ; ����������, �� ��������� ��� � ������ >> %BuildFile%
echo %newbld% >> %BuildFile%
pause