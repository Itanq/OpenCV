SRCDIR=../src
HEADIR=../include
OBJDIR=../obj

sourcefiles=$(wildcard $(SRCDIR)/*.cpp)
objectfiles=$(subst .cpp,.o,$(notdir $(sourcefiles)))

CXXFLAGS += -g -Wall -std=c++11  -shared -fPIC

INCLUDE=-I/usr/local/include/ \
		-I$(HEADIR)

LIBES=-L/usr/local/

CVLIBES=-l:libopencv_core.so.2.4 -l:libopencv_highgui.so.2.4 -l:libopencv_imgproc.so.2.4

.PHONY:all

all:$(objectfiles)

$(OBJDIR)%.o:$(SRCDIR)/%.cpp $(SCRDIR)/%.cpp.d
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDE) $(LIBES) $(CVLIBES)

$(SRCDIR)/%.cpp.d:$(SRCDIR)/%.cpp
	$(CXX) -MM $< 

clean:
	rm  -rf $(OBJDIR)/*.o 
