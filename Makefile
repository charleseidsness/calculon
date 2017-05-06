# Installation Directory
PREFIX	= .

# Source Code Diectories
SRC_DIR = .
INC_DIR =  ../../include

# Quiet (set to @ for a quite compile)
Q	?= @
#Q	?= 

# Build Tools
CROSS	?=
CC 	:= $(CROSS)gcc
AR 	:= $(CROSS)ar
RANLIB 	:= $(CROSS)ranlib
OBJDUMP	:= $(CROSS)objdump

# Tool Flags
CFLAGS 	?= -Wall -O2 -I$(INC_DIR) -I./src -I.
LDFLAGS ?= -Wall -O2
RE2C = re2c -b -s
LEMON = lemon
MAKEHEADERS = makeheaders
ARFLAGS = -cr
ifeq ($(OS), Windows_NT)
	#CFLAGS += -mno-cygwin
	#LDFLAGS += -mconsole -mno-cygwin
	LDFLAGS += -mconsole
else
	
endif

LIB = libcalc.a
LIB_OBJ = $(SRC_DIR)/parser.o $(SRC_DIR)/tokenizer.o $(SRC_DIR)/calc.o

LIBS =  $(LIB)
EXE_LIBS = $(LIBS)
EXE_OBJ = $(SRC_DIR)/main.o
ifeq ($(OS), Windows_NT)
	EXE = calculon.exe
else
	EXE = calculon
endif

.PHONY: all clean library

all: Makefile $(EXE)

$(EXE): $(LIBS) $(EXE_OBJ)
	@echo LD $@
	$(Q)$(CC) $(LDFLAGS) -o $(EXE) $(EXE_OBJ) $(EXE_LIBS)

library: $(LIB)

$(LIB):$(LIB_OBJ)
	@echo AR $@
	$(Q)$(AR) $(ARFLAGS) $@ $(LIB_OBJ)
	$(Q)$(RANLIB) $@
	
clean:
	@echo Cleaning...
	$(Q)rm -vf $(EXE_OBJ) $(EXE)
	$(Q)rm -vf $(LIB_OBJ) $(LIB)  $(SRC_DIR)/parser.h 
	$(Q)rm -vf $(SRC_DIR)/parser.c $(SRC_DIR)/tokenizer.c
	$(Q)rm -vf *~ */*~

%.o:%.c
	@echo CC $@
	$(Q)$(CC) $(CFLAGS) -I$(dir $@)../include -c -o $@ $<
	
%.dis:%.o
	@echo DIS $@
	$(Q)$(OBJDUMP) -D $< >> $@

%.c:%.re
	@echo RE2C $@
	$(Q)$(RE2C) -o $@ $<

%.c:%.lem
	@echo LEMON $@
	$(Q)$(LEMON) -m $<
	
$(SRC_DIR)/parser.h:$(SRC_DIR)/parser.c
	$(Q)rm -vf $@
	$(Q)$(MAKEHEADERS) $<

$(SRC_DIR)/parser.o:$(SRC_DIR)/parser.h
$(SRC_DIR)/tokenizer.o:$(SRC_DIR)/parser.h

