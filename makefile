#############################################################
#
# Makefile for MinGW/gcc
#
#############################################################


MAKEFILE =makefile
TARGET   =orbit_dp10

# PATH := /opt/mingw-w64/bin:$(PATH)
BIN_DIR=../bin/$(ARCH)
COMPILE_PREFIX=i586-mingw32msvc-
# COMPILE_PREFIX=i686-w64-mingw32-
# COMPILE_PREFIX=i686-w64-mingw32-
# COMPILE_PREFIX=x86_64-unknown-linux-gnu-


# CC=$(COMPILE_PREFIX)g++-4.8.2
CC=$(COMPILE_PREFIX)g++
# CC=g++-4.8.2
# CC=g++-4.8

# LD=$(COMPILE_PREFIX)g++-4.8.2
# LD=gcc-4.8.2
LD=$(CC)
RM=rm
CP=cp


SOURCE_DIRS =./

#---------------- C++ source files ----------------
CPPSRC=
CPPSRC+=main.cpp
CPPSRC+=debug.cpp
CPPSRC+=comport_win.cpp
CPPSRC+=comport_itf.cpp
# CPPSRC+=comport_linux.cpp
CPPSRC+=orbit_commands.cpp
CPPSRC+=orbit_answers.cpp
CPPSRC+=orbit_network.cpp
CPPSRC+=orbit_module.cpp
CPPSRC+=orbit_dp.cpp




#**********************************************************************************

# Optimization level, can be [0, 1, 2, 3, s]. 
#     0 = turn off optimization. s = optimize for size.
#     (Note: 3 is not always the best optimization level. See FAQ.)
OPT =3


# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
#     Use forward slashes for directory separators.
#     For a directory that has spaces, enclose it in quotes.
EXTRAINCDIRS = $(SOURCE_DIRS)


# Compiler flag to set the C Standard level.
#     c89   = "ANSI" C
#     gnu89 = c89 plus GCC extensions
#     c99   = ISO C99 standard (not yet fully implemented)
#     gnu99 = c99 plus GCC extensions
#CSTANDARD = -std=gnu99
CSTANDARD = -std=c99

# Compiler flag to set the C Standard level.
# CPPSTANDARD = -std=c++0x



# Place -D or -U options here for C sources
CDEFS=
CDEFS+=$(BUILD_NUMBER_DEF)
# CDEFS += -DF_CPU

# Place -D or -U options here for C++ sources
CPPDEFS=
CPPDEFS+=$(BUILD_NUMBER_DEF)
# CPPDEFS+= -DF_CPU



#----------------------------------------------------
# build with multiple make threads 
# without any restriction
# MAKEOPTIONS=-j 1
MAKEOPTIONS+=-j -l


#---------------- Compiler Options C ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CFLAGS += -O$(OPT)
CFLAGS = -g$(DEBUG)
CFLAGS += $(CDEFS)
CFLAGS += -O$(OPT)
CFLAGS += -funsigned-char
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -Wall
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wa,-adhlns=$(notdir $(<:%.c=%.lst))
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += $(CSTANDARD)


#---------------- Compiler Options C++ ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CPPFLAGS = -g$(DEBUG)
CPPFLAGS += $(CPPDEFS)
CPPFLAGS += -O$(OPT)
CPPFLAGS += -funsigned-char
CPPFLAGS += -funsigned-bitfields
CPPFLAGS += -fshort-enums
# CPPFLAGS += -fno-exceptions
CPPFLAGS += -Wall
# CPPFLAGS += -Wundef
CPPFLAGS +=-Wno-unknown-pragmas 
#CPPFLAGS += -mshort-calls
#CPPFLAGS += -fno-unit-at-a-time
#CPPFLAGS += -Wstrict-prototypes
#CPPFLAGS += -Wunreachable-code
#CPPFLAGS += -Wsign-compare
CPPFLAGS += -Wa,-adhlns=$(<:%.cpp=%.lst)
CPPFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CPPFLAGS +=$(CPPSTANDARD)

# CPPFLAGS +=-std=c++11
CPPFLAGS +=-std=c++0x
# !!!!  this options causes crash of OPC server  !!!!
# CPPFLAGS += -fpack-struct   




#---------------- Linker Options ----------------
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
LDFLAGS = 
LDFLAGS += -Wl,--gc-sections	#
# LDFLAGS += -ladvapi32 -lole32 -loleaut32 -luuid 
# LDFLAGS += $(LOLIB)/libightopc.a
# LDFLAGS += -lwinmm



# Uncomment one choice:
# LDFLAGS+=-Wl,--subsystem,windows
# LDFLAGS+=-Wl,--subsystem,console
LDFLAGS+=-static-libgcc
LDFLAGS+=-static

# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------
MSG_SIZE_BEFORE = Size before: 
MSG_SIZE_AFTER = Size after:
MSG_COFF = Converting to AVR COFF:
MSG_EXTENDED_COFF = Converting to AVR Extended COFF:
MSG_FLASH = Creating load file for Flash:
MSG_EEPROM = Creating load file for EEPROM:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling C:
MSG_COMPILING_CPP = Compiling C++:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:
MSG_CREATING_LIBRARY = Creating library:
MSG_FILE_NOT_FOUND = File not found:


# Define all object files.
OBJ = $(SRC:%.c=%.o) $(CPPSRC:%.cpp=%.o) $(ASRC:%.S=%.o) 

# Define all listing files.
LST = $(SRC:%.c=%.lst) $(CPPSRC:%.cpp=%.lst) $(ASRC:%.S=%.lst) 


# Compiler flags to generate dependency files.
GENDEPFLAGS = -MMD -MP -MF $(@F).d

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -I. $(CFLAGS) $(GENDEPFLAGS)
ALL_CPPFLAGS = -I. -x c++ $(CPPFLAGS) $(GENDEPFLAGS)





############################################################
# Objects & common rules

# virtual all target
all:
	@echo
	@echo "Compiling with ${MAKEOPTIONS}"
	@echo $(PATH)
	make -f ${MAKEFILE} clean
	time make -f ${MAKEFILE} ${MAKEOPTIONS} _all


#
_all: begin gccversion $(TARGET) end


# Display compiler version information.
gccversion : 
	@echo
	@$(CC) --version

begin:
	@echo
	@echo $(MSG_BEGIN)


end:
	@echo $(MSG_END)
	@echo




$(BIN_DIR)/$(TARGET): $(TARGET)
	$(CP) $(TARGET) $(BIN_DIR)
#       $(CP) $^ ~/data/VirtualBox/Windows_XP/share/OPC


installer: $(BIN_DIR)/$(TARGET)

clean-all: clean
	-$(RM) *.exe

clean:
	-$(RM) $(LST) 
	-$(RM) $(OBJ)
	-$(RM) *.d




# Compile: create object files from C++ source files.
%.o : %.cpp $(MAKEFILE)
	@echo
	@echo $(MSG_COMPILING_CPP) $<
	$(CC) -c $(ALL_CPPFLAGS) $< -o $@ 


# Compile: create object files from C source files.
%.o : %.c $(MAKEFILE)
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS) $< -o $@ 

# Create preprocessed source for use in sending a bug report.
%.i : %.c $(MAKEFILE)
	$(CC) -E -I. $(CFLAGS) $< -o $@ 




$(TARGET): $(OBJ) 
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(ALL_CFLAGS) $^ --output $@ $(LDFLAGS)


# include all dependencies
include $(wildcard *.d)

# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter gccversion \
build elf hex eep lss sym coff extcoff \
clean clean_list program  debug gdb-config boot_program boot boot_all boot_force
