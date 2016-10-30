
INCLUDE = -I/usr/local/include \
		  -I$(ProjDir)/include

LIBES = -L/usr/local/lib

sourcefiles = $(wildcard $(SrcDir)/*.cpp)
objfiles = $(addprefix $(ObjDir)/,$(subst .cpp,.o,$(notdir $(sourcefiles))))

OPENCV_LIB = -l:libopencv_core.so.2.4 -l:libopencv_highgui.so.2.4 -l:libopencv_imgproc.so.2.4

CXXFLAGS = -O3 -g -Wall -std=c++11 -Wc++11-compat

.PHONY: all

all: $(objfiles)
	$(CXX) -o $(OutDir)/Test $^ $(CXXFLAGS) $(LIBES) $(OPENCV_LIB)

$(ObjDir)/%.o: $(SrcDir)/%.cpp %.cpp.d
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE) $(LIBES)

%.cpp.d: $(SrcDir)/%.cpp
	$(CXX) $< -MM $(INCLUDE) > $(notdir $<.d)

clean:
	rm -rf out
