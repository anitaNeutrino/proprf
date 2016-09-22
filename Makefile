CC=gcc
CXX=g++
CXXFLAGS=`root-config --cflags` -I$(ANITA_UTIL_INSTALL_DIR)/include -g -fPIC
CFLAGS=-g -fPIC
LDFLAGS= `root-config --ldflags` -L$(ANITA_UTIL_INSTALL_DIR)/lib
LIBS=`root-config --glibs` -g -Wl,-z,defs -lMathMore -lRootFftwWrapper -lAnitaEvent

LIBDIR=lib
BUILDDIR=build
INCLUDEDIR=include
BINDIR=bin

.PHONY: clean install all


OBJS := $(addprefix $(BUILDDIR)/, crpol.o atmos.o geomag70_mod.o dens.o dict.o)
INCLUDES := $(addprefix $(INCLUDEDIR)/, $(shell ls $(INCLUDEDIR)))

LINKLIBNAME=proprf
LIBNAME=$(LIBDIR)/lib$(LINKLIBNAME).so 


all: $(LIBNAME) 


$(LIBNAME): $(OBJS) | $(LIBDIR)
	@echo Building shared library $@
	@$(CXX) $(SOFLAGS) $(LDFLAGS) $(OBJS) $(LIBS) $(GLIBS) -shared -o $@


$(OBJS): | $(BUILDDIR)

$(BUILDDIR): 
	mkdir -p $(BUILDDIR)

$(BINDIR): 
	mkdir -p $(BINDIR)

$(LIBDIR): 
	mkdir -p $(LIBDIR)


$(BUILDDIR)/%.o: src/%.c $(INCLUDES) Makefile | $(BUILDDIR)
	@echo Compiling  $< 
	@$(CC)  -I./include $(CFLAGS) -o $@ -c $< 


$(BUILDDIR)/%.o: src/%.cc $(INCLUDES) Makefile | $(BUILDDIR)
	@echo Compiling  $< 
	@$(CXX)  -I./include $(CXXFLAGS) -o $@ -c $< 

$(BUILDDIR)/%.o: build/%.cc $(INCLUDES) Makefile | $(BUILDDIR) 
	@echo Compiling  $< 
	$(CXX)  -I../include -I./ $(CXXFLAGS) -o $@ -c $< 


$(BUILDDIR)/dict.cc: $(INCLUDES) LinkDef.h | $(BUILDDIR)
	@echo Running rootcint
	rootcint  -f $@ -c -p -I$(ANITA_UTIL_INSTALL_DIR)/include $(INCLUDES) LinkDef.h

install: 
ifndef ANITA_UTIL_INSTALL_DIR 
	$(error Please define ANITA_UTIL_INSTALL_DIR)
endif 
	install -d $(ANITA_UTIL_INSTALL_DIR)/lib 
	install -d $(ANITA_UTIL_INSTALL_DIR)/include 
	install -c -m 755 $(LIBNAME)(ANITA_UTIL_INSTALL_DIR)/lib  
	install -c -m 644 $(INCLUDES) $(ANITA_UTIL_INSTALL_DIR)/include 
clean: 
	rm -rf build
	rm -rf bin
	rm -rf lib


