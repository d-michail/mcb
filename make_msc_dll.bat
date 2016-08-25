@echo off

rem %1: microsoft runtime library to link with

set libc=
if "%1" == "default"    set libc=
if "%1" == "ml"         set libc=ml
if "%1" == "mld"        set libc=mld
if "%1" == "md"         set libc=md
if "%1" == "mdd"        set libc=mdd
if "%1" == "mt"         set libc=mt
if "%1" == "mtd"        set libc=mtd
if "%1" == "" goto usage


set forcepostfix=none
if "%2" == "default"    set forcepostfix=default
if "%2" == "ml"         set forcepostfix=ml
if "%2" == "mld"        set forcepostfix=mld
if "%2" == "md"         set forcepostfix=md
if "%2" == "mdd"        set forcepostfix=mdd
if "%2" == "mt"         set forcepostfix=mt
if "%2" == "mtd"        set forcepostfix=mtd

set make=nmake -nologo
set vccomp=vc7

REM install necessary makefiles to folders
echo installing build files
copy confdir\msvsstatic\make.all Makefile
copy confdir\msvsstatic\make.src src\Makefile
copy confdir\msvsdll\make.lst.src src\make.lst
copy confdir\msvsstatic\make.pro test\Makefile
copy confdir\msvsdll\make.lst.test test\make.lst
copy confdir\msvsstatic\make.pro demo\Makefile
copy confdir\msvsdll\make.lst.demo demo\make.lst

rem install appropriate compiler options
echo installing appropriate compiler options
copy confdir\msvsstatic\comp\%vccomp%\cc%libc% src\comp.lst
copy confdir\msvsstatic\comp\%vccomp%\cc%libc% test\comp.lst
copy confdir\msvsstatic\comp\%vccomp%\cc%libc% demo\comp.lst

REM test if we should force the postfix of the leda library name

if "%forcepostfix%" == "none" goto :endforcepostfix
if "%forcepostfix%" == "default" goto :addemptyforcepostfix

echo LINKVER = _%forcepostfix% >> src\comp.lst
echo LINKVER = _%forcepostfix% >> test\comp.lst
echo LINKVER = _%forcepostfix% >> demo\comp.lst
goto :endforcepostfix

:addemptyforcepostfix

echo LINKVER = >> src\comp.lst
echo LINKVER = >> test\comp.lst
echo LINKVER = >> demo\comp.lst

:endforcepostfix

REM test for LEDA-5.0 include structure
if not exist %LEDAROOT%\incl\LEDA\system\basic.h goto leda_l_5

echo LEDAFLAGS = $(LEDAFLAGS) -DLEDA_GE_V5 >> test\make.lst
echo LEDAFLAGS = $(LEDAFLAGS) -DLEDA_GE_V5 >> demo\make.lst
echo LEDAFLAGS = $(LEDAFLAGS) -DLEDA_GE_V5 >> src\make.lst

:leda_l_5

%make%
set make=
goto :quit

:usage
echo.
echo Usage: make_msvs_static runtimelib [forceledapostfix]
echo.
echo Possible options for runtimelib:
echo.
echo ml, mld, mt, mtd, md, mdd, default
echo.
echo If forceledapostfix is not empty, it is assumed that the leda
echo libraries are named like libleda_forceledapostfix.lib
echo.
echo Possible options for forceledapostfix:
echo.
echo ml, mld, mt, mtd, md, mdd, default
echo.
echo default corresponds to forceledapostfix beeing the empty string.
goto :quit

:quit
set libc=
