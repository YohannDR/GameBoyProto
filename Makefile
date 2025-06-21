# Disable built-in rules
.SUFFIXES:

# If you move this project you can change the directory 
# to match your GBDK root directory (ex: GBDK_HOME = "C:/GBDK/"
GBDK_HOME = "/mnt/c/Users/yohan/Documents/gbdk/"

LCC = $(GBDK_HOME)bin/lcc 

LCCFLAGS = -Iinclude/ -Wl-m

# GBDK_DEBUG = ON
ifdef GBDK_DEBUG
	LCCFLAGS += -debug -v
endif

# Enable verbose output
ifeq ($(V),1)
	Q =
	MSG = @:
else
	Q = @
	MSG = @echo " "
endif

# You can set the name of the .gb ROM file here
PROJECTNAME = Proto

SRCDIR      = src
ASMDIR		= asm
OBJDIR      = obj
BINS	    = $(OBJDIR)/$(PROJECTNAME).gb
CSOURCES    = $(shell find $(SRCDIR) -name '*.c')
ASMSOURCES  = $(CSOURCES:.c=.s) $(shell find $(ASMDIR) -name '*.s')
OBJS		= $(ASMSOURCES:.s=.o)

.PRECIOUS: $(ASMSOURCES) $(OBJS)

all:	prepare $(BINS)

compile.bat: Makefile
	@echo "REM Automatically generated from Makefile" > compile.bat
	@make -sn | sed y/\\//\\\\/ | sed s/mkdir\ -p\/mkdir\/ | grep -v make >> compile.bat

$(BINS): $(OBJS)

# Compile .c files in to .s assembly files
%.s: %.c
	$(MSG) CC $@
	$Q$(LCC) $(LCCFLAGS) -S -o $@ $<

# Compile .s assembly files in to .o object files
%.o: %.s
	$(MSG) AS $@
	$Q$(LCC) $(LCCFLAGS) -c -o $@ $<

# Link the compiled object files into a .gb ROM file
$(BINS): $(OBJS)
	$(MSG) LD $(BINS)
	$Q$(LCC) $(LCCFLAGS) -o $(BINS) $(OBJS)

prepare:
	mkdir -p $(OBJDIR)

clean:
#	rm -f  *.gb *.ihx *.cdb *.adb *.noi *.map
	rm -f  $(SRCDIR)/*.s
	rm -f  $(SRCDIR)/*.o
	rm -f  $(ASMDIR)/*.o
	rm -f  $(OBJDIR)/*.*
