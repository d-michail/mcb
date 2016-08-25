#------------------------------------------------------------------------------
# file : Makefile
#------------------------------------------------------------------------------

include make.config

ifdef ETAGS
    ETAGFILES = $(shell find . -name "*.[Cch]" | grep -v /CVS/)
endif
ifdef CTAGS
    CTAGFILES = $(shell find . -name "*.[Cch]" | grep -v /CVS/)
endif

LIB_NAME = lib$(LEP_NAME)$(LIB_SUFFIX)
LEP_INCL_PATH_NAME = $(LEDA_INCL_PATH)/LEP/$(LEP_PATH_NAME)

#------------------------------------------------------------------------------
# Libraries & Programs
#------------------------------------------------------------------------------

all: lib Test Demo

#------------------------------------------------------------------------------
# Libraries
#------------------------------------------------------------------------------

lib:
	@cd src; \
	$(MAKE) -i

touch:
	@cd src; \
	$(MAKE) -i touch

#------------------------------------------------------------------------------
# Demos
#------------------------------------------------------------------------------

Demo:
	cd demo; $(MAKE) -i

#------------------------------------------------------------------------------
# Test programs
#------------------------------------------------------------------------------

Test:
	cd test; $(MAKE) -i  

#------------------------------------------------------------------------------
# Reorganinzation and cleaning up 
#------------------------------------------------------------------------------

lepintegrate:
	@if [ ! -d $(LEP_INCL_PATH_NAME) ]; then \
	$(MKDIR) $(LEP_INCL_PATH_NAME); fi
	$(CP) incl/LEP/$(LEP_PATH_NAME)/*.h $(LEP_INCL_PATH_NAME)
# moves header files to <LEDA>/incl/LEP/<LEP_PATH_NAME>

confclean: allclean
	$(RM) -f *~ log config.log config.status configure
	cd confdir; $(MAKE) -i clean

allclean: clean
	cd src; $(MAKE) -i allclean
	cd demo; $(MAKE) -i allclean
	cd test; $(MAKE) -i allclean
	$(RM) -f lib$(LEP_NAME).so static/lib$(LEP_NAME).a lib$(LEP_NAME).sl

clean:
	cd demo; $(MAKE) -i clean
	cd test; $(MAKE) -i clean
	cd src; $(MAKE) -i clean

depend:
	cd src; $(MAKE) -i depend
	cd demo; $(MAKE) -i depend
	cd test; $(MAKE) -i depend

#------------------------------------------------------------------------------
# CVS changelog and tags
#------------------------------------------------------------------------------

changelog: 
	cvs2cl -P --utc

tags: $(CTAGFILES)
	$(CTAGS) $(CTAGFILES)

TAGS: $(ETAGFILES)
	$(ETAGS) $(ETAGFILES)

#------------------------------------------------------------------------------
# Creating configure script
#------------------------------------------------------------------------------

conf:
	cd confdir; $(MAKE) -i configure
	cd confdir; $(MAKE) -i copyfiles
