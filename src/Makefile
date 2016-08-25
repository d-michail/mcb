#------------------------------------------------------------------------------
# file : src/Makefile
#------------------------------------------------------------------------------

include ../make.config
include make.lst

ifdef ETAGS
    ETAGFILES = $(shell find . -name "*.[Cch]" | grep -v /CVS/)
endif
ifdef CTAGS
    CTAGFILES = $(shell find . -name "*.[Cch]" | grep -v /CVS/)
endif

#------------------------------------------------------------------------------
# General settings
#------------------------------------------------------------------------------

LEP_OBJECTS = $(LEP_SOURCES:.c=.o)

IFLAGS = $(patsubst %,-I%,$(subst :, ,$(LEDA_INCL_PATH))) -I$(LEP_INCL_PATH) 
CFLAGS = $(CXX_OPT) $(CXX_FLAGS) $(CXX_USER_FLAGS)
LIB_NAME = lib$(LEP_NAME)$(LIB_SUFFIX)

all: $(LINKING_MODE)

.KEEP_STATE:


#------------------------------------------------------------------------------
# Library building
#------------------------------------------------------------------------------

shared: $(LEP_OBJECTS)
	$(CXX) $(CXX_LIB_FLAGS) $(LEP_LIB_FLAGS) -o $(LIB_NAME) $(LEP_OBJECTS)
	@$(MV) $(LIB_NAME) $(LEP_LIB_PATH)

static: $(LEP_OBJECTS)
	$(AR) $(AROPTS) $(LIB_NAME) $?
	@if [ ! -d $(LEP_LIB_PATH) ]; then \
	$(MKDIR) $(LEP_LIB_PATH); fi
	@$(MV) $(LIB_NAME) $(LEP_LIB_PATH)

#----------------------------------------------------------------------------
# general rules:
#----------------------------------------------------------------------------

.c.o:
	$(CXX) $(IFLAGS) $(CFLAGS) $(CXX_COMP_FLAGS) $(LEP_COMP_FLAGS) -c $*.c        

#----------------------------------------------------------------------------
# depend
#----------------------------------------------------------------------------

depend:
	makedepend -- $(CFLAGS) -I$(LEP_INCL_PATH) -- $(LEP_SOURCES) 2>/dev/null

#----------------------------------------------------------------------------
# clean and touch
#----------------------------------------------------------------------------

allclean : clean
	$(RM) -f so_locations *~
	$(RM) -f -r ii_files

clean :
	$(RM) -f $(LEP_OBJECTS)

touch: 
	touch *.c

tags: $(CTAGFILES)
	$(CTAGS) $(CTAGFILES)

TAGS: $(ETAGFILES)
	$(ETAGS) $(ETAGFILES)
