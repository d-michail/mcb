#-----------------------------------------------------------------------------
# BORLAND Version 5.x (win32)
#-----------------------------------------------------------------------------

CC  = bcc32 -c -P -O2
PLAIN_C = bcc32 -c -P- -O2
LD  = bcc32

!include Make.lst

.obj.exe:
        $(LD) -L.. -L$(LEDAROOT) lib$(LEPNAME).lib $(WINLIBS) $(LEDALIBS) $*.obj

.c.obj:
        $(CC) $(LEDAFLAGS) -I..\incl -I$(LEDAROOT)\incl $*.c

.c.exe:
        $(CC) $(LEDAFLAGS) -I..\incl -I$(LEDAROOT\incl $*.c
        $(LD) -L.. -L$(LEDAROOT) lib$(LEPNAME).lib $(WINLIBS) $(LEDALIBS) $*.obj

all: $(PROGS)

clean:
	del *.obj

del:
	del *.obj
        del *.tds
	del $(PROGS)
