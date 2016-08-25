#------------------------------------------------------------------------------
# file : Makefile
#
# Makefile for building programs listed in PROGS variable
#------------------------------------------------------------------------------

##
## CHANGE THE SETTINGS BELOW
##

#
# fix this to point to the make.config file in the root directory of the LEP
#
include <PATH-TO-MCB-LEP>/make.config

#
# fix this to contain a space separated list of programs to build
#
PROGS = myprogram1 myprogram2

##
## DO NOT CHANGE AFTER HERE
##

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
