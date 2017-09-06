#Main Makefile
CC = gcc
CFLAGS = -g -Wall -MMD -ansi

#Binary
ifeq ($(OS),Windows_NT)
    BIN = main.exe
else
    BIN = main
endif

#Directories
IDIR = ./include
SDIR = ./src

ifeq ($(OS),Windows_NT)
	ODIR = ./obj/windows
else
	ODIR = ./obj/linux
endif

#Files
HEADER = .h
SOURCE = .c

#Paths
INCLUDE_PATHS = -I$(IDIR)

#Libraries
LIBRARIES = 

#Compilation line
COMPILE = $(CC) $(CFLAGS) $(INCLUDE_PATHS)

#FILEs
#---------------Include---------------#
INCS = $(wildcard $(IDIR)/*$(HEADER)) $(wildcard $(IDIR)/*/*$(HEADER))

#---------------Source----------------#
SRCS = $(wildcard $(SDIR)/*$(SOURCE)) $(wildcard $(SDIR)/*/*$(SOURCE))

#---------------Object----------------#
OBJS = $(SRCS:$(SDIR)/%$(SOURCE)=$(ODIR)/%.o)
#-------------Dependency--------------#
DEPS = $(SRCS:$(SDIR)/%$(SOURCE)=$(ODIR)/%.d)

all: $(OBJS)
	$(COMPILE) $(OBJS) main$(SOURCE) -o $(BIN) $(LIBRARIES)

dll: LIBRARIES+= -lm -fPIC
dll: $(OBJS)
	$(COMPILE) -shared -o libguisdl.so $(OBJS) $(LIBRARIES)

# Include all .d files
-include $(DEPS)

$(ODIR)/%.o: $(SDIR)/%$(SOURCE)
	$(COMPILE) -MMD -c $< -o $@ $(LIBRARIES)

.PHONY : clean
clean :
	-rm $(BIN) $(OBJS) $(DEPS)
