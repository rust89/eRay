@echo off
set OutFileName=build.h
set BuildFile=build.txt
set inc=1
set bld=
For /F "EOL=; DELIMS=" %%i In (%BuildFile%) Do (set bld=%%i)	

set /a newbld=bld+inc
rem Формирование *.h файла с номером сборки
echo // Этот файл генрируется автоматически после каждой сборки>%OutFileName%
echo #ifndef _BUILD_H_>>%OutFileName%
echo #define _BUILD_H_>>%OutFileName%
echo #define __BUILD_NUMBER "Номер сборки: %newbld%">>%OutFileName%
echo #endif>>%OutFileName%

rem перзапись файла, хранящего номер сборки
echo ; этот файл содержит номер текущей сборки > %BuildFile%
echo ; пожалуйста, не изменяйте его в ручную >> %BuildFile%
echo %newbld% >> %BuildFile%
pause