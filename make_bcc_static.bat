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

%make%
set make=
