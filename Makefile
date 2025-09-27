# Disable built-in rules
.SUFFIXES:

ifndef GBDK_HOME
GBDK_HOME = "../gbdk/"
endif

LCC = $(GBDK_HOME)bin/lcc 

LDFLAGS = -Iinclude/ -Wl-m -Wl-w

# Enable optimizations, as suggested by gbdk (https://gbdk.org/docs/api/docs_coding_guidelines.html#autotoc_md80)
LDFLAGS += -Wf--max-allocs-per-node50000 -Wf--peep-asm -Wf--peep-return

# Don't include anything from GBDK whatsoever, I do everything manually
LDFLAGS += -no-crt -no-libs

# I don't like the shadow oam symbol name, so I map my own symbol (gOamBuffer) to the shadow oam instead
# The reason I also remap the shadow oam to its default address is because otherwise, the gOamBuffer mapping
# is done before the mapping of the shadow oam, so it ends up being 0
# Mapping it manually to the default value like this ensure the mapping is done in the correct order
LDFLAGS += -Wl-g_shadow_OAM=0xC000 -Wl-g_gOamBuffer=_shadow_OAM

# GBDK_DEBUG = ON
ifdef GBDK_DEBUG
	LDFLAGS += -debug -v
endif

# Add --Werror for .c and .s files compilation only, using this flag during linking give a warning
CFLAGS = $(LDFLAGS) -Wf--Werror

# Make the "control flow changed" warning shut up
CFLAGS += -Wf--less-pedantic

# Specify rom type 1 (MBC1), and the amount of banks (4)
LDFLAGS += -Wl-yt1 -Wl-yo4


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
# IMPORTANT Make sure to link the files in asm/ first
# this will make crt0.s the first file to be linked, which is necessary to properly link the ROM
ASMSOURCES  = $(shell find $(ASMDIR) -name '*.s') $(CSOURCES:.c=.s)
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
	$Q$(LCC) $(CFLAGS) -S -o $@ $<

# Compile .s assembly files in to .o object files
%.o: %.s
	$(MSG) AS $@
	$Q$(LCC) $(CFLAGS) -c -o $@ $<

# Link the compiled object files into a .gb ROM file
$(BINS): $(OBJS)
	$(MSG) LD $(BINS)
	$Q$(LCC) $(LDFLAGS) -o $(BINS) $(OBJS)

prepare:
	mkdir -p $(OBJDIR)

clean:
#	rm -f  *.gb *.ihx *.cdb *.adb *.noi *.map
	$(MSG) rm *.o
	$Qfind . -name '*.o' -delete
	$(MSG) rm *.s
	$Qrm -f $(CSOURCES:.c=.s)
	$(MSG) rm obj/
	$Qrm -f $(OBJDIR)/*.*
