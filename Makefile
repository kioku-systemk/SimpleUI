INCS=
LIBS=-lGL

DEFINES = -g

CXX=g++

all:
	$(CXX) $(INCS) $(DEFINES) test.cpp Core/CoreWindow_x11.cpp $(LIBS)
