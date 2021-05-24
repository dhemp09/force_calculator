CXX=g++
CXXFLAGS= -g -std=c++11
MKDIR = mkdir -p
OUT_DIRS = data csvs
LIBS=

all: gsfc dirs
dirs: $(OUT_DIRS)
$(OUT_DIRS):
	$(MKDIR) $(OUT_DIRS)
gsfc: gsfc.o
	$(CXX) $(CXXFLAGS) -o gsfc gsfc.o
gsfc.o:
	$(CXX) $(CXXFLAGS) -c gsfc.cpp
clean:
	-rm -f *.o gsfc *~
