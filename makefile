# project Penguin

CFLAGS_C = $(filter-out -include "sdk.h",$(CFLAGS))

# -----------------------------------------

# MAKE_DEP = -MMD -MT $@ -MF $(@:.o=.d)

CFLAGS = `pkg-config gtk+-2.0 --cflags`
INCLUDES = 
LDFLAGS = `pkg-config gtk+-2.0 --libs`
RCFLAGS = 
LDLIBS = $(T_LDLIBS)  -lstdc++

LINK_exe = gcc -o $@ $^ $(LDFLAGS) $(LDLIBS)
COMPILE_c = gcc $(CFLAGS_C) -o $@ -c $< $(MAKEDEP) $(INCLUDES)

%.o : %.c ; $(COMPILE_c)
.SUFFIXES: .o .d .c

all: all.targets all.after

all.after : $(FIRST_TARGET)
	
all.targets : Release_target Debug_target 

clean :
	rm -fv $(clean.OBJ)
	rm -fv $(DEP_FILES)
	rm PenguinRelease
	rm PenguinDebug

.PHONY: all clean distclean

# -----------------------------------------
# Release_target

Release_target.BIN = bin/Release/1
Release_target.OBJ = src/assertion.o src/main.o src/mydic.o src/participle.o src/pyFile.o src/pyGraphic.o 
Release_target.SOL = src/libassertion.so
DEP_FILES += src/assertion.d src/main.d src/mydic.d src/participle.d src/pyFile.d src/pyGraphic.d 
clean.OBJ += $(Release_target.BIN) $(Release_target.OBJ) $(Release_target.SOL)

Release_target : $(Release_target.SOL) Release_target.after_always

Release_target.after_always : PenguinRelease
	
$(Release_target.SOL) : $(Release_target.BIN)
	gcc src/assertion.c -fPIC -shared -o src/libassertion.so

$(Release_target.BIN) : $(Release_target.OBJ)
	$(LINK_exe)

PenguinRelease:
	cp $(Release_target.BIN) ./PenguinRelease

# -----------------------------------------
# Debug_target

Debug_target.BIN = bin/Debug/1
Debug_target.OBJ = src/assertion.o src/main.o src/mydic.o src/participle.o src/pyFile.o src/pyGraphic.o 
Debug_target.SOL = src/libassertion.so
DEP_FILES += src/assertion.d src/main.d src/mydic.d src/participle.d src/pyFile.d src/pyGraphic.d 
clean.OBJ += $(Debug_target.BIN) $(Debug_target.OBJ) $(Debug_target.SOL)

Debug_target : $(Debug_target.SOL) Debug_target.after_always
Debug_target : CFLAGS += -g

Debug_target.after_always : PenguinDebug
	
$(Debug_target.SOL) : $(Debug_target.BIN)
	gcc src/assertion.c -fPIC -shared -o src/libassertion.so

$(Debug_target.BIN) : $(Debug_target.OBJ)
	$(LINK_exe)

PenguinDebug:
	cp $(Debug_target.BIN) ./PenguinDebug
