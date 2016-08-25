@echo off
set make=make

REM install necessary makefiles to folders
copy confdir\bcc32static\make.all Makefile
copy confdir\bcc32static\make.src src\Makefile
copy confdir\bcc32static\make.lst.src src\make.lst
copy confdir\bcc32static\make.pro test\Makefile
copy confdir\bcc32static\make.lst.test test\make.lst
copy confdir\bcc32static\make.pro demo\Makefile
copy confdir\bcc32static\make.lst.demo demo\make.lst

REM test for LEDA-5.0 include structure
if not exist %LEDAROOT%\incl\LEDA\system\basic.h goto leda_l_5

echo LEDAFLAGS = $(LEDAFLAGS) -DLEDA_GE_V5 >> test\make.lst
echo LEDAFLAGS = $(LEDAFLAGS) -DLEDA_GE_V5 >> demo\make.lst
echo LEDAFLAGS = $(LEDAFLAGS) -DLEDA_GE_V5 >> src\make.lst

:leda_l_5


REM not check about LEDA 6.0 and library paths
if exist %LEDAROOT%\libGeoW.lib goto leda_l_6

echo LEDALIBS = leda.lib >> test\make.lst
echo LEDALIBS = leda.lib >> demo\make.lst

:leda_l_6

echo LEDALIBS = libg.lib libl.lib >> test\make.lst
echo LEDALIBS = libw.lib libp.lib libg.lib libl.lib >> test\make.lst

%make%
set make=
