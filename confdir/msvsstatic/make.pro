#-----------------------------------------------------------------------------
# BORLAND Version 5.x (win32)
#-----------------------------------------------------------------------------

include comp.lst
include Make.lst

.obj.exe:
        $(LD) $(LEDALIBS) ..\lib$(LEPNAME).lib $(WINLIBS) $*.obj

.c.obj:
        $(CC) $(LEDAFLAGS) -I..\incl -I$(LEDAROOT)\incl $*.c

.c.exe:
        $(CC) $(LEDAFLAGS) -I..\incl -I$(LEDAROOT)\incl $*.c
        $(LD) $(LEDALIBS) ..\lib$(LEPNAME).lib $(WINLIBS) $*.obj

all: $(PROGS)

clean:
	del *.obj

del:
	del *.obj
	del $(PROGS)
