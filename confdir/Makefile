#------------------------------------------------------------------------------
#
# Makefile for creating configure and installing the Makefiles
#
#------------------------------------------------------------------------------

CP = cp

INPUT_FILES = configure.in make.config.in

COPY_FILES = Makefile.demo Makefile.root Makefile.src Makefile.test \
make.lst.demo make.lst.src make.lst.test configure

all : configure copyfiles

configure : $(INPUT_FILES) 
	autoconf

copyfiles : $(COPYFILES)
	$(CP) Makefile.demo ../demo/Makefile
	$(CP) Makefile.test ../test/Makefile
	$(CP) Makefile.src ../src/Makefile
	$(CP) Makefile.root ../Makefile
	$(CP) make.lst.demo ../demo/make.lst
	$(CP) make.lst.test ../test/make.lst
	$(CP) make.lst.src ../src/make.lst
	$(CP) configure ../configure

clean :
	rm -f *~
