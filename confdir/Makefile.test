#------------------------------------------------------------------------------
# file : test/Makefile
#
# Makefile for building demo programs listed in make.lst
#------------------------------------------------------------------------------

include ../make.config
include make.lst

#------------------------------------------------------------------------------
# General settings
#------------------------------------------------------------------------------

IFLAGS = $(patsubst %,-I%,$(subst :, ,$(LEDA_INCL_PATH))) -I$(LEP_INCL_PATH) 
LFLAGS = -L$(LEDA_LIB_PATH) -L$(LEP_LIB_PATH)
RFLAGS = $(RFLAG)$(LEDA_LIB_PATH) $(RFLAG)$(LEP_LIB_PATH)
CFLAGS = $(CXX_OPT) $(CXX_FLAGS) $(CXX_USER_FLAGS)
LIB_NAME = lib$(LEP_NAME)$(LIB_SUFFIX)
LIBS = -l$(LEP_NAME) $(LEP_LIBS) -lG -lL $(SOCKET_LIBS)

all: $(PROGS)

.KEEP_STATE:

#----------------------------------------------------------------------------
# general rules:
#----------------------------------------------------------------------------

.c.o:
	$(CXX) $(IFLAGS) $(CFLAGS) $(LEP_COMP_FLAGS) -c $*.c        

.c:
	$(CXX) $(IFLAGS) $(CFLAGS) $(LEP_COMP_FLAGS) -c $*.c
	$(CXX) $(LEP_LINK_FLAGS) -o $* $*.o $(LFLAGS) $(RFLAGS) $(LIBS)

.o:
	$(CXX) $(LEP_LINK_FLAGS) -o $* $*.o $(LFLAGS) $(RFLAGS) $(LIBS)

#----------------------------------------------------------------------------
# clean and touch
#----------------------------------------------------------------------------

allclean: clean
	$(RM) -f $(PROGS)
	$(RM) -f -r ii_files

clean:
	$(RM) -f *.o *~ core

touch:
	touch *.c
