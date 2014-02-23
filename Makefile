CXX = gcc
CCFLAGS = -g -Wall -Werror -ansi -pedantic -fpic

LIBS = -lBibtexManager
LIBSDIR = -L/usr/lib/ -L.
INCLUDEDIR = -I. -I/usr/include/

LIBNAME = libBibtexManager.so
LIBFILES = bibtexmanager
SOURCELIBFILES = $(LIBFILES:=.c)
OBJECTLIBFILES = $(LIBFILES:=.o)

TARGET = $(TARGETFILES).exe
TARGETFILES = bibtexmain
SOURCETARGETFILE = $(TARGETFILES:=.c)
OBJECTTARGETFILE = $(TARGETFILES:=.o)

INITFILE = init

all: clean $(TARGET)
	@echo "\n*************************************************************************\n*Now you can run the project by entering ./"$(TARGET)" in the console*\n*OR enter \"make launch\" in the console                                  *\n*---->it'll launch the program after the common makefile!               *\n*************************************************************************"

$(TARGET): $(OBJECTTARGETFILE) lib
	@echo "\nGeneration of " $(TARGET)
	$(CXX) $(CCFLAGS) $(LIBSDIR) -o $(TARGET) $(OBJECTTARGETFILE) $(LIBS)

.c.o:
	@echo "\nGeneration of " $@ " from " $<
	$(CXX) $(INCLUDEDIR) $(CCFLAGS) -c -o $@ $<

lib: $(OBJECTLIBFILES)
	@echo "\nLibrary Generation named " $(LIBNAME)
	$(CXX) $(CCFLAGS) -shared -o $(LIBNAME) $(OBJECTLIBFILES)

clean:
	@echo "\n Cleaning current directory"
	rm -f *.o *~ *.txt *.so *.exe

launch: clean $(TARGET)
	@echo "*****************\nLAUNCHING PROGRAM\n*****************\n"
	./$(TARGET)

open:
	gedit ./$(TARGETFILES).c ./$(LIBFILES).h ./$(LIBFILES).c ./Makefile

init: clean
	$(CXX) $(CCFLAGS) $(INITFILE).c -o $(INITFILE).exe
	./$(INITFILE).exe
